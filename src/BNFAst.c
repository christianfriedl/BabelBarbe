#include<cgenerics/CGTree.h>
#include"BNF.h"
#include"BNFAst.h"

DEFINE_TREE_FUNCS(BNFAst)
DECLARE_ARRAY_ITERATOR(BNFAst)
DEFINE_ARRAY_ITERATOR(BNFAst)

BNFAst* BNFAst__new(BNFAst* parent, BNFToken* token, BNFSentence* sentence) {
    BNFAst* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->tree = CGTree__new(BNFAst, this);
        if (parent != NULL)
            CGTree_addSubTree(BNFAst, parent->tree, this->tree);
        this->token = token;
        this->sentence = sentence;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, CGExceptionID_CannotAllocate, "Cannot allocate BNFAst in %s", __func__);
    return this;
}
void BNFAst_delete(BNFAst* this) {
    CGAppState_THROW(CGAppState__getInstance(), Severity_notice, CGExceptionID_GeneralNonfatalException, "TODO: delete the subtrees of the Ast in %s!", __func__);
    CGTree_delete(BNFAst, this->tree);
    BNFToken_delete(this->token);
    /* BNFSentence_delete(this->sentence); --> NO, the Ast should not delete the sentence, since the ref is only informational */
}
BNFAst* BNFAst_clone(BNFAst* this) {
    return BNFAst__new(BNFAst_getParent(this), this->token, this->sentence);
}

BNFToken* BNFAst_getToken(BNFAst* this) {
    return this->token;
}

BNFAst* BNFAst_getParent(BNFAst* this) {
    return CGTree_getValue(BNFAst, CGTree_getParent(BNFAst, this->tree));
}

void BNFAst_setParent(BNFAst* this, BNFAst* parent) {
    CGTree_setParent(BNFAst, this->tree, parent->tree);
}

BNFSentence* BNFAst_getSentence(BNFAst* this) {
    return this->sentence;
}

void BNFAst_setSubAsts(BNFAst* this, CGArray(BNFAst)* subAsts) {
    CGArrayIterator(BNFAst)* iter = CGArrayIterator__new(BNFAst, subAsts);
    BNFAst* ast = NULL;
    while ((ast = CGArrayIterator_fetch(BNFAst, iter)) != NULL)
        CGTree_addSubTree(BNFAst, this->tree, ast->tree);
}


