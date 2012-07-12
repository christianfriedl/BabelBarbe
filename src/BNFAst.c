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
CGArrayOfCGTreeOfBNFAstIterator* BNFAst_getSubAstIterator(BNFAst* this) {
    CGArrayOfCGTreeOfBNFAst* subTrees = CGTree_getSubTrees(BNFAst, this->tree);
    CGArrayOfCGTreeOfBNFAstIterator* iter = CGArrayIterator__new(CGTreeOfBNFAst, subTrees);
    return iter;
}
BNFAst* BNFAst_getSubAstAt(BNFAst* this, unsigned int index) {
    CGArrayOfCGTreeOfBNFAst* subTrees = CGTree_getSubTrees(BNFAst, this->tree);
    CGTreeOfBNFAst* subAstTree = CGArrayOfCGTreeOfBNFAst_getValueAt(subTrees, index);
    BNFAst* subAst = CGTree_getValue(BNFAst, subAstTree);
    return subAst;
}

void BNFAst_print(BNFAst* this, unsigned int indentationLevel) {
    CGString* indentation = CGString__newFromLengthAndPreset(indentationLevel * BNF_INDENTATION_SIZE, ' ');
    CGString* tokenString = BNFToken_toString(this->token);
    CGString* sentenceName = BNFSentence_getName(this->sentence);
    printf("%sBNFAst (token %s; sentence %s)\n", indentation, tokenString, sentenceName);
    CGArrayOfCGTreeOfBNFAst* subTrees = CGTree_getSubTrees(BNFAst, this->tree);
    CGArrayOfCGTreeOfBNFAstIterator* iter = CGArrayIterator__new(CGTreeOfBNFAst, subTrees);
    CGTree(BNFAst)* astTree = NULL;
    while ((astTree = CGArrayIterator_fetch(CGTreeOfBNFAst, iter)) != NULL)
        BNFAst_print(CGTree_getValue(BNFAst, astTree), indentationLevel + 1);
    CGArrayOfCGTreeOfBNFAstIterator_delete(iter);
    CGString_delete(tokenString);
    CGString_delete(indentation);
}


