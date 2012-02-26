#include<cgenerics/CGTree.h>
#include"BNF.h"
#include"BNFAst.h"

DEFINE_TREE_FUNCS(BNFToken)

BNFAst* BNFAst__new(BNFAst* parent, BNFToken* token) {
    BNFAst* this = CGTree__new(BNFToken, token);
    if (parent != NULL)
        CGTree_addSubTree(BNFToken, parent, this);
    return this;
}
void BNFAst_delete(BNFAst* this) {
    CGTree_deleteValues(BNFToken, this);
    CGTree_delete(BNFToken, this);
}

BNFToken* BNFAst_getToken(BNFAst* this) {
    return CGTree_getValue(BNFToken, this);
}
