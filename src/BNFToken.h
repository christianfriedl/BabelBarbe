#ifndef _BNF_TOKEN_H
#define _BNF_TOKEN_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<cgenerics/CGString.h>
#include<cgenerics/CGAppState.h>

typedef enum { BNFTokenType_start, BNFTokenType_noise, BNFTokenType_identifier, BNFTokenType_definition, BNFTokenType_semicolon } BNFTokenType;

typedef struct {
    BNFTokenType type;
    CGString* text;
    unsigned int textLength;
} BNFToken;

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

#endif
