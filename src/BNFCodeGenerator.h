#ifndef _BNF_CODE_GENERATOR_H
#define _BNF_CODE_GENERATOR_H

#include<cgenerics/CGArray.h>
#include<cgenerics/CGString.h>
#include"BNFParserRuleset.h"
#include"BNFAst.h"

DECLARE_ARRAY(CGString)

typedef struct {
    BNFAst* ast;
    CGArray(CGString)* identifiers;
} BNFCodeGenerator;

BNFCodeGenerator* BNFCodeGenerator__new(BNFAst* ast);
void BNFCodeGenerator_delete(BNFCodeGenerator* this);
CGString* BNFCodeGenerator_createCode(BNFCodeGenerator* this);

#endif

