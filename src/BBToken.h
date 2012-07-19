#ifndef _BB_TOKEN_H
#define _BB_TOKEN_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<cgenerics/CGString.h>
#include<cgenerics/CGAppState.h>
#include"BBTokenType.h"
#include"BBTokenTypeFactory.h"

BBTokenType *BBTokenType_nonTerminal, *BBTokenType_noise, *BBTokenType_identifier, *BBTokenType_definitionSign, *BBTokenType_semicolon, 
             *BBTokenType_OrSign, *BBTokenType_openParen, *BBTokenType_closeParen, *BBTokenType_repeatZeroOrOnce, *BBTokenType_repeatZeroOrMore, 
             *BBTokenType_repeatMany, *BBTokenType_stringLiteral, *BBTokenType_regexLiteral;

typedef struct {
    BBTokenType* type;
    CGString* text;
    unsigned int textLength;
} BBToken;

BBToken* BBToken__new(BBTokenType* type, CGString* text);
BBToken* BBToken_clone(BBToken* this);
void BBToken_delete(BBToken* this);
void BBToken_print(BBToken* this);
CGString* BBToken_toString(BBToken* this);
CGString* BBToken_getTypeName(BBToken* this);
CGString* BBToken_getText(BBToken* this);
BBTokenType* BBToken_getType(BBToken* this);
unsigned int BBToken_getTextLength(BBToken* this);
void BBToken_setTextLength(BBToken* this, unsigned int length);
bool BBToken_isEQual(BBToken* this, BBToken* other);

/* NOTE: this is a function specific to the BB parser, not the framework */
/* this removes the quotation marks from the START and the END of the token text (if any), returning a new token */
BBToken* BBToken_removeQuotationMarks(BBToken* token);
/* NOTE: this is a function specific to the BB parser, not the framework */
/* this removes the slashes from the START and the END of the token text (if any), returning a new token */
BBToken* BBToken_removeRegexSlashes(BBToken* token);
#endif
