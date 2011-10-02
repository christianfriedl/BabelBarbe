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

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"dasl.h"
#include"results.h"
#include"tm.h"

void result__raise_error(result_t *result, error_t error);

result_t *result__new() {
    result_t *result = malloc(sizeof(*result));
    if (result != NULL) {
        result->type = rt_none;
        result->uint_val = 0;
        result->unit_val = unit_none;
        result->string_val[0] = '\0';
        result->datetime_val = NULL;
        result->error = e_ok;
    } else
        dasl_raise_fatal_error("Unable to allocate result.");
    return result;
}
result_t *result__new_uint(const unsigned int uint_val) {
    result_t *result = result__new();
    if (result != NULL) {
        result->type = rt_uint;
        result->uint_val = uint_val;
    } else
        dasl_raise_fatal_error("Unable to allocate result.");
    return result;
}
result_t *result__new_operator(const char *src) {
    result_t *result = result__new();
    if (result != NULL) {
        result->type = rt_operator;
        strcpy(result->string_val, src);
    } else
        dasl_raise_fatal_error("Unable to allocate result.");
    return result;
}
result_t *result__new_identifier(const char *src) {
    result_t *result = result__new();
    if (result != NULL) {
        result->type = rt_identifier;
        strcpy(result->string_val, src);
    } else
        dasl_raise_fatal_error("Unable to allocate result.");
    return result;
}
result_t *result__new_date(const unsigned int year, const unsigned int month, const unsigned int day) {
    result_t *result = result__new();
    if (result != NULL) {
        result->type = rt_date;
        result->datetime_val = tm__new(year, month, day, 0, 0, 0);
    } else
        dasl_raise_fatal_error("Unable to allocate result.");
    return result;
}
result_t *result__new_datetime(const unsigned int year, const unsigned int month, const unsigned int day, const unsigned int hour, const unsigned int minute, const unsigned int second) {
    result_t *result = result__new();
    if (result != NULL) {
        result->type = rt_datetime;
        result->datetime_val = tm__new(year, month, day, hour, minute, second);
    } else
        dasl_raise_fatal_error("Unable to allocate result.");
    return result;
}
result_t *result__new_datetime_from_tm(tm_t *_time) { 
    return result__new_datetime(_time->tm_year + 1900, _time->tm_mon + 1, _time->tm_mday, _time->tm_hour, _time->tm_min, _time->tm_sec);
}
result_t *result__new_date_from_tm(tm_t *_time) { 
    return result__new_date(_time->tm_year + 1900, _time->tm_mon + 1, _time->tm_mday);
}
result_t *result__new_from_current_datetime() {
    result_t *result = result__new();
    if (result != NULL) {
        result->type = rt_datetime;
        result->datetime_val = tm__new_from_current_time();
    } else
        dasl_raise_fatal_error("Unable to allocate result.");
    return result;
}
result_t *result__new_from_current_date() {
    result_t *result = result__new();
    if (result != NULL) {
        result->type = rt_date;
        result->datetime_val = tm__new_from_current_time();
    } else
        dasl_raise_fatal_error("Unable to allocate result.");
    return result;
}
result_t *result__new_unit(unit_t unit_val) {
    result_t *result = result__new();
    if (result != NULL) {
        result->type = rt_unit;
        result->unit_val = unit_val;
    } else
        dasl_raise_fatal_error("Unable to allocate result.");
    return result;
}
result_t *result__new_type(type_t type_val) {
    result_t *result = result__new();
    if (result != NULL) {
        result->type = rt_type;
        result->type_val = type_val;
    } else
        dasl_raise_fatal_error("Unable to allocate result.");
    return result;
}
result_t *result__new_error(error_t error) {
    result_t *result = result__new();
    if (result != NULL) {
        result->type = rt_none;
        result->error = error;
    } else
        dasl_raise_fatal_error("Unable to allocate result.");
    return result;
}


void result__delete(result_t *result) {
    if (result->datetime_val != NULL)
        tm__delete(result->datetime_val);
    free(result);
}

void result__raise_error(result_t *result, error_t error) {
    result->error = error;
}


result_list_t *result_list__new() {
    result_list_t *result_list = malloc(sizeof(*result_list));
    if (result_list != NULL) {
        unsigned int i;
        for (i=0; i < MAX_RESULTS; ++i)
            result_list->results[i] = NULL;
        result_list->count_results = 0;
    } else
        dasl_raise_fatal_error("Unable to allocate result list.");
    return result_list;
}

void result_list__delete(result_list_t *result_list) {
    unsigned int i;
    for (i=0; i < result_list->count_results; ++i)
        result__delete(result_list->results[i]);
    free(result_list);
}
void result_list__add_result(result_list_t *result_list, result_t *result) {
    if (result_list->count_results < MAX_RESULTS)
        result_list->results[(result_list->count_results)++] = result;
    else 
        dasl_raise_fatal_error("Cannot add more than MAX_RESULTS results.");
}
result_t *result_list__get_result_by_index(result_list_t *result_list, const unsigned index) {
    if (index < result_list->count_results)
        return result_list->results[index];
    else
        dasl_raise_fatal_error("Result index out of bounds.");
    return NULL; // make gcc happy
}

