#include"CGArrayOfBNFScannerNode.h"
#include"BNFScannerRule.h"
DECLARE_ARRAY_FUNCS(BNFScannerNode)

BNFScannerNode* BNFScannerNode_clone(CGAppState* appState, BNFScannerNode* this) {
    return BNFScannerNode__new(appState, this->type, this->pattern, this->followupRule, this->token);
}

BNFScannerNode* BNFScannerNode__new(CGAppState* appState, BNFScannerNodeType type, CGString* pattern, BNFScannerRule* followupRule, BNFToken* token) {
    BNFScannerNode* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->type = type;
        this->pattern = pattern;
        this->followupRule = followupRule;
        this->token = token;
    } else
        CGAppState_throwException(appState, CGException__new(Severity_error, CGExceptionID_CannotAllocate, "Cannot allocate BNFScannerNode"));
    return this;
}

void BNFScannerNode_delete(CGAppState* appState, BNFScannerNode* this) {
    free(this);
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
