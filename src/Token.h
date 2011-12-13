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

#ifndef _TOKEN_H
#define _TOKEN_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum { TokenType_start, TokenType_identifier, TokenType_definition, TokenType_semicolon } TokenType;

typedef struct {
    TokenType type;
    char* text;
} Token;

Token* Token__new();
Token* Token__newFromTypeString(TokenType type, const char* text);
void Token_delete(Token* token);
void Token_print(Token* token);
char* Token_getTypeName(Token* token);
char* Token_toString(Token* token);

#endif
