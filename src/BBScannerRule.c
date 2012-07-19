#include<string.h>
#include"BBException.h"
#include"BBScannerRule.h"
DECLARE_ARRAY_FUNCS(BBScannerNode)
DEFINE_ARRAY_FUNCS(BBScannerNode)
DECLARE_ARRAY_ITERATOR_FUNCS(BBScannerNode)
DEFINE_ARRAY_ITERATOR_FUNCS(BBScannerNode)

typedef struct {
    unsigned int len;
    bool success;
} ApplyToTextRV_;

static ApplyToTextRV_* BBScannerNode_applyStringToText_(BBScannerNode* this, const CGString* text);
static ApplyToTextRV_* BBScannerNode_applyRegexToText_(BBScannerNode* this, const CGString* text);
static BBToken* BBScannerNode_applyFunctionToText_(BBScannerNode* this, const CGString* text, ApplyToTextRV_*(*func)(BBScannerNode*, const CGString*));

BBScannerNode* BBScannerNode__new(BBScannerNodeType type, CGString* pattern, BBScannerRule* followupRule, 
        BBTokenType* tokenType, bool isNoise, BBToken* (*onAfterScanToken)(BBToken*)) {
    BBScannerNode* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->type = type;
        if (type == BBScannerNodeType_regex) {
            this->pattern = NULL;
            this->regex = NULL; /* vim syntastic does not grok pcre* regex, ignore warning about BBScannerNode not having it */
            CGAppState_catchAndDeleteException(CGAppState__getInstance());
            BBScannerNode_setRegex(this, pattern);
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
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, CGExceptionID_CannotAllocate, "Cannot allocate BBScannerNode");
    return this;
}

BBScannerNode* BBScannerNode_clone(BBScannerNode* this) {
    return BBScannerNode__new(this->type, this->pattern, this->followupRule, this->tokenType, this->isNoise, this->onAfterScanToken);
}

bool BBScannerNode_setRegex(BBScannerNode* this, CGString* pattern) {
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
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, BBExceptionID_PCRERegexError, "PCRE reported a compilation error at offset %i, message: '%s' (pattern '%s')", errorOffset, errorString, this->pattern);
        free(this->pattern);
        this->pattern = NULL;
        success = false;
    }
    return success;
}

void BBScannerNode_delete(BBScannerNode* this) {
    free(this->pattern);
    if (this->type == BBScannerNodeType_regex)
        free(this->regex);
    free(this);
}

BBTokenType* BBScannerNode_getTokenType(BBScannerNode* this) {
    return this->tokenType;
}
bool BBScannerNode_getIsNoise(BBScannerNode* this) {
    return this->isNoise;
}

