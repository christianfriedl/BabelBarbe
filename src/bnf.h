/*
BNF Parser

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

#ifndef _BNF_H
#define _BNF_H

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

void bnf_raise_fatal_error(const char *msg) __attribute((noreturn));

#endif
