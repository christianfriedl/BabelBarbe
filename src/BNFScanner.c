#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<cgenerics/CGException.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGArray.h>
#include"BNF.h"
#include"BNFToken.h"
#include"BNFException.h"
#include"BNFScanner.h"

DEFINE_ARRAY_FUNCS(BNFToken)

BNFScanner* BNFScanner__new(BNFScannerRule* startRule, CGString* text) {
    BNFScanner* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->currentRule = startRule;
        this->text = text;
        this->textPtr = this->text;
        this->textEndPtr = this->textPtr + CGString_getSize(this->text);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate BNFScanner");
    return this;
}

void BNFScanner_delete(BNFScanner* this) {
    /* TODO delete rules */
    CGString_delete(this->text);
    free(this);
}

BNFToken* BNFScanner_scanNextToken(BNFScanner* this) {
    if (this->textPtr == this->textEndPtr) /* end of text reached, so no more tokens */
        return NULL;
    if (this->currentRule == NULL) { /* we are not at EOT, and we have no rule to follow -> error */
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, BNFExceptionID_ScannerError, "No rule for text at (%ld, %ld) %u, near '%s ...'", this->text, this->textPtr, (this->textPtr - this->text), CGString_createSubstring(this->textPtr, 0, 20));
        return NULL;
    }
        
    BNFToken* token = BNFScannerRule_applyToText(this->currentRule, this->textPtr);
    if (token != NULL) {
        BNFScannerNode* node = BNFScannerRule_getNode(this->currentRule);
        this->textPtr += BNFToken_getTextLength(token);
        this->currentRule = BNFScannerNode_getFollowupRule(node);
    } else 
        /* no token, not noise, but not EOT, therefore there was an error */
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, BNFExceptionID_ScannerError, "Scanner error at %d, near '%s ...'", (this->textPtr - this->text), CGString_createSubstring(this->textPtr, 0, 20));
    return token;
}

CGArray(BNFToken)* BNFScanner_scanAllTokens(BNFScanner* this) {
	CGArray(BNFToken)* tokenList = CGArray__new(BNFToken, 64);
	BNFToken* token = NULL;
	while ((token = BNFScanner_scanNextToken(this)) != NULL)
		CGArray_add(BNFToken, tokenList, token);
	return tokenList;
}