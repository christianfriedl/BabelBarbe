#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<cgenerics/CGException.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGArray.h>
#include"BB.h"
#include"BBToken.h"
#include"BBException.h"
#include"BBScanner.h"

DEFINE_ARRAY_FUNCS(BBToken)

BBScanner* BBScanner__new(BBScannerRule* startRule, CGString* text) {
    BBScanner* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->currentRule = startRule;
        this->text = text;
        this->textPtr = this->text;
        this->textEndPtr = this->textPtr + CGString_getSize(this->text);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_ScannerError, "unable to allocate BBScanner");
    return this;
}

void BBScanner_delete(BBScanner* this) {
    /* TODO delete rules */
    CGString_delete(this->text);
    free(this);
}

BBToken* BBScanner_scanNextToken(BBScanner* this) {
    if (this->textPtr == this->textEndPtr) /* end of text reached, so no more tokens */
        return NULL;
    if (this->currentRule == NULL) { /* we are not at EOT, and we have no rule to follow -> error */
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, BBExceptionID_ScannerError, "No rule for text at (%ld, %ld) %u, near '%s ...'", this->text, this->textPtr, (this->textPtr - this->text), CGString_createSubstring(this->textPtr, 0, 20));
        return NULL;
    }
    BBToken* token = NULL;
    BBScannerNode* node = NULL;
    do {
        node = NULL;
        token = BBScannerRule_applyToText(this->currentRule, this->textPtr);
        if (token != NULL) {
            node = BBScannerRule_getSuccessNode(this->currentRule);
            this->textPtr += BBToken_getTextLength(token);
            token = BBScannerNode_runOnAfterScanToken(node, token);
            this->currentRule = BBScannerNode_getFollowupRule(node);
            if (this->currentRule == NULL && BBScannerNode_getIsNoise(node) == true) {
                CGAppState_THROW(CGAppState__getInstance(), Severity_error, BBExceptionID_ScannerError, "Noise node without followupRule detected.");
                return NULL;
            }
        } else 
            /* no token, not noise, but not EOT, therefore there was an error */
            CGAppState_THROW(CGAppState__getInstance(), Severity_error, BBExceptionID_ScannerError, "Scanner error at %d, near '%s ...'", (this->textPtr - this->text), CGString_createSubstring(this->textPtr, 0, 20));
    } while (node != NULL && BBScannerNode_getIsNoise(node) == true && this->textPtr != this->textEndPtr);
    if (node != NULL && BBScannerNode_getIsNoise(node) == true && this->textPtr == this->textEndPtr)
		return NULL;
    return token;
}

CGArray(BBToken)* BBScanner_scanAllTokens(BBScanner* this) { 
	CGArray(BBToken)* tokenList = CGArray__new(BBToken, 64);
	BBToken* token = NULL;
	while ((token = BBScanner_scanNextToken(this)) != NULL)
		CGArray_add(BBToken, tokenList, token);
	return tokenList;
}
