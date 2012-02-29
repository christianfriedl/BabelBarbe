#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"BNF_RDParser.h"

DEFINE_ARRAY(BNFSentence);
DEFINE_ARRAY(BNFPhrase);
DEFINE_ARRAY(BNFAlternative);
DEFINE_ARRAY_ITERATOR(BNFToken);


BNFPhrase* BNFPhrase__new(BNFPhraseRepeatSwitch repeatSwitch, CGArray(BNFSentence)* parts) {
    BNFPhrase* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->repeatSwitch = repeatSwitch;
        this->parts = parts;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}

BNFPhrase* BNFPhrase_clone(BNFPhrase* this) {
    return BNFPhrase__new(this->repeatSwitch, this->parts);
}

    
void BNFPhrase_delete(BNFPhrase* this) {
    /* TODO: ooops, with recursion this probably will lead to problems... solution: object-store? coughcough refcounting cough? */
    /* CGArray_deleteValues(BNFSentence, this->parts); */
    CGAppState_THROW(CGAppState__getInstance(), Severity_warning, CGExceptionID_GeneralNonfatalException, "TODO: sentences are currently not deleted - create object-store for them");
    CGArray_delete(BNFSentence, this->parts);
    free(this);
}

void BNFPhrase_setParts(BNFPhrase* this, CGArray(BNFSentence)* parts) {
    this->parts = parts;
}

BNFAst* BNFPhrase_parse(BNFPhrase* this) {
    return NULL;
}

BNFAlternative* BNFAlternative__new(CGArray(BNFPhrase)* phrases) {
    BNFAlternative* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->phrases = phrases;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}
BNFAlternative* BNFAlternative_clone(BNFAlternative* this) {
    return BNFAlternative__new(this->phrases);
}
void BNFAlternative_delete(BNFAlternative* this) {
    CGArray_deleteValues(BNFPhrase, this->phrases);
    CGArray_delete(BNFPhrase, this->phrases);
    free(this);
}

BNFSentence* BNFSentence__new(CGString* name, BNFTokenType tokenType, CGArray(BNFAlternative)* alternatives) {
    BNFSentence* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->name = name;
        this->tokenType = tokenType;
        this->alternatives = alternatives;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}
BNFSentence* BNFSentence_clone(BNFSentence* this) {
    return BNFSentence__new(this->name, this->tokenType, this->alternatives);
}
CGString* BNFSentence_getName(BNFSentence* this) {
    return this->name;
}
void BNFSentence_delete(BNFSentence* this) {
    CGString_delete(this->name);
    if (this->alternatives != NULL) {
        CGArray_deleteValues(BNFAlternative, this->alternatives);
        CGArray_delete(BNFAlternative, this->alternatives);
    }
    free(this);
}

BNFAst* BNFSentence_parse(BNFSentence* this, CGArrayIterator(BNFToken)* iter) {
    if (this->alternatives == NULL) { /* this is a terminal symbol */
        BNFToken* token = CGArrayIterator_fetch(BNFToken, iter);
        if (token == NULL)
            return NULL;
        if (BNFToken_getType(token) == this->tokenType)
            return BNFAst__new(NULL, token, this);
    }
    return NULL;
}

BNF_RDParser* BNF_RDParser__new(BNFSentence* startSentence) {
    BNF_RDParser* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->startSentence = startSentence;
        this->tokenListIterator = NULL;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}
void BNF_RDParser_delete(BNF_RDParser* this) {
    BNFSentence_delete(this->startSentence);
    free(this);
}
BNFAst* BNF_RDParser_parse(BNF_RDParser* this, CGArray(BNFToken)* tokenList) {
    this->tokenListIterator = CGArrayIterator__new(BNFToken, tokenList);
    return BNFSentence_parse(this->startSentence, this->tokenListIterator);
}
