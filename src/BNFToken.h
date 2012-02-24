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
#include<cgenerics/CGString.h>
#include<cgenerics/CGAppState.h>

typedef enum { BNFTokenType_start, BNFTokenType_identifier, BNFTokenType_definition, BNFTokenType_semicolon } BNFTokenType;

typedef struct {
    BNFTokenType type;
    CGString* text;
} BNFToken;

BNFToken* BNFToken__new(CGAppState* appState, BNFTokenType type, CGString* text);
void BNFToken_delete(CGAppState* appState, BNFToken* this);
void BNFToken_print(CGAppState* appState, BNFToken* this);
CGString* BNFToken_toString(CGAppState* appState, BNFToken* this);
CGString* BNFToken_getTypeName(CGAppState* appState, BNFToken* this);
bool BNFToken_isEQual(CGAppState* appState, BNFToken* this, BNFToken* other);

#endif
