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
#include"bnf.h"
#include"Token.h"

Token* Token__new() {
    Token* token = malloc(sizeof(*token));
    if (token) {
        token->type = TokenType_start;
        token->text = NULL;
    } else
        BNF__raiseFatalError("Unable to allocate token.");
    return token;
}

Token* Token__newFromTypeString(TokenType type, const char* text) {
    Token* token = Token__new();
    token->type = type;
    token->text = strdup(text);
    return token;
}

void Token_delete(Token* token) {
    if (token->text != NULL)
        free(token->text);
    free(token);
}

void Token_print(Token* token) {
    char* type_name = Token_getTypeName(token);
    printf("token @%ld: type='%s', text='%s'\n", (long int)token, type_name, token->text);
    free(type_name);
}

char* Token_toString(Token* token) {
    char* type_name = Token_getTypeName(token);
    char* mystrbuf = malloc(255);
    sprintf(mystrbuf, "token @%ld: type='%s', text='%s'", (long int)token, type_name, token->text);
    free(type_name);
    return mystrbuf;
}

char* Token_getTypeName(Token* token) {
    switch (token->type) {
        case TokenType_start: 
            return strdup("TokenType_start");
            break;
        case TokenType_identifier: 
            return strdup("TokenType_identifier");
            break;
        case TokenType_definition:
            return strdup("TokenType_definition");
            break;
        case TokenType_semicolon: 
            return strdup("TokenType_semicolon");
            break;
        default:
            BNF__raiseFatalError("unknown token type");
            return NULL;
    }
}
