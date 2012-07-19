#ifndef _BB_TOKEN_TYPE_FACTORY_H
#define _BB_TOKEN_TYPE_FACTORY_H

#include<cgenerics/CGAppState.h>
#include<cgenerics/CGString.h>
#include<stdlib.h>
#include"BBTokenType.h"

typedef struct {
    unsigned int nextId;
} BBTokenTypeFactory;

BBTokenTypeFactory* BBTokenTypeFactory_delete(BBTokenTypeFactory* this);
BBTokenTypeFactory* BBTokenTypeFactory__getInstance();
BBTokenType* BBTokenTypeFactory_createBBTokenType(BBTokenTypeFactory* this, CGString* name);


#endif
