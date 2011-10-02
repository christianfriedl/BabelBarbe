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
#include"../token_list_container.h"

/*
    positive tests
*/
void test_tlc_new_delete() {
    char *text = "123";
    token_t *token;
    token_list_t *token_list;
    token_list_container_t *tlc;

    printf("%s...\n", __func__);

    token = token__new(text);
    token->type = t_literal_uint;
    token->text = text;

    token_list = token_list__new(NULL, token);
    tlc = tlc__new(token_list);
    assert(tlc != NULL);
    assert(tlc->current_token_list == token_list);
    assert(tlc->token_list_start == token_list);

    tlc__delete(tlc);        

    printf("ok\n");
}

char *text_1 = "123";
char *text_2 = "456";
token_t *token_1, *token_2;
token_list_t *token_list_1, *token_list_2;
token_list_container_t *tlc;

void create_tlc() {
    token_1 = token__new();
    token_1->type = t_literal_uint;
    token_1->text = text_1;
    token_2 = token__new();
    token_2->type = t_literal_uint;
    token_2->text = text_2;

    token_list_1 = token_list__new(NULL, token_1);
    token_list_2 = token_list__new(token_list_1, token_2);

    tlc = tlc__new(token_list_1);
}

void test_tlc_current() {
    token_t *token;

    printf("%s...\n", __func__);

    create_tlc();

    assert(tlc->current_token_list == token_list_1);
    assert(tlc->token_list_start == token_list_1);
    
    token = tlc__get_current(tlc);

    assert(tlc->current_token_list == token_list_1);
    assert(tlc->token_list_start == token_list_1);
    assert(token == token_1);

    tlc__delete(tlc);

    printf("ok\n");
}

void test_tlc_next_prev_start() {
    token_t *token;

    printf("%s...\n", __func__);

    create_tlc();

    assert(tlc->current_token_list == token_list_1);
    assert(tlc->token_list_start == token_list_1);
    
    token = tlc__move_to_next(tlc);

    assert(tlc->current_token_list == token_list_2);
    assert(tlc->token_list_start == token_list_1);
    assert(token == token_2);
    assert(tlc__get_current(tlc) == token_2);

    token = tlc__move_to_prev(tlc);

    assert(tlc->current_token_list == token_list_1);
    assert(tlc->token_list_start == token_list_1);
    assert(token == token_1);
    assert(tlc__get_current(tlc) == token_1);

    token = tlc__move_to_next(tlc);

    token = tlc__move_to_start(tlc);

    assert(tlc->current_token_list == token_list_1);
    assert(tlc->token_list_start == token_list_1);
    assert(token == token_1);
    assert(tlc__get_current(tlc) == token_1);

    tlc__delete(tlc);

    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_tlc_new_delete();
    test_tlc_current();
    test_tlc_next_prev_start();
    
	return 0;
}
