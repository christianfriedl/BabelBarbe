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
#include"../ast.h"

/*
    positive tests
*/

void test_ast__print() {
    ast_leaf_t *root = ast__new(NULL, token__new_from_type_string_len(t_start, "root", 4), ast_pos_nowhere);
    ast_leaf_t *left1 = ast__new(root, token__new_from_type_string_len(t_symbol, "left1", 5), ast_pos_left);
    ast_leaf_t *left1_left = ast__new(left1, token__new_from_type_string_len(t_symbol, "left1_left", 10), ast_pos_left);
    ast_leaf_t *left1_right = ast__new(left1, token__new_from_type_string_len(t_symbol, "left1_right", 11), ast_pos_right);
    ast_leaf_t *right1 = ast__new(root, token__new_from_type_string_len(t_symbol, "right1", 6), ast_pos_right);
    ast_leaf_t *right1_right = ast__new(right1, token__new_from_type_string_len(t_symbol, "right1_right", 12), ast_pos_right);
    printf("%s...\n", __func__);

    ast__print(root);
    
    printf("ok\n");
}


int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_ast__print();

    /*
        negative tests
    */

	return 0;
}
