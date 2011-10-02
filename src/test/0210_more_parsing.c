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

void test_parser_arithmetics() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text = "'2011-01-01 00:00:00' + 1 HOUR";
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
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,1,1,0,0));
    parser__delete(parser);

    text = "'2011-01-01' + 1 DAY";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,2,0,0,0));
    parser__delete(parser);

    text = "'2011-01-02 00:00:00' - 1 HOUR";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,1,23,0,0));
    parser__delete(parser);

    text = "'2011-01-01' - 1 DAY";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2010,12,31,0,0,0));
    parser__delete(parser);

    printf("ok\n");

}

void test_parser_multi_terms() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text; 
    bool rv;
    time_t systime;
    result_t *result = NULL;
    printf("%s...\n", __func__);

    text = "'2011-01-01 00:00:00' + 1 HOUR + 1 DAY";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    assert(result_list__get_count(parser->result_list) == 1);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,2,1,0,0));
    parser__delete(parser);
    
    text = "'2011-01-01 00:00:00' + 23 HOURS + 59 MINUTES + 59 SECONDS";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    assert(result_list__get_count(parser->result_list) == 1);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,1,23,59,59));
    parser__delete(parser);

    text = "'2011-01-01 00:00:00' + 1 DAY - 1 HOUR";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    assert(result_list__get_count(parser->result_list) == 1);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,1,23,0,0));
    parser__delete(parser);

    printf("ok\n");
}
void test_parser_parens() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text; 
    bool rv;
    time_t systime;
    result_t *result = NULL;
    printf("%s...\n", __func__);

    text = "('2011-01-01 00:00:00' + 1 HOUR) + 1 DAY";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,2,1,0,0));
    parser__delete(parser);

    text = "(('2011-01-01 00:00:00' + 1 HOUR) + 1 DAY) + 10 SECOND";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,2,1,0,10));
    parser__delete(parser);

    text = "((((('2011-01-01 00:00:00' + 1 HOUR) + 1 DAY) + 10 SECOND)))";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    systime = time(NULL);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,2,1,0,10));
    parser__delete(parser);

    printf("ok\n");
}

void test_parser_typed_expressions() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text; 
    bool rv;
    result_t *result = NULL;
    printf("%s...\n", __func__);

    text = "'2011-01-01 00:00:00'::date";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    assert(result->type == rt_date);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,1,0,0,0));
    parser__delete(parser);

    text = "'2011-01-01'::datetime";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    assert(result->type == rt_datetime);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,1,0,0,0));
    parser__delete(parser);

    printf("ok\n");
}

void test_parser_more_complex_typed_expressions() {
    scanner_t *scanner; 
    parser_t *parser;
    result_t *result;
    char *text; 
    bool rv;
    printf("%s...\n", __func__);

    text = "'2011-01-01 00:00:00'::date + 1 DAY";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    assert(result->type == rt_date);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,2,0,0,0));
    parser__delete(parser);

    text = "('2011-01-01 00:00:00'::date + 1 DAY)::datetime + 1 HOUR";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    assert(result->type == rt_datetime);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,2,1,0,0));
    parser__delete(parser);

    printf("ok\n");
}
void test_parser_dotfunction_expressions() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text = NULL;
    bool rv;
    result_t *result = NULL;
    printf("%s...\n", __func__);

    text = "'2011-01-01'.END(MONTH)";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    assert(result->type == rt_date);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011,1,31,0,0,0));
    printf("%s", asctime(result->datetime_val));
    parser__delete(parser);

    text = "(('2011-05-23 14:33:12' + 1 YEAR).BEGIN(YEAR) + 1 MONTH).END(MONTH)";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == true);
    result = result_list__get_result_by_index(parser->result_list, 0);
    assert(result != NULL);
    printf("%s", asctime(result->datetime_val));
    assert(result->type == rt_datetime);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2012,2,29,23,59,59));
    parser__delete(parser);

    printf("ok\n");
}

/*
    negative tests
*/

void test_parser_negative_typed_expressions() {
    scanner_t *scanner; 
    parser_t *parser;
    char *text; 
    bool rv;
    printf("%s...\n", __func__);

    text = "'2011-01-01 00:00:00'::datetime";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser->error == e_cannot_cast_to_datetime);
    printf("%s\n", parser->error_text);
    parser__delete(parser);

    text = "'2011-01-01'::date";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser->error == e_cannot_cast_to_date);
    printf("%s\n", parser->error_text);
    parser__delete(parser);

    text = "'2011-01-01'::uint";
    scanner = scanner__new(text);
    parser = parser__new(scanner);
    rv = parser__parse(parser);
    assert(rv == false);
    assert(parser->error == e_cannot_cast_to_uint);
    printf("%s\n", parser->error_text);
    parser__delete(parser);

    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_parser_arithmetics();
    test_parser_multi_terms();
    test_parser_parens();
    test_parser_typed_expressions();
    test_parser_more_complex_typed_expressions();
    test_parser_dotfunction_expressions();

    /*
        negative tests
    */
    test_parser_negative_typed_expressions();

	return 0;
}
