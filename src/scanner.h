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

#ifndef _SCANNER_H
#define _SCANNER_H

#include<ctype.h>
#include<stdlib.h>
#include"bnf.h"
#include"token.h"

typedef enum { s_initial, s_reset, s_alpha, s_digit, s_single_character_symbol, s_double_character_symbol, s_quote } scanner_state_t;

typedef struct { 
    char *text;
    char *text_start;
    token_t *token;
    scanner_state_t state;
    error_t error;
} scanner_t;

scanner_t *scanner__new(const char *text);
void scanner__delete(scanner_t *scanner);
bool scanner__scan(scanner_t *scanner);
void scanner__raise_error(scanner_t *scanner, error_t error);
void scanner__raise_fatal_error(const char *msg) __attribute((noreturn));



#endif
