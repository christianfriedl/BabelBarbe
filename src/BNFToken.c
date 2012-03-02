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

CGString* BNFTokenType_toString(BNFTokenType this) {
    switch (this) {
        case BNFTokenType_nonTerminal: return CGString__new("BNFTokenType_nonTerminal"); break;
        case BNFTokenType_noise: return CGString__new("BNFTokenType_noise"); break;
        case BNFTokenType_identifier: return CGString__new("BNFTokenType_identifier"); break;
        case BNFTokenType_definitionSign: return CGString__new("BNFTokenType_definitionSign"); break;
        case BNFTokenType_semicolon: return CGString__new("BNFTokenType_semicolon"); break;
        case BNFTokenType_OrSign: return CGString__new("BNFTokenType_OrSign"); break;
        case BNFTokenType_openParen: return CGString__new("BNFTokenType_openParen"); break;
        case BNFTokenType_closeParen: return CGString__new("BNFTokenType_closeParen"); break;
        case BNFTokenType_repeatZeroOrOnce: return CGString__new("BNFTokenType_repeatZeroOrOnce"); break;
        case BNFTokenType_repeatZeroOrMore: return CGString__new("BNFTokenType_repeatZeroOrMore"); break;
        case BNFTokenType_repeatMany: return CGString__new("BNFTokenType_repeatMany"); break;
        case BNFTokenType_stringLiteral: return CGString__new("BNFTokenType_stringLiteral"); break;
        case BNFTokenType_regexLiteral : return CGString__new("BNFTokenType_regexLiteral "); break;
        default: CGAppState_THROW(CGAppState__getInstance(), Severity_warning, BNFExceptionID_UnknownBNFTokenType, "No such BNFTokenType"); return CGString__new("unkown BNFTokenType"); break;
    }
}

BNFToken* BNFToken__new(BNFTokenType type, CGString* text) {
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
    printf("token @%ld: type='%s', text='%s'\n", (long int)this, type_name, this->text);
    free(type_name);
}

CGString* BNFToken_toString(BNFToken* this) {
    CGString* type_name = BNFToken_getTypeName(this);
    CGString* string = CGString__newWithSprintf("token @%ld: type='%s', text='%s'", (long int)this, type_name, this->text);
    CGString_delete(type_name);
    return string;
}

CGString* BNFToken_getTypeName(BNFToken* this) {
    switch (this->type) {
        case BNFTokenType_nonTerminal: 
            return CGString__new("BNFTokenType_nonTerminal");
            break;
        case BNFTokenType_noise: 
            return CGString__new("BNFTokenType_noise");
            break;
        case BNFTokenType_identifier: 
            return CGString__new("BNFTokenType_identifier");
            break;
        case BNFTokenType_definitionSign:
            return CGString__new("BNFTokenType_definitionSign");
            break;
        case BNFTokenType_semicolon: 
            return CGString__new("BNFTokenType_semicolon");
            break;
        default:
            CGAppState_THROW(CGAppState__getInstance(), Severity_error, BNFExceptionID_UnknownBNFTokenType, "unknown token type %u", this->type);
            return CGString__new("Unknown Token Type");
    }
}

BNFTokenType BNFToken_getType(BNFToken* this) {
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
