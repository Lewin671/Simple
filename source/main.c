#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef PARSE_H
#include "parse.h"
#endif

#ifndef PARSE_H
#include "analyse.h"
#endif

#include"symbol.h"

#include "cgen.h"
#include <stdio.h>

extern FILE *yyin;
int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0){
            printf("Usage: \n");
            printf("./main: run the program with input from terminal\n");
            printf("./main file: run the program with the given file\n");    
            return 0;
        }
        yyin = fopen(argv[1], "rb+");
    }
    TreeNode *t = parse();
    printf("\nprogram:\n");
    printGrammerTree(t);

    printf("开始语义分析\n");
    // 建立符号表
    buildSymtab(t->child[0]);
    typeCheck(t);

    // 生成中间代码
    genStmt(t);
    printQuadruple();
    return 0;
}
