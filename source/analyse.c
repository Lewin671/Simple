#ifndef GLOBALS_H
#include "globals.h"
#endif
#ifndef SYMBOL_H
#include "symbol.h"
#endif
#include "analyse.h"
#include<stdio.h>

/*counter for variable memory location*/
static int location = 0;

static void typeError(TreeNode* t,char* msg){
    printf("Type error at line %d: %s\n",t->lineno,msg);
}

static void traverse(TreeNode * t,
                     void(*preProc) (TreeNode *),
                     void(*postProc) (TreeNode *))
{
    if (t != NULL)
    {
        // 先序操作
        //if(t->child[0] == NULL && t->child[1] == NULL && t->child[2] == NULL)
        preProc(t);

        {
            int i;
            for (i = 0; i < MAX_CHILDREN_NUMBER; i++)
                traverse(t->child[i], preProc, postProc);
        }
        // 后序操作
        postProc(t);
        traverse(t->sibling, preProc, postProc);
    }
}

static void nullProc(TreeNode * t)
{
    if (t == NULL) return;
    else return;
}


// 插入ID到符号表
static void insertNode(TreeNode* t){
    TreeNode* p_tmp = t;
    int exp_type_tmp = -1;
    switch (t->nodekind) {
    case StmtK:
        switch (t->kind.stmt) {
        case DefineK:
            p_tmp = t->child[1];
            ExpType now_type = p_tmp->type;

            while (p_tmp != NULL) {
                // 如果符号未定义
                if(st_lookup_type(p_tmp->attr.name) == -1){
                    // 插入符号表
                    st_insert(p_tmp->attr.name,p_tmp->lineno,location++,now_type);
                }else{
                    typeError(t,"error! this variable has been defined.");
                }
                p_tmp = p_tmp->sibling;
            }
            break;
        default:
            break;
        }
        break;
    case ExpK:
        switch (t->kind.exp) {
        case IdK:
            exp_type_tmp = st_lookup_type(t->attr.name);
            if(exp_type_tmp == -1){
                printf("undefined error!name:%s,line:%d,__%d\n", p_tmp->attr.name,
                       p_tmp->lineno,p_tmp->type);
            }else{
                st_insert(t->attr.name, t->lineno, 0, exp_type_tmp);
                t->type = exp_type_tmp;
            }
            break;
        default:
            break;
        }
        break;
    }
}


void buildSymtab(TreeNode *tree){
    traverse(tree,insertNode,nullProc);
    printf("符号表建立完毕!\n");
    printSymTab();
}

// 检查单个结点的类型
static void checkNode(TreeNode* t){

    switch (t->nodekind) {
    case ExpK:
        switch (t->kind.exp) {
        case OpK:
            if((t->child[0]->type  != t->child[1]->type)){
                typeError(t,"操作数类型不匹配");
            }

            if(t->attr.op == EQ || t->attr.op == NE || t->attr.op == GE ||
                    t->attr.op == LE || t->attr.op == '>' || t->attr.op == '<'||
                    t->attr.op == AND || t->attr.op == OR || t->attr.op == '!'){
                t->type = Boolean;
            }else{ // + - * /
                t->type = Integer;
            }
            break;
        case IdK:
            if(st_lookup_type(t->attr.name) == -1){
                printf("%d  %s   ", t->lineno,t->attr.name);
                typeError(t, "ID type Error___");
            }
            break;
        default:
            break;
        }
        break;
    case StmtK:
        switch (t->kind.stmt) {
        case IfK:
            if(t->child[0]->type != Boolean){
                typeError(t->child[0],"if [0] is not Boolean!");
            }
            break;
        case WhileK:
            if(t->child[1]->type != Boolean){
                typeError(t->child[1],"While [1] is not Boolean");
            }
            break;
        default:
            break;
        }
    default:
        break;
    }
}

void typeCheck(TreeNode* tree){
    traverse(tree,nullProc,checkNode);
}
