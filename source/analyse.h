#ifndef ANALYSE_H
#define ANALYSE_H

#ifndef GLOBALS_H
#include "globals.h"
#endif

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode *);

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode *);

#endif // ANALYSE_H
