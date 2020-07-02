#include "parse.h"

#ifndef GLOBALS_H
#include "globals.h"
#endif

#include <stdio.h>

static TreeNode *program();
static TreeNode *stmt_seq();
static TreeNode *statement();
static TreeNode *define_stmt();
static TreeNode *if_stmt();
static TreeNode *assign_stmt();
static TreeNode *while_stmt();

static TreeNode *type();
static TreeNode *id_list();
static TreeNode *bool_exp();
static TreeNode *arithmetic_exp();
static TreeNode *cmp_stmt();
static TreeNode* item1();
static TreeNode *item();

void match(int expected)
{
    if (expected == token)
    {
        next_token();
    }
    else
    {
        syntaxError("unexpected token -> ");
        printf("%s\n",token_str);
    }
}

TreeNode *parse()
{
    next_token();
    return program();
}

// program -> statement;stmt_seq
// 程序至少有一条语句
TreeNode *program()
{
    // 说明不止一条语句
    if (token != 0) // token = 0表示EOF
    {
        return stmt_seq();
    }else{
        syntaxError("");
        printf("%s\n",get_token_str(token));
        return NULL;
    }
}

// stmt_seq -> (statement;)*
TreeNode *stmt_seq()
{
    TreeNode *t = NULL;
    TreeNode *p = NULL;
    while (!(token == 0 || token == ELSE || token == WHILE || token == END))
    {
        TreeNode *q = statement();
        if (t == NULL)
        {
            t = q;
        }
        else
        {
            p->sibling = q;
        }
        p = q;
    }

    TreeNode* res = newStmtNode(StmtSeq);
    res->child[0] = t;
    return res;
}

// statement -> define-stmt | if-stmt | assign-stmt | while-stmt
TreeNode *statement()
{
    TreeNode* t = NULL;
    switch (token)
    {
    case INT:
    case BOOL:
    case STRING:
        t =  define_stmt();
        break;
    case IF:
        t =  if_stmt();
        break;
    case DO:
        t = while_stmt();
        break;
    case ID:
        t = assign_stmt();
        break;
    default:
        syntaxError("");
        printf("%s\n",get_token_str(token));
        if(token == 0){
            return NULL;
        }
    }
    match(';');
    return t;
}

// define-stmt -> type id-list
TreeNode *define_stmt()
{
    TreeNode *t = newStmtNode(DefineK);
    t->child[0] = type();
    t->child[1] = id_list(t->child[0]->type);
    t->type = t->child[0]->type;
    return t;
}

// if-stmt -> if bool-exp then stmt_seq end | if bool-exp then stmt_seq else stmt_seq end
TreeNode *if_stmt()
{
    TreeNode *t = newStmtNode(IfK);
    match(IF);
    t->child[0] = bool_exp();
    match(THEN);
    t->child[1] = stmt_seq();
    if (token == ELSE)
    {
        match(ELSE);
        t->child[2] = stmt_seq();
    }
    match(END);
    return t;
}

TreeNode *assign_stmt()
{
    TreeNode *t = newExpNode(IdK);
    t->attr.name = token_str;
    match(ID);

    TreeNode *p = newExpNode(OpK);
    p->attr.op = '=';
    match('=');

    p->child[0] = t;

    if (token == STRING_CONST)
    {
        TreeNode *s = NULL;
        s = newExpNode(StringK);
        s->attr.name = token_str;
        s->type = String; // 设置结点的数据类型
        p->child[1] = s;
        match(STRING_CONST);
    }
    else
    {
        p->child[1] = arithmetic_exp();
    }
    return p;
}

// while-stmt -> do stmt_seq while bool-exp
TreeNode *while_stmt()
{
    TreeNode *t = newStmtNode(WhileK);
    match(DO);
    t->child[0] = stmt_seq();
    match(WHILE);
    t->child[1] = bool_exp();
    return t;
}

// type-> int | bool | string
TreeNode *type()
{
    TreeNode *t = NULL;
    switch (token)
    {
    case INT:
        t = newExpNode(TypeK);
        t->attr.name = "int";
        t->type = Integer;
        match(INT);
        break;
    case STRING:
        t = newExpNode(TypeK);
        t->attr.name = "string";
        t->type = String;
        match(STRING);
        break;
    case BOOL:
        t = newExpNode(TypeK);
        t->attr.name = "bool";
        t->type = Boolean;
        match(BOOL);
        break;
    default:
        syntaxError("");
        break;
    }
    return t;
}

// id-list -> (id)  (,id )*
TreeNode *id_list(ExpType type)
{
    if (token == ID)
    {
        TreeNode *t = newExpNode(IdK);
        TreeNode* q = t;
        t->attr.name = token_str;
        t->type = type;
        match(ID);

        while(token == ',')
        {
            match(',');
            TreeNode* p = newExpNode(IdK);
            p->attr.name = token_str;
            p->type = type;
            q->sibling = p;
            q = p;
            match(ID);
        }
        return t;
    }
    else
    {
        syntaxError("id list has no id");
        return NULL;
    }
}

// bool-exp -> cmp-stmt ([or|and|not] cmp-stmt)
TreeNode *bool_exp()
{
    TreeNode *t = cmp_stmt();
    t->type = Boolean;
    if (token == OR || token == AND || token == '!')
    {
        TreeNode *p = newExpNode(OpK);
        p->attr.op = token;
        p->child[0] = t;
        p->type = Boolean;
        t = p;

        match(token);

        t->child[1] = bool_exp();
    }
    return t;
}

//arithmetic-exp -> item1 ((+|-|) item1)*
TreeNode *arithmetic_exp()
{
    TreeNode *t = item1();
    while (token == '+' || token == '-')
    {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL)
        {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = item1();
        }
    }
    return t;
}

// cmp-stmt -> arithmetic_exp (> | < | >= | <= | == | !=) arithmetic_exp
static TreeNode *cmp_stmt()
{
    TreeNode *t = arithmetic_exp();
    if (token == GE || token == LE || token == '>' ||
            token == '<' || token == EQ || token == NE)
    {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL)
        {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = arithmetic_exp();
        }
    }
    return t;
}

// 处理乘除
static TreeNode* item1(){
    TreeNode *t = item();
    while (token == '*' || token == '/')
    {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL)
        {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = item();
        }
    }
    return t;
}

// item -> id | num
static TreeNode *item()
{
    TreeNode *t = NULL;
    switch (token)
    {
    case ID:
        t = newExpNode(IdK);
        t->attr.name = token_str;
        match(ID);
        break;
    case INT_CONST:
        t = newExpNode(IntK);
        t->attr.val = atoi(token_str);
        t->type = Integer;
        match(INT_CONST);
        break;
    default:
        syntaxError("unexpected token -> ");
        print_token();
        break;
    }
    return t;
}
