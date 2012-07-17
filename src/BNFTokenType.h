#ifndef _BNF_TOKEN_TYPE_H
#define _BNF_TOKEN_TYPE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<cgenerics/CGString.h>
#include"BNF.h"

typedef struct {
    unsigned int id;
    CGString* name;
} BNFTokenType;

BNFTokenType* BNFTokenType__new(unsigned int id, CGString* name);
void BNFTokenType_delete(BNFTokenType* this);
unsigned int BNFTokenType_getId(BNFTokenType* this);
CGString* BNFTokenType_toString(BNFTokenType* this);
CGString* BNFTokenType_getName(BNFTokenType* this);
bool BNFTokenType_isEqual(BNFTokenType* this, BNFTokenType* that);

#endif