static BBToken* BBScannerNode_createToken_(BBScannerNode* this, const CGString* text, const unsigned int len) {
    BBToken* token  = NULL;
    if (this->isNoise) {
        token = BBToken__new(this->tokenType, CGString__new(""));
        BBToken_setTextLength(token, len);
        return token;
    } else
        return BBToken__new(this->tokenType, CGString_createSubstring(text, 0, len));
}
BBToken* BBScannerNode_runOnAfterScanToken(BBScannerNode* this, BBToken* token) {
    if (this->onAfterScanToken != NULL) {
        BBToken* token2 = this->onAfterScanToken(token);
        BBToken_delete(token);
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

CGString* BBScannerNode_createCDeclaration(BBScannerNode* this, unsigned int index) {
    CGString* text = CGString__newWithSprintf("BBScannerNode* scannerNode%u = NULL;\n", index);
    return text;
}
CGString* BBScannerNode_createCConstructor(BBScannerNode* this, unsigned int index) {
    CGString* escapedPattern = CGString__new("");
    char* occ = NULL;
    char* oldPos = this->pattern;
    while ((occ = strchr(oldPos, '\\')) != NULL) {
        strncat(escapedPattern, oldPos, occ - oldPos);
        strcat(escapedPattern, "\\\\");
        oldPos = occ + 1;
    }
    strncat(escapedPattern, oldPos, this->pattern + strlen(this->pattern) - oldPos);

    CGString* text = CGString__newWithSprintf("scannerNode%u = BBScannerNode__new(BBScannerNodeType_%s, CGString__new(\"%s\"), scannerRuleNoise, BBTokenType_%s, %s, NULL);\n",
            index, (this->type == BBScannerNodeType_regex ? "regex" : "string"),
            escapedPattern, BBTokenType_getName(this->tokenType), (this->isNoise == true ? "true" : "false"));
    return text;
}

static BBToken* BBScannerNode_applyFunctionToText_(BBScannerNode* this, const CGString* text, ApplyToTextRV_*(*func)(BBScannerNode*, const CGString*)) {
    ApplyToTextRV_* funcRv = (*func)(this, text);
    BBToken* token = NULL;

    if (funcRv->success == true)
        token = BBScannerNode_createToken_(this, text, funcRv->len); 
    else
        token = NULL;
    #ifdef DEBUG
    if (funcRv->success == true)
        printf("ApplyFunctionTotext_ success: %i len: %u; token: type %s, text %s, textlen %u\n", funcRv->success, funcRv->len, BBToken_getTypeName(token), BBToken_getText(token), BBToken_getTextLength(token));
    #endif
    ApplyToTextRV_delete(funcRv);
    return token;
}
/**
    @return success or failure
*/
BBToken* BBScannerNode_applyToText(BBScannerNode* this, const CGString* text) {
    if (this->type == BBScannerNodeType_string) 
        return BBScannerNode_applyFunctionToText_(this, text, BBScannerNode_applyStringToText_);
    else
        return BBScannerNode_applyFunctionToText_(this, text, BBScannerNode_applyRegexToText_);
}

static ApplyToTextRV_* BBScannerNode_applyStringToText_(BBScannerNode* this, const CGString* text) {
    if (!strncmp(this->pattern, text, strlen(this->pattern)))
        return ApplyToTextRV__new(strlen(this->pattern), true);
    else
        return ApplyToTextRV__new(0, false);
}

/**
    @return whether the regex matched
*/
static ApplyToTextRV_* BBScannerNode_applyRegexToText_(BBScannerNode* this, const CGString* text) {
    CGString* errorString = CGString__newFromLengthAndPreset(255, ' ');
    int errorOffset = 0;
    int outputVector[BBScannerNode__PCRE_OVECTOR_COUNT]; /* this is a hardcoded number, which is a severe limitation */
    int reResult = 0;
    
    reResult = pcre_exec(this->regex, NULL, text, strlen(text), 0, PCRE_ANCHORED, outputVector, BBScannerNode__PCRE_OVECTOR_COUNT);
    if (reResult < 0) { /* not found, but also possible error */
        switch (reResult) {
            case PCRE_ERROR_NOMATCH:
                return ApplyToTextRV__new(0, false);
                break;
            default:
                CGAppState_THROW(CGAppState__getInstance(), Severity_error, BBExceptionID_PCRERegexError, "PCRE reported an error, message '%s' at offset %i, resultcode was %i (pattern '%s', text '%s')", errorString, errorOffset, reResult, this->pattern, text);
                return ApplyToTextRV__new(0, false);
        }
    } else if (reResult == 0) {
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, BBExceptionID_PCRERegexError, "PCRE cannot write into outputVector because it is too small (pattern '%s', text '%s' ---- maximum number of sub-parts for regex is %i)", this->pattern, text, BBScannerNode__PCRE_OVECTOR_COUNT / 3);
        return ApplyToTextRV__new(0, false);
    } else
        return ApplyToTextRV__new(outputVector[1] - outputVector[0], true);
}

void BBScannerNode_setFollowupRule(BBScannerNode* this, BBScannerRule* rule) {
	this->followupRule = rule;
}
BBScannerRule* BBScannerNode_getFollowupRule(BBScannerNode* this) {
    return this->followupRule;
}


BBScannerRule* BBScannerRule_clone(BBScannerRule* this) {
    return BBScannerRule__new(CGString__new(""), this->nodes);
}

BBScannerRule* BBScannerRule__new(CGString* name, CGArray(BBScannerNode)* nodes) {
    BBScannerRule* this = malloc(sizeof(*this));
    if (this) {
        this->name = name;
        this->nodes = nodes;
        this->successNode = NULL;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, CGExceptionID_CannotAllocate, "Cannot allocate BBScannerRule");
    return this;
}

void BBScannerRule_delete(BBScannerRule* this) {
    CGString_delete(this->name);
    free(this);
}

CGString* BBScannerRule_getName(BBScannerRule* this) {
    return this->name;
}

BBToken* BBScannerRule_applyToText(BBScannerRule* this, const CGString* text) {
    CGArrayIterator(BBScannerNode)* iter = CGArrayIterator__new(BBScannerNode, this->nodes);
    BBScannerNode* node = NULL;
    while ((node = CGArrayIterator_fetch(BBScannerNode, iter)) != NULL) {
#ifdef DEBUG
        printf("applyToText reading node %s\n", BBTokenType_toString(BBScannerNode_getTokenType(node)));
#endif
        this->successNode = node;
        BBToken* token = BBScannerNode_applyToText(this->successNode, text);
#ifdef DEBUG
        printf("applyToText received token %s\n", token ? BBTokenType_toString(BBToken_getType(token)) : "(NULL)");
#endif
        if (token != NULL)
            return token;
    }
    this->successNode = NULL;
    return NULL;
}

BBScannerNode* BBScannerRule_getSuccessNode(BBScannerRule* this) {
    return this->successNode;
}

void BBScannerRule_setNodes(BBScannerRule* this, CGArray(BBScannerNode)* nodes) {
    this->nodes = nodes;
}
