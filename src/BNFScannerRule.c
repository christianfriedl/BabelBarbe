#include<string.h>
#include"BNFException.h"
#include"BNFScannerRule.h"
DECLARE_ARRAY_FUNCS(BNFScannerNode)
DEFINE_ARRAY_FUNCS(BNFScannerNode)
DECLARE_ARRAY_ITERATOR_FUNCS(BNFScannerNode)
DEFINE_ARRAY_ITERATOR_FUNCS(BNFScannerNode)

typedef struct {
    unsigned int len;
    bool success;
} ApplyToTextRV_;

static ApplyToTextRV_* BNFScannerNode_applyStringToText_(BNFScannerNode* this, const CGString* text);
static ApplyToTextRV_* BNFScannerNode_applyRegexToText_(BNFScannerNode* this, const CGString* text);
static BNFToken* BNFScannerNode_applyFunctionToText_(BNFScannerNode* this, const CGString* text, ApplyToTextRV_*(*func)(BNFScannerNode*, const CGString*));

BNFScannerNode* BNFScannerNode__new(BNFScannerNodeType type, CGString* pattern, BNFScannerRule* followupRule, BNFTokenType tokenType, bool isNoise) {
    BNFScannerNode* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->type = type;
        if (type == BNFScannerNodeType_regex) {
            this->pattern = NULL;
            this->regex = NULL;
            CGAppState_catchAndDeleteException(CGAppState__getInstance());
            BNFScannerNode_setRegex(this, pattern);
            if (CGAppState_isExceptionRaised(CGAppState__getInstance())) {
                free(this);
                return NULL;
            }
        } else {
            this->regex = NULL;
            this->pattern = strdup(pattern);
        }
        this->followupRule = followupRule;
        this->tokenType = tokenType;
        this->isNoise = isNoise;
    } else
        CGAppState_throwException(CGAppState__getInstance(), CGException__new(Severity_fatal, CGExceptionID_CannotAllocate, "Cannot allocate BNFScannerNode"));
    return this;
}

BNFScannerNode* BNFScannerNode_clone(BNFScannerNode* this) {
    return BNFScannerNode__new(this->type, this->pattern, this->followupRule, this->tokenType, this->isNoise);
}

bool BNFScannerNode_setRegex(BNFScannerNode* this, CGString* pattern) {
    const char *errorString;
    int errorOffset = 0;
    bool success = false;

    if (this->pattern != NULL) free(this->pattern);
    if (this->regex != NULL) free(this->regex);
    this->pattern = strdup(pattern);
    this->regex = pcre_compile(this->pattern, PCRE_ANCHORED, &errorString, &errorOffset, NULL); 
    if (this->regex != NULL)
        success = true;
    else {
        CGAppState_throwException(CGAppState__getInstance(), CGException__new(Severity_error, BNFExceptionID_PCRERegexError, "PCRE reported a compilation error at offset %i, message: '%s' (pattern '%s')", errorOffset, errorString, this->pattern));
        free(this->pattern);
        this->pattern = NULL;
        success = false;
    }
    return success;
}

void BNFScannerNode_delete(BNFScannerNode* this) {
    free(this->pattern);
    if (this->type == BNFScannerNodeType_regex)
        free(this->regex);
    free(this);
}

BNFTokenType BNFScannerNode_getTokenType(BNFScannerNode* this) {
    return this->tokenType;
}
bool BNFScannerNode_getIsNoise(BNFScannerNode* this) {
    return this->isNoise;
}

static BNFToken* BNFScannerNode_createToken_(BNFScannerNode* this, const CGString* text, const unsigned int len) {
    if (this->isNoise) {
        BNFToken* token = BNFToken__new(this->tokenType, CGString__new(""));
        BNFToken_setTextLength(token, len);
        return token;
    } else
        return BNFToken__new(this->tokenType, CGString_createSubstring(text, 0, len));
}

static ApplyToTextRV_* ApplyToTextRV__new(unsigned int len, bool success) {
    ApplyToTextRV_* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->len = len;
        this->success = success;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, CGExceptionID_CannotAllocate, "Cannot allocate ApplyToTextRV_");
    return this;
}

static void ApplyToTextRV_delete(ApplyToTextRV_* this) {
    free(this);
}

