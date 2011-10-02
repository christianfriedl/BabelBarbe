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
#include"../parser.h"
#include"../results.h"

/*
    positive tests
*/
void test_parser_new_delete() {
    char *text = "";
    scanner_t *scanner; 
    parser_t *parser;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    parser__delete(parser);
    printf("ok\n");
}

void test_parser_uint() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text = "123";
    bool rv;
    result_t *result = NULL;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    assert(parser->token_list != NULL);
    assert(result->type == rt_uint);
    assert(result->uint_val == 123);
    assert(parser->token_list->token->type == t_literal_uint);
    assert(!strcmp(parser->token_list->token->text, "123"));
    parser__delete(parser);
    printf("ok\n");
}


void test_parser_date_literal() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text = "'2011-01-01'";
    bool rv;
    result_t *result = NULL;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(rv == true);
    assert(result != NULL);
    assert(parser->token_list != NULL);
    assert(result->type == rt_date);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011, 1, 1, 0, 0, 0));
    parser__delete(parser);

    text = "'1985-02-28'";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(rv == true);
    assert(result != NULL);
    assert(parser->token_list != NULL);
    assert(result->type == rt_date);
    assert(tm__to_timestamp(result->datetime_val) == maketime(1985, 2, 28, 0, 0, 0));
    parser__delete(parser);

    printf("ok\n");
}

void test_parser_datetime_literal() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text = "'2011-01-01 00:00:00'";
    bool rv;
    result_t *result = NULL;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(rv == true);
    assert(result != NULL);
    assert(parser->token_list != NULL);
    assert(result->type == rt_datetime);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011, 1, 1, 0, 0, 0));
    parser__delete(parser);

    text = "'2011-01-01 00:00:01'";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(rv == true);
    assert(result != NULL);
    assert(parser->token_list != NULL);
    assert(result->type == rt_datetime);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011, 1, 1, 0, 0, 1));
    parser__delete(parser);

    text = "'2011-05-30 20:12:30'";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(rv == true);
    assert(result != NULL);
    assert(parser->token_list != NULL);
    assert(result->type == rt_datetime);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011, 5, 30, 20, 12, 30));
    parser__delete(parser);

    printf("ok\n");
}

void test_parser_functions() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text = "TODAY";
    char *text2 = "NOW";
    bool rv;
    time_t systime;
    result_t *result = NULL;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    assert(parser->token_list != NULL);
    assert(result->type == rt_date);
    assert(parser->token_list->token->type == t_identifier);
    assert(!strcmp(parser->token_list->token->text, "TODAY"));
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == mktime(localtime(&systime)));
    parser__delete(parser);

    scanner = scanner__new(text2);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    assert(parser->token_list != NULL);
    assert(result->type == rt_datetime);
    assert(parser->token_list->token->type == t_identifier);
    assert(!strcmp(parser->token_list->token->text, "NOW"));
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == mktime(localtime(&systime)));
    parser__delete(parser);

    printf("ok\n");
}

void test_parser_interval_expression() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text = "1 DAY";
    bool rv;
    result_t *result = NULL;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    assert(result->type == rt_unit);
    assert(result->uint_val == 1);
    assert(result->unit_val == unit_days);
    parser__delete(parser);

    text = "2 DAYS";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    assert(result->type == rt_unit);
    assert(result->uint_val == 2);
    assert(result->unit_val == unit_days);
    parser__delete(parser);

    text = "512 WEEKS";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    assert(result->type == rt_unit);
    assert(result->uint_val == 512);
    assert(result->unit_val == unit_weeks);
    parser__delete(parser);

    printf("ok\n");
}


/*
    negative tests
*/
void test_parser_negative_empty_text() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text = "   ";
    bool rv;
    printf("%s...\n", __func__);
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser->scanner->token == NULL);
    parser__delete(parser);
    printf("ok\n");
}
void test_parser_negative_wrong_symbols() {
    char *text; 
    parser_t *parser;
    scanner_t *scanner;
    bool rv;
    printf("%s...\n", __func__);

    text = "*";
    scanner = scanner__new(text);
    rv = scanner__scan(scanner);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_unexpected_symbol);
    parser__delete(parser);

    text = "/";
    scanner = scanner__new(text);
    rv = scanner__scan(scanner);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_unexpected_symbol);
    parser__delete(parser);

    printf("ok\n");
}

void test_parser_negative_wrong_types() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text;
    bool rv;
    printf("%s...\n", __func__);

    text = "'2011-01-02' + 1 SECOND";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_add_unit_to_date);
    parser__delete(parser);

    text = "'2011-01-02' + 1 MINUTE";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_add_unit_to_date);
    parser__delete(parser);

    text = "'2011-01-02' + 1 HOUR";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_add_unit_to_date);
    parser__delete(parser);

    text = "'2011-01-02' + 1 HOURS";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_add_unit_to_date);
    parser__delete(parser);

    text = "'2011-01-02' + 20 HOUR";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_add_unit_to_date);
    parser__delete(parser);

    text = "'2011-01-02' + 10";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_add_types);
    parser__delete(parser);

    text = "'2011-01-02' + '2011-03-04'";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_add_types);
    parser__delete(parser);

    text = "'2011-01-02 10:02:12' + '2011-03-04'";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_add_types);
    parser__delete(parser);

    text = "'2011-01-02' - 1 SECOND";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_subtract_unit_from_date);
    parser__delete(parser);

    text = "'2011-01-02' - 1 MINUTE";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_subtract_unit_from_date);
    parser__delete(parser);

    text = "'2011-01-02' - 1 HOUR";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_subtract_unit_from_date);
    parser__delete(parser);

    text = "'2011-01-02' - 1 HOURS";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_subtract_unit_from_date);
    parser__delete(parser);

    text = "'2011-01-02' - 20 HOUR";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_subtract_unit_from_date);
    parser__delete(parser);

    text = "'2011-01-02' - 10";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_subtract_types);
    parser__delete(parser);

    text = "'2011-01-02' - '2011-03-04'";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_subtract_types);
    parser__delete(parser);

    text = "'2011-01-02 10:02:12' - '2011-03-04'";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser__is_error(parser) == true);
    assert(parser__get_error(parser) == e_cannot_subtract_types);
    parser__delete(parser);

    printf("ok\n");
}


int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_parser_new_delete();
    test_parser_uint();
    test_parser_datetime_literal();
    test_parser_date_literal();
    test_parser_functions();
    test_parser_interval_expression();

    /*
        negative tests
    */

    test_parser_negative_empty_text();
    test_parser_negative_wrong_symbols();
    test_parser_negative_wrong_types();
	return 0;
}
