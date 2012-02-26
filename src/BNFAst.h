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

struct BNFAst_struct;
typedef struct BNFAst_struct {
    Token* token;
    struct BNFAst_struct* parent;
    struct BNFAst_struct* left;
    struct BNFAst_struct* right;
} BNFAst;

typedef enum { BNFAstPosition_left = 0, BNFAstPosition_right = 1, BNFAstPosition_nowhere = 2 } BNFAstPosition;

BNFAst* BNFAst__new(BNFAst* parent, Token* token, BNFAstPosition where);
void BNFAst_delete(BNFAst* this);
void BNFAst_print(BNFAst* this);
BNFAst* BNFAst_getParent(BNFAst* this);
BNFAst* BNFAst_getLeft(BNFAst* this);
BNFAst* BNFAst_getRight(BNFAst* this);
Token* BNFAst_getToken(BNFAst* this);
void BNFAst_setToken(BNFAst* this, Token* token);
void BNFAst_setParent(BNFAst* this, BNFAst* parent);
void BNFAst_setLeft(BNFAst* this, BNFAst* left);
void BNFAst_setRight(BNFAst* this, BNFAst* right);

#endif