static BNFToken* BNFScannerNode_applyFunctionToText_(BNFScannerNode* this, const CGString* text, ApplyToTextRV_*(*func)(BNFScannerNode*, const CGString*)) {
    ApplyToTextRV_* funcRv = (*func)(this, text);
    BNFToken* token = NULL;

    if (funcRv->success == true)
        token = BNFScannerNode_createToken_(this, text, funcRv->len); 
    else
        token = NULL;
    ApplyToTextRV_delete(funcRv);
    return token;
}
/**
    @return success or failure
*/
BNFToken* BNFScannerNode_applyToText(BNFScannerNode* this, const CGString* text) {
    if (this->type == BNFScannerNodeType_string) 
        return BNFScannerNode_applyFunctionToText_(this, text, BNFScannerNode_applyStringToText_);
    else
        return BNFScannerNode_applyFunctionToText_(this, text, BNFScannerNode_applyRegexToText_);
}

static ApplyToTextRV_* BNFScannerNode_applyStringToText_(BNFScannerNode* this, const CGString* text) {
    if (!strncmp(this->pattern, text, strlen(this->pattern)))
        return ApplyToTextRV__new(strlen(this->pattern), true);
    else
        return ApplyToTextRV__new(0, false);
}

/**
    @return whether the regex matched
*/
static ApplyToTextRV_* BNFScannerNode_applyRegexToText_(BNFScannerNode* this, const CGString* text) {
    CGString* errorString = CGString__newFromLengthAndPreset(255, ' ');
    int errorOffset = 0;
    int outputVector[3];
    int reResult = 0;
    
    reResult = pcre_exec(this->regex, NULL, text, strlen(text), 0, PCRE_ANCHORED, outputVector, 3);
    if (reResult < 0) { /* not found, but also possible error */
        switch (reResult) {
            case PCRE_ERROR_NOMATCH:
                return ApplyToTextRV__new(0, false);
                break;
            default:
                CGAppState_throwException(CGAppState__getInstance(), CGException__new(Severity_error, BNFExceptionID_PCRERegexError, "PCRE reported an error, message '%s' at offset %i, resultcode was %i (pattern '%s', text '%s')", errorString, errorOffset, reResult, this->pattern, text));
                return ApplyToTextRV__new(0, false);
        }
    } else if (reResult == 0) {
        CGAppState_throwException(CGAppState__getInstance(), CGException__new(Severity_error, BNFExceptionID_PCRERegexError, "PCRE cannot write into outputVector because it is too small (pattern '%s', text '%s')", this->pattern, text));
        return ApplyToTextRV__new(0, false);
    } else
        return ApplyToTextRV__new(outputVector[1] - outputVector[0], true);
}

void BNFScannerNode_setFollowupRule(BNFScannerNode* this, BNFScannerRule* rule) {
	this->followupRule = rule;
}
BNFScannerRule* BNFScannerNode_getFollowupRule(BNFScannerNode* this) {
    return this->followupRule;
}


BNFScannerRule* BNFScannerRule_clone(BNFScannerRule* this) {
    return BNFScannerRule__new(this->nodes);
}

BNFScannerRule* BNFScannerRule__new(CGArray(BNFScannerNode)* nodes) {
    BNFScannerRule* this = malloc(sizeof(*this));
    if (this) {
        this->nodes = nodes;
        this->successNode = NULL;
    } else
        CGAppState_throwException(CGAppState__getInstance(), CGException__new(Severity_error, CGExceptionID_CannotAllocate, "Cannot allocate BNFScannerRule"));
    return this;
}

void BNFScannerRule_delete(BNFScannerRule* this) {
    free(this);
}

BNFToken* BNFScannerRule_applyToText(BNFScannerRule* this, const CGString* text) {
    CGArrayIterator(BNFScannerNode)* iter = CGArrayIterator__new(BNFScannerNode, this->nodes);
    BNFScannerNode* node = NULL;
    while ((node = CGArrayIterator_fetch(BNFScannerNode, iter)) != NULL) {
        this->successNode = node;
        BNFToken* token = BNFScannerNode_applyToText(this->successNode, text);
        if (token != NULL)
            return token;
    }
    this->successNode = NULL;
    return NULL;
}

BNFScannerNode* BNFScannerRule_getSuccessNode(BNFScannerRule* this) {
    return this->successNode;
}

void BNFScannerRule_setNodes(BNFScannerRule* this, CGArray(BNFScannerNode)* nodes) {
    this->nodes = nodes;
}
