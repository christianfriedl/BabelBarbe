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

BNFToken* BNFToken__new(CGAppState* appState, BNFTokenType type, CGString* text) {
    BNFToken* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->type = type;
        this->text = text;
    } else
        CGAppState_THROW(appState, Severity_error, CGExceptionID_CannotAllocate, "unable to allocate BNFToken");
    return this;
}

void BNFToken_delete(CGAppState* appState, BNFToken* this) {
    if (this->text != NULL) /* TODO: decide - will that ever be possible? */
        free(this->text);
    free(this);
}

void BNFToken_print(CGAppState* appState, BNFToken* this) {
    char* type_name = BNFToken_getTypeName(appState, this);
    printf("token @%ld: type='%s', text='%s'\n", (long int)this, type_name, this->text);
    free(type_name);
}

CGString* BNFToken_toString(CGAppState* appState, BNFToken* this) {
    CGString* type_name = BNFToken_getTypeName(appState, this);
    CGString* string = CGString__newWithSprintf(appState, "token @%ld: type='%s', text='%s'", (long int)this, type_name, this->text);
    CGString_delete(appState, type_name);
    return string;
}

CGString* BNFToken_getTypeName(CGAppState* appState, BNFToken* this) {
    switch (this->type) {
        case BNFTokenType_start: 
            return CGString__new(appState, "BNFTokenType_start");
            break;
        case BNFTokenType_identifier: 
            return CGString__new(appState, "BNFTokenType_identifier");
            break;
        case BNFTokenType_definition:
            return CGString__new(appState, "BNFTokenType_definition");
            break;
        case BNFTokenType_semicolon: 
            return CGString__new(appState, "BNFTokenType_semicolon");
            break;
        default:
            CGAppState_THROW(appState, Severity_error, BNFExceptionID_UnknownTokenType, "unknown token type %u", this->type);
            return CGString__new(appState, "Unknown Token Type");
    }
}

CGString* BNFToken_getText(CGAppState* appState, BNFToken* this) {
    return this->text;
}

bool BNFToken_isEQual(CGAppState* appState, BNFToken* this, BNFToken* other) {
    return toBool((this->type == other->type && !CGString__compare(appState, this->text, other->text)));
}
