#ifndef _BB_AST_H
#define _BB_AST_H

#include<cgenerics/CGTree.h>
#include"BBToken.h"
struct BBAst_struct;
typedef struct BBAst_struct BBAst;
DECLARE_ARRAY(BBAst)
#include"BB_RDParser.h"

DECLARE_TREE_TYPE(BBAst)
struct BBAst_struct {
    CGTree(BBAst)* tree;
    BBToken* token;
    BBSentence* sentence;
};
DECLARE_TREE_FUNCS(BBAst)


BBAst* BBAst__new(BBAst* parent, BBToken* token, BBSentence* sentence);
BBAst* BBAst_clone(BBAst* this);
void BBAst_delete(BBAst* this);
CGTree(BBAst)* BBAst_getTree(BBAst* this);
BBToken* BBAst_getToken(BBAst* this);
BBAst* BBAst_getParent(BBAst* this);
void BBAst_setParent(BBAst* this, BBAst* parent);
BBSentence* BBAst_getSentence(BBAst* this);
void BBAst_setSubAsts(BBAst* this, CGArray(BBAst)* subAsts);
void BBAst_print(BBAst* this, unsigned int indentationLevel);
CGArrayOfCGTreeOfBBAstIterator* BBAst_getSubAstIterator(BBAst* this);
BBAst* BBAst_getSubAstAt(BBAst* this, unsigned int index);
unsigned int BBAst_getSubAstsSize(BBAst* this);
CGArrayOfCGTreeOfBBAst* BBAst_getSubAsts(BBAst* this);

#endif
