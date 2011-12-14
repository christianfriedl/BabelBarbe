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

#ifndef _AST_H
#define _AST_H

#include"Token.h"

struct ASTLeaf_struct;
typedef struct ASTLeaf_struct {
    Token* token;
    struct ASTLeaf_struct* parent;
    struct ASTLeaf_struct* left;
    struct ASTLeaf_struct* right;
} ASTLeaf;

typedef enum { ASTLeafPosition_left = 0, ASTLeafPosition_right = 1, ASTLeafPosition_nowhere = 2 } ASTLeafPosition;

ASTLeaf* ASTLeaf__new(ASTLeaf* parent, Token* token, ASTLeafPosition where);
void ASTLeaf_delete(ASTLeaf* this);
void ASTLeaf_print(ASTLeaf* this);
ASTLeaf* ASTLeaf_getParent(ASTLeaf* this);
ASTLeaf* ASTLeaf_getLeft(ASTLeaf* this);
ASTLeaf* ASTLeaf_getRight(ASTLeaf* this);
Token* ASTLeaf_getToken(ASTLeaf* this);
void ASTLeaf_setToken(ASTLeaf* this, Token* token);
void ASTLeaf_setParent(ASTLeaf* this, ASTLeaf* parent);
void ASTLeaf_setLeft(ASTLeaf* this, ASTLeaf* left);
void ASTLeaf_setRight(ASTLeaf* this, ASTLeaf* right);

#endif

