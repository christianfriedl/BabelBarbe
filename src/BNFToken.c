/*
BNF Parser

Copyright (C) 2011  Christian Friedl

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<cgenerics/CGAppState.h>
#include"BNF.h"
#include"BNFException.h"
#include"BNFToken.h"

BNFToken* BNFToken__new(BNFTokenType* type, CGString* text) {
    BNFToken* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->type = type;
        this->text = text;
        this->textLength = CGString_getSize(text);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, CGExceptionID_CannotAllocate, "unable to allocate BNFToken");
    return this;
}

BNFToken* BNFToken_clone(BNFToken* this) {
	return BNFToken__new(this->type, this->text);
}

void BNFToken_delete(BNFToken* this) {
    if (this->text != NULL) /* TODO: decide - will that ever be possible? */
        free(this->text);
    free(this);
}

void BNFToken_print(BNFToken* this) {
    char* type_name = BNFToken_getTypeName(this);
    printf("token @%ld (type='%s', text='%s')", (long int)this, type_name, this->text);
}

CGString* BNFToken_toString(BNFToken* this) {
    CGString* type_name = BNFToken_getTypeName(this);
    CGString* string = CGString__newWithSprintf("token @%ld: type='%s', text='%s'", (long int)this, type_name, this->text);
    return string;
}

CGString* BNFToken_getTypeName(BNFToken* this) {
    return BNFTokenType_getName(this->type);
}

BNFTokenType* BNFToken_getType(BNFToken* this) {
    return this->type;
}
CGString* BNFToken_getText(BNFToken* this) {
    return this->text;
}
unsigned int BNFToken_getTextLength(BNFToken* this) {
    return this->textLength;
}
void BNFToken_setTextLength(BNFToken* this, unsigned int length) {
    this->textLength = length;
}

bool BNFToken_isEQual(BNFToken* this, BNFToken* other) {
    return toBool((this->type == other->type && !CGString__compare(this->text, other->text)));
}

/* NOTE: this is a function specific to the BNF parser, not the framework */
BNFToken* BNFToken_removeQuotationMarks(BNFToken* this) {
    char *newText = NULL;
    if (CGString_getCharAt(this->text, 0) == '\'' && CGString_getCharAt(this->text, CGString_getSize(this->text) - 1) == '\'') 
        newText = CGString_createSubstring(this->text, 1, CGString_getSize(this->text) - 2);
    else
        newText = CGString_clone(this->text); /* has to be cloned because the old text will be destroyed by delete'ing the old token */
    return BNFToken__new(this->type, newText);
    
}
/* NOTE: this is a function specific to the BNF parser, not the framework */
BNFToken* BNFToken_removeRegexSlashes(BNFToken* this) {
    char *newText = NULL;
    if (CGString_getCharAt(this->text, 0) == '/' && CGString_getCharAt(this->text, CGString_getSize(this->text) - 1) == '/') 
        newText = CGString_createSubstring(this->text, 1, CGString_getSize(this->text) - 2);
    else
        newText = CGString_clone(this->text); /* has to be cloned because the old text will be destroyed by delete'ing the old token */
    return BNFToken__new(this->type, newText);
    
}
