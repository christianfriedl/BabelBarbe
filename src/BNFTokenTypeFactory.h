#ifndef _BNF_TOKEN_TYPE_FACTORY_H
#define _BNF_TOKEN_TYPE_FACTORY_H

#include<cgenerics/CGAppState.h>
#include<cgenerics/CGString.h>
#include<stdlib.h>
#include"BNFTokenType.h"

typedef struct {
    unsigned int nextId;
} BNFTokenTypeFactory;

BNFTokenTypeFactory* BNFTokenTypeFactory_delete(BNFTokenTypeFactory* this);
BNFTokenTypeFactory* BNFTokenTypeFactory__getInstance();
BNFTokenType* BNFTokenTypeFactory_createBNFTokenType(BNFTokenTypeFactory* this, CGString* name);


#endif
