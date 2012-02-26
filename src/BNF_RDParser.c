#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"BNF_RDParser.h"


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

BNFSentence* BNFSentence__new(CGString* name, BNFAst* (*parse)(BNF_RDParser* parser), CGArray(BNFAlternative)* alternatives) {
    BNFSentence* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->name = name;
        this->parse = parse;
        this->alternatives = alternatives;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}
BNFSentence* BNFSentence_clone(BNFSentence* this) {
    return BNFSentence__new(this->name, this->parse, this->alternatives);
}
void BNFSentence_delete(BNFSentence* this) {
    CGString_delete(this->name);
    CGArray_deleteValues(BNFAlternative, this->alternatives);
    CGArray_delete(BNFAlternative, this->alternatives);
    free(this);
}

BNF_RDParser* BNF_RDParser__new(CGArray(BNFToken)* tokenList, BNFSentence* startSentence) {
    BNF_RDParser* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->tokenList = tokenList;
        this->startSentence = startSentence;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}
void BNF_RDParser_delete(BNF_RDParser* this) {
    CGArray_deleteValues(BNFToken, this->tokenList);
    CGArray_delete(BNFToken, this->tokenList);
    BNFSentence_delete(this->startSentence);
    free(this);
}

