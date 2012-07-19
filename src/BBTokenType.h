#ifndef _BB_TOKEN_TYPE_H
#define _BB_TOKEN_TYPE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<cgenerics/CGString.h>
#include"BB.h"

typedef struct {
    unsigned int id;
    CGString* name;
} BBTokenType;

BBTokenType* BBTokenType__new(unsigned int id, CGString* name);
BBTokenType* BBTokenType_clone(const BBTokenType* this);
void BBTokenType_delete(BBTokenType* this);
unsigned int BBTokenType_getId(BBTokenType* this);
CGString* BBTokenType_toString(BBTokenType* this);
CGString* BBTokenType_getName(BBTokenType* this);
bool BBTokenType_isEqual(BBTokenType* this, BBTokenType* that);
CGString* BBTokenType_createCDeclaration(BBTokenType* this);
CGString* BBTokenType_createCConstructor(BBTokenType* this);

#endif
