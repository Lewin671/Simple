#include "globals.h"

// 词法分析的token和token string
int token;
char *token_str;
int cur_line_num = 1;

// token string长度大于1的集合
static char *token_strs[] = {
    "LE", "GE", "EQ", "NE",
    "AND", "OR",
    "STRING", "ID", "INT", "BOOL",
    "STRING_CONST", "INT_CONST",
    "DO", "WHILE", "IF", "ELSE", "THEN", "END",
};

void next_token(){
    token = yylex();
    token_str = copyString(yytext);
    //print_token();
}


void print_token()
{
    // token 的编号原则为：单字符运算符的 token 编号就是其字符的数值，其他类型的 token 则从 256 开始编号。
    if (token < 256)
    {
        printf("%c %s\n", token, token_str);
    }
    else
    {
        printf("%s %s\n", token_strs[token - 256], token_str);
    }
}

char* get_token_str(int token){
    if(token == 0){
        return "EOF";
    }
    else if(token < 256){
        static char s[1];
        s[0] = token;
        return s;
    }else{
        return token_strs[token - 256];
    }
}

TreeNode *newStmtNode(StmtKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        printf("Out of memory error at line %d\n", cur_line_num);
    else
    {
        for (i = 0; i < MAX_CHILDREN_NUMBER; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = cur_line_num;
    }
    return t;
}

TreeNode *newExpNode(ExpKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        printf("Out of memory error at line %d\n", cur_line_num);
    else
    {
        for (i = 0; i < MAX_CHILDREN_NUMBER; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = cur_line_num;
        //t->type = Void;
    }
    return t;
}

void syntaxError(char *message)
{
    printf("\n>>> ");
    printf("Syntax error at line %d: %s", cur_line_num, message);
    //Error = TRUE;
}

static void printSpaces()
{
    size_t i;
    for (i = 0; i < indentno; i++)
        printf(" ");
}

void printGrammerTree(TreeNode *tree)
{
    {
        int i;
        INDENT;
        while (tree != NULL)
        {
            printSpaces();

            if (tree->nodekind == StmtK)
            {
                switch (tree->kind.stmt)
                {
                case IfK:
                    printf("If\n");
                    break;
                case WhileK:
                    printf("While\n");
                    break;
                case DefineK:
                    printf("Define\n");
                    break;
                case StmtSeq:
                    printf("Statements:\n");
                    break;
                default:
                    printf("Unknown StmtKNode kind\n");
                    break;
                }
            }
            else if (tree->nodekind == ExpK)
            {
                switch (tree->kind.exp)
                {
                case OpK:
                    printf("Op: %s\n",get_token_str(tree->attr.op));
                    break;
                case IntK:
                    printf("Const: %d\n", tree->attr.val);
                    break;
                case IdK:
                    printf("Id: %s\n", tree->attr.name);
                    break;
                case StringK:
                    printf("STR: %s\n", tree->attr.name);
                    break;
                case TypeK:
                    printf("%s\n",tree->attr.name);
                    break;
                default:
                    printf("Unknown ExpKNode kind\n");
                    break;
                }
            }
            else
            {
                printf("Unkown node kind\n");
            }
            for (i = 0; i < MAX_CHILDREN_NUMBER; i++)
                printGrammerTree(tree->child[i]);
            tree = tree->sibling;
        }
        UNINDENT;
    }
}

char *copyString(char *s)
{
    int n;
    char *t;
    if (s == NULL)
        return NULL;
    n = strlen(s) + 1;
    t = (char *)malloc(n);
    if (t == NULL)
        printf( "Out of memory error at line %d\n", cur_line_num);
    else
        strcpy(t, s);
    return t;
}

char* int_to_str(int num)
{
    char* str = (char*)malloc(32);
    if(num == 0){
        str[0] = '0';
        str[1] = 0;
        return str;
    }
    int i = 0;
    while(num != 0){
        str[i] ='0'+num%10;
        i++;
        num/=10;
    }

    str[i] = 0;

    // 逆序
    int len = i;
    for(int j = 0;j<len/2;j++){
        char tmp = str[j];
        str[j] = str[len - j - 1];
        str[len - j - 1] = tmp;
    }
    return str;//返回转换后的字符串
}

int str_to_int(char* str){
    int res = 0;
    size_t i = 0;
    while(i < 32 && str[i]!=0){
        res *= 10;
        res += (str[i] - '0');
        i++;
    }
    return res;
}
