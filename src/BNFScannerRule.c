#include"CGArrayOfBNFScannerRule.h"
#include"BNFScannerRule.h"
DECLARE_ARRAY_FUNCS(BNFScannerRule)

BNFScannerRule* BNFScannerRule_clone(CGAppState* appState, BNFScannerRule* this) {
    return BNFScannerRule__new(appState, this->type, this->pattern, this->followupRules);
}

BNFScannerRule* BNFScannerRule__new(CGAppState* appState, BNFScannerRuleType type, CGString* pattern, CGArray(BNFScannerRule)* followupRules) {
    BNFScannerRule* this = malloc(sizeof(*this));
    if (this) {
        this->type = type;
        this->pattern = pattern;
        this->followupRules = followupRules;
    } else
        CGAppState_throwException(appState, CGException__new(Severity_error, CGExceptionID_CannotAllocate, "Cannot allocate BNFScannerRule"));
    return this;
}

void BNFScannerRule_delete(CGAppState* appState, BNFScannerRule* this) {
    free(this);
}
