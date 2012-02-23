#include<string.h>
#include"CGArrayOfBNFScannerNode.h"
#include"BNFScannerRule.h"
DECLARE_ARRAY_FUNCS(BNFScannerNode)
DECLARE_ARRAY_ITERATOR_FUNCS(BNFScannerNode)

static bool BNFScannerNode_applyRegexToText_(CGAppState* appState, BNFScannerNode* this, const char* text);

BNFScannerNode* BNFScannerNode_clone(CGAppState* appState, BNFScannerNode* this) {
    return BNFScannerNode__new(appState, this->type, this->pattern, this->followupRule, this->tokenType);
}

BNFScannerNode* BNFScannerNode__new(CGAppState* appState, BNFScannerNodeType type, CGString* pattern, BNFScannerRule* followupRule, BNFTokenType tokenType) {
    BNFScannerNode* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->type = type;
        this->pattern = strdup(pattern);
        this->followupRule = followupRule;
        this->tokenType = tokenType;
    } else
        CGAppState_throwException(appState, CGException__new(Severity_error, CGExceptionID_CannotAllocate, "Cannot allocate BNFScannerNode"));
    return this;
}

void BNFScannerNode_delete(CGAppState* appState, BNFScannerNode* this) {
    free(this->pattern);
    free(this);
}

BNFTokenType BNFScannerNode_getTokenType(CGAppState* appState, BNFScannerNode* this) {
    return this->tokenType;
}

/**
    @return success or failure
*/
bool BNFScannerNode_applyToText(CGAppState* appState, BNFScannerNode* this, const char* text) {
    if (this->type == BNFScannerNodeType_string)
        return (!strncmp(this->pattern, text, strlen(this->pattern))) ? true : false;
    else
        return BNFScannerNode_applyRegexToText_(appState, this, text);
}

bool BNFScannerNode_applyRegexToText_(CGAppState* appState, BNFScannerNode* this, const char* text) {
    return false;
}
BNFScannerRule* BNFScannerRule_clone(CGAppState* appState, BNFScannerRule* this) {
    return BNFScannerRule__new(appState, this->nodes);
}

BNFScannerRule* BNFScannerRule__new(CGAppState* appState, CGArray(BNFScannerNode)* nodes) {
    BNFScannerRule* this = malloc(sizeof(*this));
    if (this) {
        this->nodes = nodes;
    } else
        CGAppState_throwException(appState, CGException__new(Severity_error, CGExceptionID_CannotAllocate, "Cannot allocate BNFScannerRule"));
    return this;
}

void BNFScannerRule_delete(CGAppState* appState, BNFScannerRule* this) {
    free(this);
}

