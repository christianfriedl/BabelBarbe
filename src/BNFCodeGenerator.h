#ifndef _BNF_CODE_GENERATOR_H
#define _BNF_CODE_GENERATOR_H

#include<cgenerics/CGArray.h>
#include<cgenerics/CGString.h>
#include"BNFParserRuleset.h"
#include"BNFAst.h"

DECLARE_ARRAY(CGString)

typedef struct {
    BNFAst* ast;
    BNFSentence* startSentence;
    CGArray(BNFSentence)* currentSentenceStack;
    unsigned int nextTokenType;
} BNFCodeGenerator;

BNFCodeGenerator* BNFCodeGenerator__new(BNFAst* ast);
void BNFCodeGenerator_delete(BNFCodeGenerator* this);
CGString* BNFCodeGenerator_createCode(BNFCodeGenerator* this);
BNFTokenType* BNFCodeGenerator_createTokenType(BNFCodeGenerator* this); /* TODO this should be private, I made it public for testing */

#endif

