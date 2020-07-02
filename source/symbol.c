#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"
#include "globals.h"

// reference from tiny+

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

/* the hash function */
int str_hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* the list of line numbers of the source
 * code in which a variable is referenced
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

/* The record in the bucket lists for
 * each variable, including name,
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec
   { char * name;
     LineList lines;
     int type;
     int memloc ; /* memory location for variable */
     struct BucketListRec * next;
   } * BucketList;

/* the hash table */
static BucketList hashTable[SIZE];

int getHashIndex(char * name)
{
    return  str_hash(name);
}


/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(char * name, int lineno, int loc,int exp_type)
{
    printf("insert %d %s", lineno, name);
    int h = str_hash(name);
    BucketList l = hashTable[h];
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;
    if (l == NULL) /* variable not yet in table */
    {
        printf(" location= %d", loc);
        l = (BucketList)malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lines = (LineList)malloc(sizeof(struct LineListRec));
        l->lines->lineno = lineno;
        l->memloc = loc;
        l->type =exp_type;
        l->lines->next = NULL;
        l->next = hashTable[h];
        hashTable[h] = l;
    }
    else /* found in table, so just add line number */
    {
        LineList t = l->lines;
        while (t->next != NULL) t = t->next;
        t->next = (LineList)malloc(sizeof(struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;
    }
    printf("\n");
} /* st_insert */


/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup_type(char * name)
{
    int h = str_hash(name);
    BucketList l = hashTable[h];
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;
    if (l == NULL) return -1;
    else
        return l->type;
        //return l->memloc;
}
/* Function st_lookup returns the memory
* location of a variable or -1 if not found
*/
int st_lookup(char * name)
{
    int h = str_hash(name);
    BucketList l = hashTable[h];
    int i_tmp = -1;//hash±íÖÐÄ³Ò»ÐÐÖÐÒ»¹²ÓÐi_tmp+1Ïî
    while ((l != NULL) && (strcmp(name, l->name) != 0))
    {
        i_tmp++;
        l = l->next;
    }
    if (l == NULL) return -1;
    else
        return 10*h+i_tmp;//
}

/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */
void printSymTab()
{
    int i;
    printf("Variable Name  Location   Line Numbers\n");
    printf("-------------  --------   ------------\n");
    for (i = 0; i < SIZE; ++i)
    {
        if (hashTable[i] != NULL)
        {
            BucketList l = hashTable[i];
            while (l != NULL)
            {
                LineList t = l->lines;
                printf("%-14s ", l->name);
                printf("%-8d  ", l->memloc);

                while (t != NULL)
                {
                    printf("%4d ", t->lineno);
                    t = t->next;
                }
                printf("\n");
                l = l->next;
            }
        }
    }
} /* printSymTab */
