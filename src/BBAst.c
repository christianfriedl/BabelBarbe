#include<cgenerics/CGTree.h>
#include"BB.h"
#include"BBAst.h"

DEFINE_TREE_FUNCS(BBAst)
DECLARE_ARRAY_ITERATOR(BBAst)
DEFINE_ARRAY_ITERATOR(BBAst)

BBAst* BBAst__new(BBAst* parent, BBToken* token, BBSentence* sentence) {
    BBAst* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->tree = CGTree__new(BBAst, this);
        if (parent != NULL)
            CGTree_addSubTree(BBAst, parent->tree, this->tree);
        this->token = token;
        this->sentence = sentence;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, CGExceptionID_CannotAllocate, "Cannot allocate BBAst in %s", __func__);
    return this;
}
void BBAst_delete(BBAst* this) {
    CGAppState_THROW(CGAppState__getInstance(), Severity_notice, CGExceptionID_GeneralNonfatalException, "TODO: delete the subtrees of the Ast in %s!", __func__);
    CGTree_delete(BBAst, this->tree);
    BBToken_delete(this->token);
    /* BBSentence_delete(this->sentence); --> NO, the Ast should not delete the sentence, since the ref is only informational */
}
BBAst* BBAst_clone(BBAst* this) {
    return BBAst__new(BBAst_getParent(this), this->token, this->sentence);
}

BBToken* BBAst_getToken(BBAst* this) {
    return this->token;
}

BBAst* BBAst_getParent(BBAst* this) {
    return CGTree_getValue(BBAst, CGTree_getParent(BBAst, this->tree));
}

void BBAst_setParent(BBAst* this, BBAst* parent) {
    CGTree_setParent(BBAst, this->tree, parent->tree);
}

CGTree(BBAst)* BBAst_getTree(BBAst* this) {
    return this->tree;
}

BBSentence* BBAst_getSentence(BBAst* this) {
    return this->sentence;
}

void BBAst_setSubAsts(BBAst* this, CGArray(BBAst)* subAsts) {
    CGArrayIterator(BBAst)* iter = CGArrayIterator__new(BBAst, subAsts);
    BBAst* ast = NULL;
    while ((ast = CGArrayIterator_fetch(BBAst, iter)) != NULL)
        CGTree_addSubTree(BBAst, this->tree, ast->tree);
}
CGArrayOfCGTreeOfBBAstIterator* BBAst_getSubAstIterator(BBAst* this) {
    CGArrayOfCGTreeOfBBAst* subTrees = CGTree_getSubTrees(BBAst, this->tree);
    CGArrayOfCGTreeOfBBAstIterator* iter = CGArrayIterator__new(CGTreeOfBBAst, subTrees);
    return iter;
}
BBAst* BBAst_getSubAstAt(BBAst* this, unsigned int index) {
    CGArrayOfCGTreeOfBBAst* subTrees = CGTree_getSubTrees(BBAst, this->tree);
    CGTreeOfBBAst* subAstTree = CGArrayOfCGTreeOfBBAst_getValueAt(subTrees, index);
    BBAst* subAst = CGTree_getValue(BBAst, subAstTree);
    return subAst;
}

unsigned int BBAst_getSubAstsSize(BBAst* this) {
    CGArrayOfCGTreeOfBBAst* subTrees = CGTree_getSubTrees(BBAst, this->tree);
	return CGArray_getSize(CGTreeOfBBAst, subTrees);
}
CGArrayOfCGTreeOfBBAst* BBAst_getSubAsts(BBAst* this) {
    return CGTree_getSubTrees(BBAst, this->tree);
}

void BBAst_print(BBAst* this, unsigned int indentationLevel) {
    CGString* indentation = CGString__newFromLengthAndPreset(indentationLevel * BB_INDENTATION_SIZE, ' ');
    CGString* tokenString = BBToken_toString(this->token);
    CGString* sentenceName = BBSentence_getName(this->sentence);
    printf("%sBBAst (token %s; sentence %s)\n", indentation, tokenString, sentenceName);
    CGArrayOfCGTreeOfBBAst* subTrees = CGTree_getSubTrees(BBAst, this->tree);
    CGArrayOfCGTreeOfBBAstIterator* iter = CGArrayIterator__new(CGTreeOfBBAst, subTrees);
    CGTree(BBAst)* astTree = NULL;
    while ((astTree = CGArrayIterator_fetch(CGTreeOfBBAst, iter)) != NULL)
        BBAst_print(CGTree_getValue(BBAst, astTree), indentationLevel + 1);
    CGArrayOfCGTreeOfBBAstIterator_delete(iter);
    CGString_delete(tokenString);
    CGString_delete(indentation);
}