void result_list__clear(result_list_t *result_list) {
    unsigned int i;
    for (i = 0; i < result_list->count_results; ++i) {
        result__delete(result_list->results[i]);
        result_list->results[i] = NULL;
    }
    result_list->count_results = 0;
}
result_t *result_list__remove_result_by_index(result_list_t *result_list, const unsigned int index) {
    result_t *result = NULL;
    if (index < result_list->count_results) {
        unsigned int i;
        result = result_list__get_result_by_index(result_list, index);
        for (i=index; i < result_list->count_results - 1; ++i)
            result_list->results[i] = result_list->results[i+1];
        result_list->results[result_list->count_results - 1] = NULL;
        --(result_list->count_results);
    } else
        dasl_raise_fatal_error("Result index out of bounds.");
    return result;
}

unsigned int result_list__get_count(result_list_t *result_list) {
    return result_list->count_results;
}

void result_list__prune_to_count(result_list_t *result_list, const unsigned int count) {
    while (result_list__get_count(result_list) > count)
        result_list__remove_result_by_index(result_list, result_list__get_count(result_list)-1);
}
/* evaluating results */

void swap_results(result_t **result1, result_t **result2) { 
    result_t *tmp = *result1;
    *result1 = *result2;
    *result2 = tmp;
}

result_t *add_results(result_t *result1, result_t *result2) { 
    result_t *result = NULL;
    if (result2->type == rt_date || result2->type == rt_datetime)
        swap_results(&result1, &result2);

    switch(result1->type) {
        case rt_date:
            switch(result2->type) {
                case rt_unit:
                    if (result2->unit_val == unit_years || 
                            result2->unit_val == unit_months || 
                            result2->unit_val == unit_weeks || 
                            result2->unit_val == unit_days) {
                        return result__new_date_from_tm(tm__add_interval(result1->datetime_val, result2->unit_val, result2->uint_val));
                    } else {
                        result = result__new();
                        result__raise_error(result, e_cannot_add_unit_to_date);
                        return result;
                    }
                    break;
                default:
                    result = result__new();
                    result__raise_error(result, e_cannot_add_types);
                    return result;
            }
            break;
        case rt_datetime:
            switch(result2->type) {
                case rt_unit:
                    return result__new_datetime_from_tm(tm__add_interval(result1->datetime_val, result2->unit_val, result2->uint_val));
                    break;
                default:
                    result = result__new();
                    result__raise_error(result, e_cannot_add_types);
                    return result;
            }
            break;
        default:
            result = result__new();
            result__raise_error(result, e_cannot_add_types);
            return result;
    }
    dasl_raise_fatal_error("Error in add_results.");
    return NULL;
}

result_t *subtract_results(result_t *result1, result_t *result2) { 
    result_t *result = NULL;
    if (result2->type == rt_date || result2->type == rt_datetime)
        swap_results(&result1, &result2);

    switch(result1->type) {
        case rt_date:
            switch(result2->type) {
                case rt_unit:
                    if (result2->unit_val == unit_years || 
                            result2->unit_val == unit_months || 
                            result2->unit_val == unit_weeks || 
                            result2->unit_val == unit_days) {
                        return result__new_datetime_from_tm(tm__subtract_interval(result1->datetime_val, result2->unit_val, result2->uint_val));
                    } else {
                        result = result__new();
                        result__raise_error(result, e_cannot_subtract_unit_from_date);
                        return result;
                    }
                    break;
                default:
                    result = result__new();
                    result__raise_error(result, e_cannot_subtract_types);
                    return result;
            }
            break;
        case rt_datetime:
            switch(result2->type) {
                case rt_unit:
                    return result__new_datetime_from_tm(tm__subtract_interval(result1->datetime_val, result2->unit_val, result2->uint_val));
                    break;
                default:
                    result = result__new();
                    result__raise_error(result, e_cannot_subtract_types);
                    return result;
            }
            break;
        default:
            result = result__new();
            result__raise_error(result, e_cannot_subtract_types);
            return result;
    }
    dasl_raise_fatal_error("Error in subtract_results.");
    return NULL;
}

char *result__type_to_string(result_type_t type) {
    switch (type) {
        case rt_none:
            return strdup("rt_none");
        case rt_uint:
            return strdup("rt_uint");
        case rt_date:
            return strdup("rt_date");
        case rt_datetime:
            return strdup("rt_datetime");
        case rt_unit:
            return strdup("rt_unit");
        case rt_operator:
            return strdup("rt_operator");
        case rt_identifier:
            return strdup("rt_identifier");
        default:
            return strdup("<unknown result type>");
   } 
}
void result__print(result_t *result) {
    char *unit_val_string = unit__to_string(result->unit_val);
    char *result_type_string = result__type_to_string(result->type);
    printf("result @%ld: type '%s', uint_val '%u', datetime_val '%s', unit_val '%s'\n", 
        (long unsigned int)result, result_type_string, result->uint_val, result->datetime_val != NULL? asctime(result->datetime_val) : "NULL", unit_val_string);
    free(result_type_string);
}

