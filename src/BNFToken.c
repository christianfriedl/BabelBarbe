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
#include"BNF.h"
#include"BNFToken.h"

BNFToken* BNFToken__new() {
    BNFToken* token = malloc(sizeof(*token));
    if (token) {
        token->type = BNFTokenType_start;
        token->text = NULL;
    } else
        BNF__raiseFatalError("Unable to allocate token.");
    return token;
}

BNFToken* BNFToken__newFromTypeString(BNFTokenType type, const char* text) {
    BNFToken* token = BNFToken__new();
    token->type = type;
    token->text = strdup(text);
    return token;
}

void BNFToken_delete(BNFToken* token) {
    if (token->text != NULL)
        free(token->text);
    free(token);
}

void BNFToken_print(BNFToken* token) {
    char* type_name = BNFToken_getTypeName(token);
    printf("token @%ld: type='%s', text='%s'\n", (long int)token, type_name, token->text);
    free(type_name);
}

char* BNFToken_toString(BNFToken* token) {
    char* type_name = BNFToken_getTypeName(token);
    char* mystrbuf = malloc(255);
    sprintf(mystrbuf, "token @%ld: type='%s', text='%s'", (long int)token, type_name, token->text);
    free(type_name);
    return mystrbuf;
}

char* BNFToken_getTypeName(BNFToken* token) {
    switch (token->type) {
        case BNFTokenType_start: 
            return strdup("BNFTokenType_start");
            break;
        case BNFTokenType_identifier: 
            return strdup("BNFTokenType_identifier");
            break;
        case BNFTokenType_definition:
            return strdup("BNFTokenType_definition");
            break;
        case BNFTokenType_semicolon: 
            return strdup("BNFTokenType_semicolon");
            break;
        default:
            BNF__raiseFatalError("unknown token type");
            return NULL;
    }
}
