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

#include"bnf.h"
#include"ast.h"

ast_leaf_t *ast__new(ast_leaf_t *parent, token_t *token, ast_pos_enum where) {
    ast_leaf_t *this = malloc(sizeof(*this));
    if (this != NULL) {
        this->parent = parent;
        this->token = token;
        this->left = NULL;
        this->right = NULL;
        if (parent != NULL) {
            if (where == ast_pos_left)
                parent->left = this;
            else if (where == ast_pos_right)
                parent->right = this;
            else
                bnf_raise_fatal_error("Parent given, but no ast_pos argument");
        }
    } else
        bnf_raise_fatal_error("Unable to allocate ast_leaf.");
    return this;
}
void ast__delete(ast_leaf_t *this) {
    if (this->left != NULL)
        ast__delete(this->left);
    if (this->right != NULL)
        ast__delete(this->right);
    token__delete(this->token);
    free(this);
}
ast_leaf_t *ast__get_parent(ast_leaf_t *this) {
    return this->parent;
}
ast_leaf_t *ast__get_left(ast_leaf_t *this) {
    return this->left;
}
ast_leaf_t *ast__get_right(ast_leaf_t *this) {
    return this->right;
}
token_t *ast__get_token(ast_leaf_t *this) {
    return this->token;
}
void ast__set_token(ast_leaf_t *this, token_t *token) {
    token__delete(this->token);
    this->token = token;
}
void ast__set_parent(ast_leaf_t *this, ast_leaf_t *parent) {
    this->parent = parent;
}
void ast__set_left(ast_leaf_t *this, ast_leaf_t *left) {
    this->left = left;
}
void ast__set_right(ast_leaf_t *this, ast_leaf_t *right) {
    this->right = right;
}
void ast__print_internal(ast_leaf_t *this, unsigned int level);
void ast__print(ast_leaf_t *this) {
    ast__print_internal(this, 0);
}
void ast__print_internal(ast_leaf_t *this, unsigned int level) {
    unsigned int i;
    if (this->left != NULL)
        ast__print_internal(this->left, level + 1);
    for (i=0; i < level; ++i)
        printf("    ");
    token__print(this->token);
    if (this->right != NULL)
        ast__print_internal(this->right, level + 1);
}
