    /*
    DaSL - Datetime Specific Language, a little DSL for dealing with dates and times

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

#include<assert.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"ASTLeaf.h"
#include"Token.h"

/*
    positive tests
*/

void test_ast__print() {
    ASTLeaf* root = ASTLeaf__new(NULL, Token__newFromTypeString(TokenType_start, "root"), ASTLeafPosition_nowhere);
    ASTLeaf* left1 = ASTLeaf__new(root, Token__newFromTypeString(TokenType_identifier, "left1"), ASTLeafPosition_left);
    ASTLeaf* left1_left = ASTLeaf__new(left1, Token__newFromTypeString(TokenType_identifier, "left1_left"), ASTLeafPosition_left);
    ASTLeaf* left1_right = ASTLeaf__new(left1, Token__newFromTypeString(TokenType_identifier, "left1_right"), ASTLeafPosition_right);
    ASTLeaf* right1 = ASTLeaf__new(root, Token__newFromTypeString(TokenType_identifier, "right1"), ASTLeafPosition_right);
    ASTLeaf* right1_right = ASTLeaf__new(right1, Token__newFromTypeString(TokenType_identifier, "right1_right"), ASTLeafPosition_right);
    printf("%s...\n", __func__);

    ASTLeaf_print(root);
    ASTLeaf_delete(root);
    
    printf("ok\n");
}
void test_ast_setters_getters() {
    ASTLeaf* root = ASTLeaf__new(NULL, Token__newFromTypeString(TokenType_start, "root"), ASTLeafPosition_nowhere);
    ASTLeaf* left1 = ASTLeaf__new(root, Token__newFromTypeString(TokenType_identifier, "left1"), ASTLeafPosition_left);
    ASTLeaf* left1_left = ASTLeaf__new(left1, Token__newFromTypeString(TokenType_identifier, "left1_left"), ASTLeafPosition_left);
    ASTLeaf* left1_right = ASTLeaf__new(left1, Token__newFromTypeString(TokenType_identifier, "left1_right"), ASTLeafPosition_right);
    ASTLeaf* right1 = ASTLeaf__new(root, Token__newFromTypeString(TokenType_identifier, "right1"), ASTLeafPosition_right);
    ASTLeaf* right1_right = ASTLeaf__new(right1, Token__newFromTypeString(TokenType_identifier, "right1_right"), ASTLeafPosition_right);
    printf("%s...\n", __func__);

    assert(ASTLeaf_getParent(left1) == root);
    assert(ASTLeaf_getLeft(root) == left1);
    assert(ASTLeaf_getRight(root) == right1);

    ASTLeaf_setLeft(root, right1);
    ASTLeaf_setRight(root, left1);
    assert(ASTLeaf_getLeft(root) == right1);
    assert(ASTLeaf_getRight(root) == left1);
    ASTLeaf_setParent(right1_right, root);
    assert(ASTLeaf_getParent(right1_right) == root);

    ASTLeaf_delete(root);
    
    printf("ok\n");
}



int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_ast__print();
    test_ast_setters_getters();

    /*
        negative tests
    */

	return 0;
}
