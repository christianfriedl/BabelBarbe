#include<string.h>
#include"CGArrayOfBNFScannerNode.h"
#include"BNFException.h"
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
        if (type == BNFScannerNodeType_regex) {
            this->pattern = NULL;
            this->regex = NULL;
            BNFScannerNode_setRegex(appState, this, pattern);
            if (CGAppState_isExceptionRaised(appState)) {
                free(this);
                return NULL;
            }
        } else {
            this->regex = NULL;
            this->pattern = strdup(pattern);
        }
        this->followupRule = followupRule;
        this->tokenType = tokenType;
    } else
        CGAppState_throwException(appState, CGException__new(Severity_error, CGExceptionID_CannotAllocate, "Cannot allocate BNFScannerNode"));
    return this;
}

bool BNFScannerNode_setRegex(CGAppState* appState, BNFScannerNode* this, CGString* pattern) {
    CGString errorString[255];
    int errorOffset = 0;
    bool success = false;

    if (this->pattern != NULL) free(this->pattern);
    if (this->regex != NULL) free(this->regex);
    this->pattern = strdup(pattern);
    this->regex = pcre_compile(this->pattern, PCRE_ANCHORED, (const char**)&errorString, &errorOffset, NULL); /* TODO optimization: compile the pattern once and store it */
    if (this->regex != NULL)
        success = true;
    else {
        CGAppState_throwException(appState, CGException__new(Severity_error, BNFExceptionID_PCRERegexError, "PCRE reported a compilation error at offset %i, message: '%s' (pattern '%s')", errorOffset, errorString, this->pattern));
        free(this->pattern);
        this->pattern = NULL;
        success = false;
    }
    return success;
}

void BNFScannerNode_delete(CGAppState* appState, BNFScannerNode* this) {
    free(this->pattern);
    if (this->type == BNFScannerNodeType_regex)
        free(this->regex);
    free(this);
}

BNFTokenType BNFScannerNode_getTokenType(CGAppState* appState, BNFScannerNode* this) {
    return this->tokenType;
}

/**
    @return success or failure
*/
bool BNFScannerNode_applyToText(CGAppState* appState, BNFScannerNode* this, const CGString* text) {
    if (this->type == BNFScannerNodeType_string)
        return (!strncmp(this->pattern, text, strlen(this->pattern))) ? true : false;
    else
        return BNFScannerNode_applyRegexToText_(appState, this, text);
}

/**
    @return whether the regex matched
*/
bool BNFScannerNode_applyRegexToText_(CGAppState* appState, BNFScannerNode* this, const CGString* text) {
    CGString* errorString = CGString__newFromLengthAndPreset(appState, 255, ' ');
    int errorOffset = 0;
    int outputVector[3];
    int reResult = 0;
    
    reResult = pcre_exec(this->regex, NULL, text, strlen(text), 0, PCRE_ANCHORED, outputVector, 3);
    if (reResult < 0) { /* not found, but also possible error */
        switch (reResult) {
            case PCRE_ERROR_NOMATCH:
                return false;
                break;
            default:
                CGAppState_throwException(appState, CGException__new(Severity_error, BNFExceptionID_PCRERegexError, "PCRE reported an error, message '%s' at offset %i, resultcode was %i (pattern '%s', text '%s')", errorString, errorOffset, reResult, this->pattern, text));
                return false;
        }
    } else if (reResult == 0) {
        CGAppState_throwException(appState, CGException__new(Severity_error, BNFExceptionID_PCRERegexError, "PCRE cannot write into outputVector because it is too small (pattern '%s', text '%s')", this->pattern, text));
        return false;
    } else
        return true;

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

BNFScannerNode* BNFScannerRule_applyToText(CGAppState* appState, BNFScannerRule* this, const CGString* text) {
    CGArrayIterator(BNFScannerNode)* iter = CGArrayIterator__new(appState, BNFScannerNode, this->nodes);
    while (CGArrayIterator_isInsideBounds(appState, BNFScannerNode, iter)) {
        BNFScannerNode* node = CGArrayIterator_getCurrentElement(appState, BNFScannerNode, iter);
        if (BNFScannerNode_applyToText(appState, node, text) == true)
            return node;
        CGArrayIterator_moveToNextElement(appState, BNFScannerNode, iter);
    }
    return NULL;
}

