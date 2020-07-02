#include "cgen.h"
#ifndef GLOBALS_H
#include "globals.h"
#endif
#include "hashtable.h"

#include<stdio.h>
#include<string.h>

static int nxq = 0;
static int tmpcnt = 0;

#define JNZ 400 // (jnz, A1, ,p)
#define JROP 401 // (jrop, A1, A2, p)
#define JMP 402 // 无条件跳转
#define JZ 403 // (jnz, A1, ,p)

char* get_op(int op){
    if(op < 256){
        return get_token_str(op);
    }else{
        if(op == JNZ){
            return "jnz";
        }else if(op == JROP){
            return "jrop";
        }else if(op == JMP){
            return "jmp";
        }else if(op == JZ){
            return "jz";
        }
        return get_token_str(op);
    }
}

void printQuadruple(){
    for(int i = 0;i<nxq;i++){
        printf("%d : <\"%s\", %s,%s,%s>",i,
               get_op(quads[i].op),quads[i].arg1,quads[i].arg2,quads[i].result);
        if(quads[i].comment){
            printf("-> %s",quads[i].comment);
        }
        printf("\n");
    }
}

// 产生临时变量的函数，每次调用都定义一个新的临时变量。返回值为该变量的编号。
int newTmp(){
    return tmpcnt++;
}


// 产生一个四元式并放入中间代码表中
// 每次调用,NXQ++；
void gen(int op,char* arg1,char* arg2,char* result){
    quads[nxq].op = op;
    quads[nxq].arg1 = arg1;
    quads[nxq].arg2 = arg2;
    quads[nxq].result = result;
    nxq += 1;
}

void setTargetP(int i,int target){
    quads[i].result = int_to_str(target);
}

void setCommemt(char* comment){
    quads[nxq].comment = comment;
}

char* getTmpStr(int id){
    //static char s[32];
    char* s = (char*) malloc(32);
    strcpy(s,"tmp");
    char* s1 = int_to_str(id);
    size_t i = 0;
    while(i < 32 && s1[i] != 0){
        s[i + 3] = s1[i];
        i++;
    }
    s[i + 3] = 0;
    return s;
}

int getTmpId(char* s){
    int res = 0;
    size_t i = 3;
    while(i < 32 && s[i]!=0){
        res *= 10;
        res += (s[i] - '0');
        i++;
    }
    return res;
}



void genc(TreeNode* tree);

// 输出一条语句的四元式
void genStmt(TreeNode* tree){
    if(tree == NULL) return;
    TreeNode *p1,*p2,*p3;
    int savedLoc1, savedLoc2,curLoc;

    switch (tree->kind.stmt) {
    case IfK:
        p1 = tree->child[0];
        p2 = tree->child[1];
        p3 = tree->child[2];

        // 为测试表达式产生代码
        genExp(p1);
        setCommemt("如果condition为false则跳转到else语句，或者if语句块之外");
        savedLoc1 = nxq;

        // 测试结果保存在tmp$id
        gen(JZ,getTmpStr(tree->child[0]->tid),"_",0); // 需要填充
        // condition-statement 如果为0，则跳转到else语句

        // 在then部分递归
        genStmt(p2);
        savedLoc2 = nxq;
        setCommemt("如果condition为true则跳转到if语句块之外(else不执行)");
        gen(JNZ,getTmpStr(tree->child[0]->tid),"_",0); // 需要填充
        // 如果condition-statement为真,则不能执行else语句块

        // 在else上递归
        genStmt(p3);

        //拉链回填
        curLoc = nxq;
        setTargetP(savedLoc1,savedLoc2);
        setTargetP(savedLoc2,curLoc);
        break;
    case WhileK:
        savedLoc1 = nxq;
        setCommemt("开始do-while");
        // 执行do-while循环体
        genStmt(tree->child[0]);

        // 进行判断是否还要继续循环
        genExp(tree->child[1]);

        // 判断结果保存到临时变量
        setCommemt("如果判断结果为真，则继续做do-while循环");
        gen(JNZ,getTmpStr(tree->child[1]->tid),"_",int_to_str(savedLoc1));
        break;
    case StmtSeq:
        tree = tree->child[0];
        while(tree != NULL){
            genc(tree);
            tree = tree->sibling;
        }
        break;
    default:
        break;
    }
}

// 输出一条表达式的四元式
void genExp(TreeNode* tree){
    if(tree == NULL) return;

    TreeNode *p1,*p2;
    switch (tree->kind.exp) {
    case IntK: // 常数
        tree->tid = newTmp();
        gen('=',int_to_str(tree->attr.val),"_",getTmpStr(tree->tid));
        break;
    case IdK:
        if(search(tree->attr.name) == NULL){
            tree->tid = newTmp();
            gen('=',tree->attr.name,"_",getTmpStr(tree->tid));
            put(tree->attr.name,getTmpStr(tree->tid));
        }else{
            tree->tid = getTmpId(search(tree->attr.name)->value);
        }
        break;
    case StringK:
        tree->tid = newTmp();
        gen('=',tree->attr.name,"_",getTmpStr(tree->tid));
        break;
    case OpK:
        switch (tree->attr.op) {
        case '=':
            p1 = tree->child[0];
            p2 = tree->child[1];
            genExp(p1);
            genExp(p2);
            // 赋值符号不会向上返回值，所以不需要分配临时变量
            //tree->tid = newTmp();
            setCommemt("赋值语句");
            gen(tree->attr.op,getTmpStr(tree->child[1]->tid),"_",tree->child[0]->attr.name);

            // 更新相应的哈希表
            if( search(tree->child[0]->attr.name) != NULL){
                setCommemt("更新临时变量");
                gen('=',tree->child[0]->attr.name,"_",search(tree->child[0]->attr.name)->value);
            }
            break;
        default:
            p1 = tree->child[0];
            p2 = tree->child[1];
            genExp(p1);
            genExp(p2);
            tree->tid = newTmp();
            gen(tree->attr.op,getTmpStr(tree->child[0]->tid),
                    getTmpStr(tree->child[1]->tid),getTmpStr(tree->tid));
            break;
        }
        break;
    default:
        break;
    }
}

void genc(TreeNode* tree){
    if (tree)
    {
        switch (tree->nodekind)
        {
        case StmtK:
            genStmt(tree);
            break;
        case ExpK:
            genExp(tree);
            break;
        }
        //cGen(tree->sibling);
    }
}
