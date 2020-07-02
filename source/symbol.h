#ifndef SYMBOL_H
#define SYMBOL_H

// 符号表，用哈希表存储

// 如果在符号表中找到，则返回位置
// 否则返回-1
int st_lookup(char* name);
int st_lookup_type(char *name);

int str_hash ( char * key );

/* Procedure st_insert inserts line numbers and
* memory locations into the symbol table
* loc = memory location is inserted only the
* first time, otherwise ignored
*/
void st_insert(char * name, int lineno, int loc, int exp_type);

void printSymTab();
#endif // SYMBOL_H
