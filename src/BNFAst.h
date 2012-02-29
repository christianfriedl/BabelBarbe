#ifndef _BNF_AST_H
#define _BNF_AST_H

#include<cgenerics/CGTree.h>
#include"BNFToken.h"
struct BNFAst_struct;
typedef struct BNFAst_struct BNFAst;
#include"BNF_RDParser.h"

DECLARE_TREE_TYPE(BNFAst)
struct BNFAst_struct {
    CGTree(BNFAst)* tree;
    BNFToken* token;
    BNFSentence* sentence;
};
DECLARE_TREE_FUNCS(BNFAst)


BNFAst* BNFAst__new(BNFAst* parent, BNFToken* token, BNFSentence* sentence);
void BNFAst_delete(BNFAst* this);
BNFToken* BNFAst_getToken(BNFAst* this);
BNFAst* BNFAst_getParent(BNFAst* this);
BNFSentence* BNFAst_getSentence(BNFAst* this);

#endif

