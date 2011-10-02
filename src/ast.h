    /*
    BNF...

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

#include"token.h"

struct ast_leaf;
typedef struct ast_leaf {
    token_t *token;
    struct ast_leaf *parent;
    struct ast_leaf *left;
    struct ast_leaf *right;
} ast_leaf_t;

typedef enum { ast_pos_left = 0, ast_pos_right = 1, ast_pos_nowhere = 2 } ast_pos_enum;

ast_leaf_t *ast__new(ast_leaf_t *parent, token_t *token, ast_pos_enum where);
void ast__delete(ast_leaf_t *this);
void ast__print(ast_leaf_t *this);

#endif

