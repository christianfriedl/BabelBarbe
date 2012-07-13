#include"BNFCodeGenerator.h"

DEFINE_ARRAY_FUNCS(CGString)

BNFCodeGenerator* BNFCodeGenerator__new(BNFAst* ast) {
    BNFCodeGenerator* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->ast = ast;
        this->identifiers = CGArray__new(CGString, 16);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, CGExceptionID_CannotAllocate, "unable to allocate BNFCodeGenerator");
    return this;
}

void BNFCodeGenerator_delete(BNFCodeGenerator* this) {
    CGArray_deleteValues(CGString, this->identifiers);
    CGArray_delete(CGString, this->identifiers);
    free(this);
    /* TODO ast?? */
}


static void BNFCodeGenerator_handleRuleSentence_(BNFCodeGenerator* this, BNFAst* ast) {
    BNFAst* identifierAst = BNFAst_getSubAstAt(ast, 0);
    if (BNFAst_getSentence(identifierAst) == identifierSentence)
        CGArray_push(CGString, this->identifiers, BNFToken_getText(BNFAst_getToken(identifierAst)));
    else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "Identifier expected");
}
static void BNFCodeGenerator_handleStartSentence_(BNFCodeGenerator* this, BNFAst* ast) {
    CGTree(BNFAst)* astTree = NULL;
    CGArrayOfCGTreeOfBNFAstIterator* iter = BNFAst_getSubAstIterator(ast);

    while ((astTree = CGArrayIterator_fetch(CGTreeOfBNFAst, iter)) != NULL)
        BNFCodeGenerator_handleRuleSentence_(this, CGTree_getValue(BNFAst, astTree));
    CGArrayOfCGTreeOfBNFAstIterator_delete(iter);
}


static bool BNFCodeGenerator_handleSentence_(const BNFAst* ast, void* userData) {
    char *sentenceName, *tokenTypeName, *tokenText;
    if (BNFAst_getSentence(ast) && BNFSentence_getName(BNFAst_getSentence(ast)))
        sentenceName = BNFSentence_getName(BNFAst_getSentence(ast));
    else
        sentenceName = "(null)";
    if (BNFAst_getToken(ast)) {
        tokenTypeName = BNFToken_getTypeName(BNFAst_getToken(ast));
        tokenText = BNFToken_getText(BNFAst_getToken(ast));
    } else {
        tokenTypeName = tokenText = "(null)";
    }
    printf("handling sentence: %s %s %s\n", sentenceName, tokenTypeName, tokenText);
}
CGString* BNFCodeGenerator_createCode(BNFCodeGenerator* this) {
    CGTree_mapConstant(BNFAst, this->ast->tree, BNFCodeGenerator_handleSentence_, CGTreeStrategy_breadthFirst, NULL);
    return;
    if (BNFAst_getSentence(this->ast) == startSentence)
        BNFCodeGenerator_handleStartSentence_(this, this->ast);
    else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "No start sentence found.");
    printf("identifiers:");
    CGArray_print(CGString, this->identifiers, "%s");
    return "";
}
