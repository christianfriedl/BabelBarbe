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

#ifndef _RESULTS_H
#define _RESULTS_H

#include<time.h>
#include"dasl.h"
#include"tm.h"

#define MAX_RESULTS 10

typedef enum { rt_none = 0, rt_uint = 1, rt_date = 2, rt_datetime = 3, rt_unit = 4, rt_operator = 5, rt_type = 6, rt_identifier = 7 } result_type_t;
typedef enum { type_uint = 0, type_date = 1, type_datetime = 2 } type_t;

typedef struct {
    result_type_t type;
    unsigned int uint_val;
    unit_t unit_val;
    tm_t *datetime_val;
    error_t error;
    char string_val[256];
    type_t type_val;
} result_t;

typedef struct {
    result_t *results[MAX_RESULTS];
    unsigned int count_results;
} result_list_t;

result_t *result__new();
result_t *result__new_uint(const unsigned int uint_val);
result_t *result__new_operator(const char *src);
result_t *result__new_identifier(const char *src);
result_t *result__new_date(const unsigned int year, const unsigned int month, const unsigned int day);
result_t *result__new_from_current_date();
result_t *result__new_datetime(const unsigned int year, const unsigned int month, const unsigned int day, const unsigned int hour, const unsigned int minute, const unsigned int second);
result_t *result__new_from_current_datetime();
result_t *result__new_unit(unit_t unit_val);
result_t *result__new_type(type_t type_val);
result_t *result__new_error(error_t error);
void result__delete(result_t *result);
result_list_t *result_list__new();
void result_list__delete(result_list_t *result_list);
void result_list__add_result(result_list_t *result_list, result_t *result);
result_t *result_list__get_result_by_index(result_list_t *result_list, const unsigned index);
result_t *result_list__remove_result_by_index(result_list_t *result_list, const unsigned int index);
void result_list__prune_to_count(result_list_t *result_list, const unsigned int count);
void result_list__clear(result_list_t *result_list);
unsigned int result_list__get_count(result_list_t *result_list);
result_t *add_results(result_t *result1, result_t *result2);
result_t *subtract_results(result_t *result1, result_t *result2);
char *result__type_to_string(result_type_t type);
void result__print(result_t *result);

void tm__delete(tm_t *tm);


#endif
