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
#include"../token_list.h"

/*
    positive tests
*/
void test_token_list_new_delete() {
    char *text = "123";
    token_t *token;
    token_list_t *token_list;
    token = token__new(text);
    token->type = t_literal_uint;
    token->text = text;

    token_list = token_list__new(NULL, token);
    token_list__delete(token_list);        
    printf("ok\n");
}

void test_token_list_add() {
    char *text_1 = "123";
    char *text_2 = "456";
    token_t *token_1, *token_2;
    token_list_t *token_list_1, *token_list_2;
    printf("%s...\n", __func__);
    token_1 = token__new();
    token_1->type = t_literal_uint;
    token_1->text = text_1;
    token_2 = token__new();
    token_2->type = t_literal_uint;
    token_2->text = text_2;

    token_list_1 = token_list__new(NULL, token_1);
    token_list_2 = token_list__new(token_list_1, token_2);

    printf("%ld %ld %ld", (long int)token_list_1, (long int)token_1, (long int)token_list_1->token);
    assert(token_list_1->token == token_1);
    assert(token_list_2->token == token_2);
    assert(token_list_1->next == token_list_2);
    assert(token_list_1->prev == NULL);
    assert(token_list_2->next == NULL);
    assert(token_list_2->prev == token_list_1);

    token_list__delete(token_list_1);        
    printf("ok\n");
}

void test_token_list_delete() {
    char *text_1 = "123";
    char *text_2 = "456";
    token_t *token_1, *token_2;
    token_list_t *token_list_1, *token_list_2;
    printf("%s...\n", __func__);
    token_1 = token__new();
    token_1->type = t_literal_uint;
    token_1->text = text_1;
    token_2 = token__new();
    token_2->type = t_literal_uint;
    token_2->text = text_2;

    token_list_1 = token_list__new(NULL, token_1);
    token_list_2 = token_list__new(token_list_1, token_2);

    token_list__delete(token_list_2);

    assert(token_list_1->token == token_1);
    assert(token_list_1->next == NULL);
    assert(token_list_1->prev == NULL);

    token_list__delete(token_list_1);        
    printf("ok\n");
}

void test_token_list_get_prev() {
    char *text_1 = "123";
    char *text_2 = "456";
    token_t *token_1, *token_2, *current_token;
    token_list_t *token_list_1, *token_list_2, *current_token_list;
    printf("%s...\n", __func__);
    token_1 = token__new();
    token_1->type = t_literal_uint;
    token_1->text = text_1;
    token_2 = token__new();
    token_2->type = t_literal_uint;
    token_2->text = text_2;

    token_list_1 = token_list__new(NULL, token_1);
    token_list_2 = token_list__new(token_list_1, token_2);

    current_token_list = token_list__get_prev(token_list_2);
    assert(current_token_list == token_list_1);

    current_token = current_token_list->token;
    assert(current_token == token_1);

    current_token_list = token_list__get_prev(current_token_list);
    assert(current_token_list == NULL);

    token_list__delete(token_list_1);        
    printf("ok\n");
}

void test_token_list_get_next() {
    char *text_1 = "123";
    char *text_2 = "456";
    token_t *token_1, *token_2, *current_token;
    token_list_t *token_list_1, *token_list_2, *current_token_list;
    printf("%s...\n", __func__);
    token_1 = token__new();
    token_1->type = t_literal_uint;
    token_1->text = text_1;
    token_2 = token__new();
    token_2->type = t_literal_uint;
    token_2->text = text_2;

    token_list_1 = token_list__new(NULL, token_1);
    token_list_2 = token_list__new(token_list_1, token_2);

    current_token_list = token_list__get_next(token_list_1);
    assert(current_token_list == token_list_2);

    current_token = current_token_list->token;
    assert(current_token == token_2);

    current_token_list = token_list__get_next(current_token_list);
    assert(current_token_list == NULL);

    token_list__delete(token_list_1);        
    printf("ok\n");
}

void test_token_list_get_token() {
    char *text_1 = "123";
    char *text_2 = "456";
    token_t *token_1, *token_2, *current_token;
    token_list_t *token_list_1, *token_list_2;
    printf("%s...\n", __func__);
    token_1 = token__new();
    token_1->type = t_literal_uint;
    token_1->text = text_1;
    token_2 = token__new();
    token_2->type = t_literal_uint;
    token_2->text = text_2;

    token_list_1 = token_list__new(NULL, token_1);
    token_list_2 = token_list__new(token_list_1, token_2);

    current_token = token_list__get_token(token_list_1);
    assert(current_token == token_1);

    current_token = token_list__get_token(token_list_2);
    assert(current_token == token_2);

    token_list__delete(token_list_1);        
    printf("ok\n");
}


int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_token_list_new_delete();
    test_token_list_add();
    test_token_list_delete();
    test_token_list_get_prev();
    test_token_list_get_next();
    test_token_list_get_token();

	return 0;
}
