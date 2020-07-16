#ifndef GLOBALS_H
#define GLOBALS_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_GEN_LEN 10240 // 中间代码数组长度

extern int cur_line_num;
extern int token;
extern char *token_str;
extern int yylex ();
extern char* yytext;
/*
当token为单个字符时，这个字符就是它的token值
当token为EOF时，返回0
当token为多字符时，返回tokentype中的其中一个
*/
// 除单字符运算符外的 token 的编号则在下面
// 该文件中同时提供了一个 print_token 函数，可以根据 token 的编号打印其名称

typedef enum
{
    LE = 256,
    GE,
    EQ,
    NE, //比较表达式
    AND,
    OR, //逻辑操作符号
    STRING,
    ID,
    INT,
    BOOL, //声明
    STRING_CONST,
    INT_CONST,
    DO,
    WHILE,
    IF,
    ELSE,
    THEN,
    END
} TokenType;

// 获取下一个token函数
void next_token();
void print_token();
char* get_token_str(int token);

#define MAX_CHILDREN_NUMBER 3 // 语法树的子树最多分支为3

typedef enum
{
    StmtK,
    ExpK
} NodeKind;

typedef enum
{
    IfK,
    // AssignK,把赋值语句看成是一种operation，所以归属到expression类
    WhileK,
    DefineK,
    StmtSeq
} StmtKind;

typedef enum
{
    OpK,
    IdK,
    //BoolK,
    StringK,
    IntK,
    TypeK
} ExpKind;

/* ExpType is used for type checking, 语义分析的时候需要用到*/
typedef enum
{
    Void,
    Integer,
    Boolean,
    String
} ExpType;

typedef struct treeNode
{
    struct treeNode *child[MAX_CHILDREN_NUMBER];
    struct treeNode *sibling;
    int lineno;
    NodeKind nodekind;
    union {
        StmtKind stmt;
        ExpKind exp;
    } kind;
    union {
        int op;
        int val;
        char *name;
    } attr;
    ExpType type; /* for type checking of exps */

    //临时变量的id
    int tid;
} TreeNode;

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode *newStmtNode(StmtKind kind);

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 * 创建一个相应类型的表达式结点
 */
TreeNode *newExpNode(ExpKind kind);

void syntaxError(char *message);

// 输出语法树部分，参考tiny
static size_t indentno = 0; // 缩进空格个数 

/* macros to increase/decrease indentation */
#define INDENT indentno += 2
#define UNINDENT indentno -= 2
/* printSpaces indents by printing spaces */
static void printSpaces();

void printGrammerTree(TreeNode *tree);
char *copyString(char *s);

// 辅助函数
char* int_to_str(int num);
int str_to_int(char* str);
#endif
