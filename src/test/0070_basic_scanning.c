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
#include<string.h>
#include"../scanner.h"

/*
    tests that should scan ok
*/
void test_scanner_new_delete() {
    char *text = "";
    printf("%s...\n", __func__);
    scanner_t *scanner = scanner__new(text);
    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_uint() {
    char *text = "123";
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_literal_uint);
    assert(scanner->text == text+3);
    assert(atoi(scanner->token->text) == 123);
    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_date_literal() {
    char *text = "'2011-01-02'";
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_quote);
    assert(scanner->text == text+1);
    assert(!strcmp(scanner->token->text, "'"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_literal_uint);
    assert(scanner->text == text+5);
    assert(!strcmp(scanner->token->text, "2011"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == s_single_character_symbol);
    assert(scanner->text == text+6);
    assert(!strcmp(scanner->token->text, "-"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_literal_uint);
    assert(scanner->text == text+8);
    assert(!strcmp(scanner->token->text, "01"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == s_single_character_symbol);
    assert(scanner->text == text+9);
    assert(!strcmp(scanner->token->text, "-"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_quote);
    assert(scanner->token->type == t_literal_uint);
    assert(scanner->text == text+11);
    assert(!strcmp(scanner->token->text, "02"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_quote);
    assert(scanner->text == text+12);
    assert(!strcmp(scanner->token->text, "'"));
    scanner__delete(scanner);
    printf("ok\n");
}
void test_scanner_datetime_literal() {
    char *text = "'2011-01-02 23:50:51'";
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, "'"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, "2011"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, "-"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, "01"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, "-"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, "02"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, "23"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, ":"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, "50"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, ":"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, "51"));
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(!strcmp(scanner->token->text, "'"));
    scanner__delete(scanner);
    printf("ok\n");
}


void test_scanner_uints() {
    char *text;
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    text = "123 9876";
    scanner = scanner__new(text);
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_literal_uint);
    assert(scanner->text == text+3);
    assert(!strcmp(scanner->token->text, "123"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_literal_uint);
    assert(scanner->text == text+8);
    assert(!strcmp(scanner->token->text, "9876"));
    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_uints_and_symbols() {
    char *text;
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    text = "123::9876:1";
    scanner = scanner__new(text);

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_literal_uint);
    assert(scanner->text == text+3);
    assert(!strcmp(scanner->token->text, "123"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_symbol);
    assert(scanner->text == text+5);
    assert(!strcmp(scanner->token->text, "::"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_literal_uint);
    assert(scanner->text == text+9);
    assert(!strcmp(scanner->token->text, "9876"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_symbol);
    assert(scanner->text == text+10);
    assert(!strcmp(scanner->token->text, ":"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_literal_uint);
    assert(scanner->text == text+11);
    assert(!strcmp(scanner->token->text, "1"));

    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_uints_and_symbols2() {
    char *text;
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    text = "123.9876 :: 8 . 44::7";
    scanner = scanner__new(text);

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "123"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "."));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "9876"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "::"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "8"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "."));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "44"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "::"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "7"));

    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_uints_and_quotes() {
    char *text;
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    text = "'123' 45 '789'";
    scanner = scanner__new(text);

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_quote);
    assert(!strcmp(scanner->token->text, "'"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "123"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_quote);
    assert(!strcmp(scanner->token->text, "'"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "45"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_quote);
    assert(!strcmp(scanner->token->text, "'"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "789"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_quote);
    assert(!strcmp(scanner->token->text, "'"));

    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_uints_and_symbols3() {
    char *text;
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    text = "(123 + 456) :: 78 - 9";
    scanner = scanner__new(text);

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "("));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "123"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "+"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "456"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, ")"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "::"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "78"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "-"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "9"));

    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_identifier() {
    char *text = "abcde";
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_identifier);
    assert(!strcmp(scanner->token->text, "abcde"));
    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_identifier_with_parens() {
    char *text = "abcde()";
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_identifier);
    assert(!strcmp(scanner->token->text, "abcde"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "("));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, ")"));

    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_identifiers() {
    char *text = "abcde fgh";
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_identifier);
    assert(!strcmp(scanner->token->text, "abcde"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_identifier);
    assert(!strcmp(scanner->token->text, "fgh"));

    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_single_character_symbols() {
    char *text = "12.()+-";
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    
    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_literal_uint);
    assert(!strcmp(scanner->token->text, "12"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "."));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "("));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, ")"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "+"));

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->state == s_reset);
    assert(scanner->token->type == t_symbol);
    assert(!strcmp(scanner->token->text, "-"));

    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_eof() {
    char *text = "abcde fgh";
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    
    rv = scanner__scan(scanner);
    assert(rv == true);

    rv = scanner__scan(scanner);
    assert(rv == true);

    rv = scanner__scan(scanner);
    assert(rv == true);
    assert(scanner->error == e_ok);
    assert(scanner->token->type == t_eof);

    scanner__delete(scanner);
    printf("ok\n");
}

/*
    tests that should scan as an error
*/

void test_scanner_negative_only_spaces() {
    char *text = "   ";
    printf("%s...\n", __func__);
    scanner_t *scanner = scanner__new(text);
    bool rv = scanner__scan(scanner);
    assert(rv == false && scanner->error == e_unexpected_eof);
    scanner__delete(scanner);
    printf("ok\n");
}

void test_scanner_negative_wrong_symbols() {
    char *text; 
    printf("%s...\n", __func__);

    text = "*";
    scanner_t *scanner = scanner__new(text);
    bool rv = scanner__scan(scanner);
    assert(rv == false && scanner->error == e_unexpected_symbol);
    scanner__delete(scanner);

    text = "/";
    scanner = scanner__new(text);
    rv = scanner__scan(scanner);
    assert(rv == false && scanner->error == e_unexpected_symbol);
    scanner__delete(scanner);
    printf("ok\n");
}



int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_scanner_new_delete();
    test_scanner_uint();
    test_scanner_date_literal();
    test_scanner_datetime_literal();
    test_scanner_uints();
    test_scanner_single_character_symbols();
    test_scanner_uints_and_symbols();
    test_scanner_uints_and_symbols2();
    test_scanner_uints_and_symbols3();
    test_scanner_uints_and_quotes();
    test_scanner_identifier();
    test_scanner_identifier_with_parens();
    test_scanner_identifiers();
    test_scanner_eof();

    /*
        negative tests
    */

    test_scanner_negative_only_spaces();
    test_scanner_negative_wrong_symbols();
	return 0;
}
