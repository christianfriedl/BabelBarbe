#ifndef _BNF_AST_H
#define _BNF_AST_H

#include<cgenerics/CGTree.h>
#include"BNFToken.h"
struct BNFAst_struct;
typedef struct BNFAst_struct BNFAst;
DECLARE_ARRAY(BNFAst)
#include"BNF_RDParser.h"

DECLARE_TREE_TYPE(BNFAst)
struct BNFAst_struct {
    CGTree(BNFAst)* tree;
    BNFToken* token;
    BNFSentence* sentence;
};
DECLARE_TREE_FUNCS(BNFAst)


BNFAst* BNFAst__new(BNFAst* parent, BNFToken* token, BNFSentence* sentence);
BNFAst* BNFAst_clone(BNFAst* this);
void BNFAst_delete(BNFAst* this);
BNFToken* BNFAst_getToken(BNFAst* this);
BNFAst* BNFAst_getParent(BNFAst* this);
void BNFAst_setParent(BNFAst* this, BNFAst* parent);
BNFSentence* BNFAst_getSentence(BNFAst* this);
void BNFAst_setSubAsts(BNFAst* this, CGArray(BNFAst)* subAsts);

#endif

