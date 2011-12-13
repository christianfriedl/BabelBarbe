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
#include"Token.h"

/*
    positive tests
*/
void test_token_new_delete() {
    printf("%s...\n", __func__);

    char* text = "xyz";
    Token* token = Token__new();
    token->type = TokenType_identifier;
    token->text = strdup(text);
    Token_delete(token);

    text = "abcde";
    token = Token__newFromTypeString(TokenType_identifier, text);
    assert(token->type == TokenType_identifier);
    assert(!strcmp(token->text, text));
    Token_delete(token);

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
