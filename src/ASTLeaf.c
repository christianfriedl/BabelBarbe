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

#include"bnf.h"
#include"ASTLeaf.h"

ASTLeaf *ASTLeaf__new(ASTLeaf *parent, token_t *token, ASTLeafPosition where) {
    ASTLeaf *this = malloc(sizeof(*this));
    if (this != NULL) {
        this->parent = parent;
        this->token = token;
        this->left = NULL;
        this->right = NULL;
        if (parent != NULL) {
            if (where == ASTLeafPosition_left)
                parent->left = this;
            else if (where == ASTLeafPosition_right)
                parent->right = this;
            else
                bnf_raise_fatal_error("Parent given, but no ast_pos argument");
        }
    } else
        bnf_raise_fatal_error("Unable to allocate ASTLeaf.");
    return this;
}
void ASTLeaf_delete(ASTLeaf *this) {
    if (this->left != NULL)
        ASTLeaf_delete(this->left);
    if (this->right != NULL)
        ASTLeaf_delete(this->right);
    token__delete(this->token);
    free(this);
}
ASTLeaf *ASTLeaf_getParent(ASTLeaf *this) {
    return this->parent;
}
ASTLeaf *ASTLeaf_getLeft(ASTLeaf *this) {
    return this->left;
}
ASTLeaf *ASTLeaf_getRight(ASTLeaf *this) {
    return this->right;
}
token_t *ASTLeaf_getToken(ASTLeaf *this) {
    return this->token;
}
void ASTLeaf_setToken(ASTLeaf *this, token_t *token) {
    token__delete(this->token);
    this->token = token;
}
void ASTLeaf_setParent(ASTLeaf *this, ASTLeaf *parent) {
    this->parent = parent;
}
void ASTLeaf_setLeft(ASTLeaf *this, ASTLeaf *left) {
    this->left = left;
}
void ASTLeaf_setRight(ASTLeaf *this, ASTLeaf *right) {
    this->right = right;
}
void ASTLeaf_print_(ASTLeaf *this, unsigned int level);
void ASTLeaf_print(ASTLeaf *this) {
    ASTLeaf_print_(this, 0);
}
void ASTLeaf_print_(ASTLeaf *this, unsigned int level) {
    unsigned int i;
    if (this->left != NULL)
        ASTLeaf_print_(this->left, level + 1);
    for (i=0; i < level; ++i)
        printf("    ");
    token__print(this->token);
    if (this->right != NULL)
        ASTLeaf_print_(this->right, level + 1);
}
