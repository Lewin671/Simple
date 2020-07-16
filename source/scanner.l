/* Declarations part */
%{
#ifndef GLOBALS_H
#include "globals.h"
#endif
#include <string.h>

void init_scanner();
void lex_error(char* msg,int line);

//LEX和YACC通过yyin文件指针识别输入源，如果用户未指明输入，默认从键盘获取输入。
%}

/* Definitions, note: \042 is '"' */
INTEGER             ([0-9]+)
UNTERM_STRING       (\042[^\042\n]*)
STRING              (\042[^\042\n]*\042)
IDENTIFIER          ([_a-zA-Z][_a-zA-Z0-9]*)
OPERATOR            ([+*-/%=,;!<>(){}])
SINGLE_COMMENT      ("//"[^\n]*)


/* rules part */
%%
[\n]                { cur_line_num++;                       }
[ \t\r\a]+          { /* ignore all spaces */               }
{SINGLE_COMMENT}   { /* skip for single line comment */    }

{OPERATOR}  {return yytext[0]; /* 单个字符，它的字符值就是token*/}


"<="    {return LE;}
">="    {return GE;}
"=="    {return EQ;}
"!="    {return NE;}
"&&"    {return AND;}
"||"    {return OR;}
"int"   {return INT;}
"string"    {return STRING;}
"bool"  {return BOOL;}
"do"    {return DO;}
"while" {return WHILE;}
"if"    {return IF;}
"else"  {return ELSE;}
"then"  {return THEN;}
"end"   {return END;}

{INTEGER}   {return INT_CONST;}
{STRING}    {return STRING_CONST;}
{IDENTIFIER}    {return ID;}

<<EOF>>             { return 0; }

{UNTERM_STRING} {lex_error("Unterminated string constant",cur_line_num);}

. {
    char msg[50]="Unrecognized character: ";
    strcat(msg,yytext);
    lex_error(msg, cur_line_num);
 }
%%


/*
void init_scanner() {
    printf("%-20s%s\n", "TOKEN-TYPE", "TOKEN-VALUE");
    printf("-------------------------------------------------\n");
}
*/

void lex_error(char* msg, int line) {
    printf("\nError at line %-3d: %s\n\n", line, msg);
}


/*
输入文件中最后一行的 yywrap 函数的作用是将多个输入文件打包成一个输入，当 yylex 函数读入到一个文件结束（EOF）时，它会向 yywrap 函数询问， yywrap 函数返回 1 的意思是告诉 yylex 函数后面没有其他输入文件了，此时 yylex 函数结束，yywrap 函数也可以打开下一个输入文件，再向 yylex 函数返回 0 ，告诉它后面还有别的输入文件，此时 yylex 函数会继续解析下一个输入文件。总之，由于我们不考虑连续解析多个文件，因此此处返回 1
*/
int yywrap(void) {
    return 1;
}
