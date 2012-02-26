#ifndef _BNF_AST_H
#define _BNF_AST_H

#include<cgenerics/CGTree.h>
#include"BNFToken.h"

/* **** PREMARK **** */
DECLARE_TREE_TYPE(BNFToken)
/* **** MARK **** */
DECLARE_TREE_FUNCS(BNFToken)
/* **** ENDMARK **** */

typedef CGTree(BNFToken) BNFAst;

BNFAst* BNFAst__new(BNFAst* parent, BNFToken* token);
void BNFAst_delete(BNFAst* this);
BNFToken* BNFAst_getToken(BNFAst* this);
void BNFAst_print(BNFAst* this);

#endif

