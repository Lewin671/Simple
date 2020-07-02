#ifndef CGEN_H
#define CGEN_H
#ifndef GLOBALS_H
#include "globals.h"
#endif


typedef struct quadruple{
    int op;
    char *arg1,*arg2,*result;
    char* comment;
}Quadruple;

Quadruple quads[MAX_GEN_LEN];

void printQuadruple();

// 产生临时变量的函数，每次调用都定义一个新的临时变量。返回值为该变量的编号。
int newTmp();

// 产生一个四元式并放入中间代码表中
// 每次调用,NXQ++；
void gen();

// 输出一条语句的四元式
void genStmt(TreeNode* tree);

// 输出一条表达式的四元式
void genExp(TreeNode* tree);
#endif // CGEN_H
