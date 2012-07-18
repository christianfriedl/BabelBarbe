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

BNFScannerNode* BNFScannerNode__new(BNFScannerNodeType type, CGString* pattern, BNFScannerRule* followupRule, 
        BNFTokenType* tokenType, bool isNoise, BNFToken* (*onAfterScanToken)(BNFToken*)) {
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
        this->onAfterScanToken = onAfterScanToken;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, CGExceptionID_CannotAllocate, "Cannot allocate BNFScannerNode");
    return this;
}

BNFScannerNode* BNFScannerNode_clone(BNFScannerNode* this) {
    return BNFScannerNode__new(this->type, this->pattern, this->followupRule, this->tokenType, this->isNoise, this->onAfterScanToken);
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
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, BNFExceptionID_PCRERegexError, "PCRE reported a compilation error at offset %i, message: '%s' (pattern '%s')", errorOffset, errorString, this->pattern);
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

BNFTokenType* BNFScannerNode_getTokenType(BNFScannerNode* this) {
    return this->tokenType;
}
bool BNFScannerNode_getIsNoise(BNFScannerNode* this) {
    return this->isNoise;
}

static BNFToken* BNFScannerNode_createToken_(BNFScannerNode* this, const CGString* text, const unsigned int len) {
    BNFToken* token  = NULL;
    if (this->isNoise) {
        token = BNFToken__new(this->tokenType, CGString__new(""));
        BNFToken_setTextLength(token, len);
        return token;
    } else
        return BNFToken__new(this->tokenType, CGString_createSubstring(text, 0, len));
}
BNFToken* BNFScannerNode_runOnAfterScanToken(BNFScannerNode* this, BNFToken* token) {
    if (this->onAfterScanToken != NULL) {
        BNFToken* token2 = this->onAfterScanToken(token);
        BNFToken_delete(token);
        return token2;
    } else
        return token;
}

static ApplyToTextRV_* ApplyToTextRV__new(unsigned int len, bool success) {
    ApplyToTextRV_* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->len = len;
        this->success = success;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, CGExceptionID_CannotAllocate, "Cannot allocate ApplyToTextRV_");
    return this;
}

static void ApplyToTextRV_delete(ApplyToTextRV_* this) {
    free(this);
}

CGString* BNFScannerNode_createCDeclaration(BNFScannerNode* this, unsigned int index) {
    CGString* text = CGString__newWithSprintf("BNFScannerNode* scannerNode%u = NULL;\n", index);
    return text;
}
CGString* BNFScannerNode_createCConstructor(BNFScannerNode* this, unsigned int index) {
    CGString* text = CGString__newWithSprintf("scannerNode%u = BNFScannerNode__new(BNFScannerNodeType_%s, \"%s\", NULL, BNFTokenType_%s, %s, NULL);\n",
            index, (this->type == BNFScannerNodeType_regex ? "regex" : "string"),
                this->pattern, BNFTokenType_getName(this->tokenType), (this->isNoise == true ? "true" : "false"));
    return text;
}

static BNFToken* BNFScannerNode_applyFunctionToText_(BNFScannerNode* this, const CGString* text, ApplyToTextRV_*(*func)(BNFScannerNode*, const CGString*)) {
    ApplyToTextRV_* funcRv = (*func)(this, text);
    BNFToken* token = NULL;

    if (funcRv->success == true)
        token = BNFScannerNode_createToken_(this, text, funcRv->len); 
    else
        token = NULL;
    #ifdef DEBUG
    if (funcRv->success == true)
        printf("ApplyFunctionTotext_ success: %i len: %u; token: type %s, text %s, textlen %u\n", funcRv->success, funcRv->len, BNFToken_getTypeName(token), BNFToken_getText(token), BNFToken_getTextLength(token));
    #endif
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
    int outputVector[BNFScannerNode__PCRE_OVECTOR_COUNT]; /* this is a hardcoded number, which is a severe limitation */
    int reResult = 0;
    
    reResult = pcre_exec(this->regex, NULL, text, strlen(text), 0, PCRE_ANCHORED, outputVector, BNFScannerNode__PCRE_OVECTOR_COUNT);
    if (reResult < 0) { /* not found, but also possible error */
        switch (reResult) {
            case PCRE_ERROR_NOMATCH:
                return ApplyToTextRV__new(0, false);
                break;
            default:
                CGAppState_THROW(CGAppState__getInstance(), Severity_error, BNFExceptionID_PCRERegexError, "PCRE reported an error, message '%s' at offset %i, resultcode was %i (pattern '%s', text '%s')", errorString, errorOffset, reResult, this->pattern, text);
                return ApplyToTextRV__new(0, false);
        }
    } else if (reResult == 0) {
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, BNFExceptionID_PCRERegexError, "PCRE cannot write into outputVector because it is too small (pattern '%s', text '%s' ---- maximum number of sub-parts for regex is %i)", this->pattern, text, BNFScannerNode__PCRE_OVECTOR_COUNT / 3);
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
    return BNFScannerRule__new(CGString__new(""), this->nodes);
}

BNFScannerRule* BNFScannerRule__new(CGString* name, CGArray(BNFScannerNode)* nodes) {
    BNFScannerRule* this = malloc(sizeof(*this));
    if (this) {
        this->name = name;
        this->nodes = nodes;
        this->successNode = NULL;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, CGExceptionID_CannotAllocate, "Cannot allocate BNFScannerRule");
    return this;
}

void BNFScannerRule_delete(BNFScannerRule* this) {
    CGString_delete(this->name);
    free(this);
}

CGString* BNFScannerRule_getName(BNFScannerRule* this) {
    return this->name;
}

BNFToken* BNFScannerRule_applyToText(BNFScannerRule* this, const CGString* text) {
    CGArrayIterator(BNFScannerNode)* iter = CGArrayIterator__new(BNFScannerNode, this->nodes);
    BNFScannerNode* node = NULL;
    while ((node = CGArrayIterator_fetch(BNFScannerNode, iter)) != NULL) {
#ifdef DEBUG
        printf("applyToText reading node %s\n", BNFTokenType_toString(BNFScannerNode_getTokenType(node)));
#endif
        this->successNode = node;
        BNFToken* token = BNFScannerNode_applyToText(this->successNode, text);
#ifdef DEBUG
        printf("applyToText received token %s\n", token ? BNFTokenType_toString(BNFToken_getType(token)) : "(NULL)");
#endif
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
