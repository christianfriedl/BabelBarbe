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

#include"tm.h"

#ifndef _DASL_H
#define _DASL_H

#define bool int
#define true (-1)
#define false (0)

#define bool__to_string(a) (((a) == true) ? "true" : "false")

typedef enum { 
    e_ok, 
    e_generic_error,
    e_unexpected_eof, e_unknown_scanner_state, e_initial_or_reset_expected,  e_unexpected_symbol, /* scanner errors */
    e_cannot_add_unit_to_date, e_cannot_add_types, e_cannot_subtract_unit_from_date, e_cannot_subtract_types, e_cannot_cast_to_date, e_cannot_cast_to_datetime,
        e_cannot_cast_to_uint /* result errors */
} error_t;
typedef enum { unit_none, unit_seconds, unit_minutes, unit_hours, unit_days, unit_weeks, unit_months, unit_years } unit_t;
typedef struct tm tm_t;
typedef struct {
    bool is_parsed;
    tm_t *datetime;
    char *text;
    error_t error;
    char *error_text;
} dasl_result_t;

dasl_result_t *dasl_result__new(bool is_parsed, tm_t *datetime_val, char *str_val, error_t error, char *error_text);
void dasl_result__delete(dasl_result_t *dasl_result);
dasl_result_t *dasl__run(const char *text);
void dasl_raise_fatal_error(const char *msg) __attribute((noreturn));
char *unit__to_string(unit_t unit);
char *dasl__as_string(const char *text);
tm_t *dasl__as_tm(const char *text);

#endif
