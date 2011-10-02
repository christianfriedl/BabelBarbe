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
#include<stdio.h>
#include"../token.h"

/*
    positive tests
*/
void test_token_new_delete() {
    char *text = "xyz";
    printf("%s...\n", __func__);
    token_t *token = token__new();
    token->type = t_literal_uint;
    token->text = text;
    token__delete(token);        // works, because currently, token_delete does not delete the string
    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_token_new_delete();

	return 0;
}
