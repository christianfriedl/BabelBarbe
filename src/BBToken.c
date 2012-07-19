/*
BB Parser

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
#include"BB.h"
#include"BBException.h"
#include"BBToken.h"

BBToken* BBToken__new(BBTokenType* type, CGString* text) {
    BBToken* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->type = type;
        this->text = text;
        this->textLength = CGString_getSize(text);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, CGExceptionID_CannotAllocate, "unable to allocate BBToken");
    return this;
}

BBToken* BBToken_clone(BBToken* this) {
	return BBToken__new(this->type, this->text);
}

void BBToken_delete(BBToken* this) {
    if (this->text != NULL) /* TODO: decide - will that ever be possible? */
        free(this->text);
    free(this);
}

void BBToken_print(BBToken* this) {
    char* type_name = BBToken_getTypeName(this);
    printf("token @%ld (type='%s', text='%s')", (long int)this, type_name, this->text);
}

CGString* BBToken_toString(BBToken* this) {
    CGString* type_name = BBToken_getTypeName(this);
    CGString* string = CGString__newWithSprintf("token @%ld: type='%s', text='%s'", (long int)this, type_name, this->text);
    return string;
}

CGString* BBToken_getTypeName(BBToken* this) {
    return BBTokenType_getName(this->type);
}

BBTokenType* BBToken_getType(BBToken* this) {
    return this->type;
}
CGString* BBToken_getText(BBToken* this) {
    return this->text;
}
unsigned int BBToken_getTextLength(BBToken* this) {
    return this->textLength;
}
void BBToken_setTextLength(BBToken* this, unsigned int length) {
    this->textLength = length;
}

bool BBToken_isEQual(BBToken* this, BBToken* other) {
    return toBool((this->type == other->type && !CGString__compare(this->text, other->text)));
}

/* NOTE: this is a function specific to the BB parser, not the framework */
BBToken* BBToken_removeQuotationMarks(BBToken* this) {
    char *newText = NULL;
    if (CGString_getCharAt(this->text, 0) == '\'' && CGString_getCharAt(this->text, CGString_getSize(this->text) - 1) == '\'') 
        newText = CGString_createSubstring(this->text, 1, CGString_getSize(this->text) - 2);
    else
        newText = CGString_clone(this->text); /* has to be cloned because the old text will be destroyed by delete'ing the old token */
    return BBToken__new(this->type, newText);
    
}
/* NOTE: this is a function specific to the BB parser, not the framework */
BBToken* BBToken_removeRegexSlashes(BBToken* this) {
    char *newText = NULL;
    if (CGString_getCharAt(this->text, 0) == '/' && CGString_getCharAt(this->text, CGString_getSize(this->text) - 1) == '/') 
        newText = CGString_createSubstring(this->text, 1, CGString_getSize(this->text) - 2);
    else
        newText = CGString_clone(this->text); /* has to be cloned because the old text will be destroyed by delete'ing the old token */
    return BBToken__new(this->type, newText);
    
}
