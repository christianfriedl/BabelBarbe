#ifndef _BB_CODE_GENERATOR_H
#define _BB_CODE_GENERATOR_H

#include<cgenerics/CGArray.h>
#include<cgenerics/CGString.h>
#include"BBParserRuleset.h"
#include"BBAst.h"

DECLARE_ARRAY(CGString)

typedef struct {
    BBAst* ast;
    BBSentence* startSentence;
    CGArray(BBSentence)* currentSentenceStack;
    BBTokenType* nonTerminalTokenType;
    unsigned int ruleTokenTypeCount;
    unsigned int ruleSentenceCount;
    CGString* ruleName;
    CGArray(BBSentence)* ruleSentences;
    CGArray(BBSentence)* terminalSentences;
    CGArray(BBTokenType)* tokenTypes;
    CGArray(BBScannerNode)* scannerNodes;
} BBCodeGenerator;

BBCodeGenerator* BBCodeGenerator__new(BBAst* ast);
void BBCodeGenerator_delete(BBCodeGenerator* this);
CGString* BBCodeGenerator_createCode(BBCodeGenerator* this);
BBTokenType* BBCodeGenerator_createTokenType(BBCodeGenerator* this); /* TODO this should be private, I made it public for testing */

#endif

