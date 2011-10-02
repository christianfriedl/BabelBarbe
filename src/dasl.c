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
#include<stdlib.h>
#include<stdio.h>
#include"dasl.h"
#include"tm.h"
#include"parser.h"
#include"parser.h"

dasl_result_t *dasl_result__new(bool is_parsed, tm_t *datetime_val, char *str_val, error_t error, char *error_text) {
    dasl_result_t *dasl_result = malloc(sizeof(*dasl_result));
    if (dasl_result != NULL) {
        dasl_result->is_parsed = is_parsed;
        if (datetime_val != NULL) dasl_result->datetime = tm__copy(datetime_val);
        else dasl_result->datetime = NULL;
        if (str_val != NULL) dasl_result->text = strdup(str_val);
        else dasl_result->text = NULL;
        dasl_result->error = error;
        dasl_result->error_text = error_text;
    } else
        dasl_raise_fatal_error("unable to allocate dasl_result.");
    return dasl_result;
}
void dasl_result__delete(dasl_result_t *dasl_result) {
    if (dasl_result->datetime != NULL) free(dasl_result->datetime);
    if (dasl_result->text != NULL) free(dasl_result->text);
    free(dasl_result);
}
dasl_result_t *dasl__run(const char *text) {
    dasl_result_t *dasl_result = NULL;
    bool is_parsed = false;
    scanner_t *scanner = scanner__new(text);
    parser_t *parser = parser__new(scanner);
    is_parsed = parser__parse(parser);
    if (is_parsed) {
        char *fmt;
        unsigned int len;
        char *result_text;
        result_t *result = parser__remove_last_result(parser);
        if (result->type == rt_date) {
            fmt = "%F";
            len = 12;
        } else if (result->type == rt_datetime) {
            fmt = "%F %T";
            len = 22;
        } else {
            dasl_result = dasl_result__new(false, NULL, NULL, e_generic_error, "unknown return type.");
            parser__delete(parser);
            return dasl_result;
        }

        result_text = malloc(sizeof(char) * len);
        strftime(result_text, len - 1, fmt, result->datetime_val); 
        dasl_result = dasl_result__new(true, result->datetime_val, result_text, parser__get_error(parser), parser__get_error_text(parser));
    } else {
        dasl_result = dasl_result__new(false, NULL, NULL, parser__get_error(parser), parser__get_error_text(parser));
    }
    parser__delete(parser);
    return dasl_result;
}

void dasl_raise_fatal_error(const char *msg) {
    printf("DaSL FATAL ERROR: %s\n", msg);
    abort();
}

char *unit__to_string(unit_t unit) {
    switch(unit) {
        case unit_none:
            return strdup("unit_none");
        case unit_seconds:
            return strdup("unit_seconds");
        case unit_minutes:
            return strdup("unit_minutes");
        case unit_hours:
            return strdup("unit_hours");
        case unit_days:
            return strdup("unit_days");
        case unit_weeks:
            return strdup("unit_weeks");
        case unit_months:
            return strdup("unit_months");
        case unit_years:
            return strdup("unit_years");
        default:
            return strdup("<unknown unit type>");
    }
}
