#ifndef _BNF_TOKEN_H
#define _BNF_TOKEN_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<cgenerics/CGString.h>
#include<cgenerics/CGAppState.h>

typedef enum { BNFTokenType_nonTerminal, BNFTokenType_noise, BNFTokenType_identifier, BNFTokenType_definitionSign, BNFTokenType_semicolon, BNFTokenType_OrSign, BNFTokenType_openParen, BNFTokenType_closeParen, BNFTokenType_repeatZeroOrOnce, BNFTokenType_repeatZeroOrMore, BNFTokenType_repeatMany, BNFTokenType_stringLiteral, BNFTokenType_regexLiteral } BNFTokenType;

typedef struct {
    BNFTokenType type;
    CGString* text;
    unsigned int textLength;
} BNFToken;

CGString* BNFTokenType_toString(BNFTokenType this);

BNFToken* BNFToken__new(BNFTokenType type, CGString* text);
BNFToken* BNFToken_clone(BNFToken* this);
void BNFToken_delete(BNFToken* this);
void BNFToken_print(BNFToken* this);
CGString* BNFToken_toString(BNFToken* this);
CGString* BNFToken_getTypeName(BNFToken* this);
CGString* BNFToken_getText(BNFToken* this);
BNFTokenType BNFToken_getType(BNFToken* this);
unsigned int BNFToken_getTextLength(BNFToken* this);
void BNFToken_setTextLength(BNFToken* this, unsigned int length);
bool BNFToken_isEQual(BNFToken* this, BNFToken* other);

/* NOTE: this is a function specific to the BNF parser, not the framework */
/* this removes the quotation marks from the START and the END of the token text (if any), returning a new token */
BNFToken* BNFToken_removeQuotationMarks(BNFToken* token);
/* NOTE: this is a function specific to the BNF parser, not the framework */
/* this removes the slashes from the START and the END of the token text (if any), returning a new token */
BNFToken* BNFToken_removeRegexSlashes(BNFToken* token);
#endif
