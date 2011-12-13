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

#ifndef _SCANNER_H
#define _SCANNER_H

#include<ctype.h>
#include<stdlib.h>
#include"bnf.h"
#include"token.h"

typedef enum { s_initial = 0 } scanner_state_t;

typedef struct { 
    char *text;
    char *text_start;
    token_t *token;
    scanner_state_t state;
    error_t error;
} scanner_t;

struct scanner_table_cell_s;

struct scanner_table_cell_s {
    scanner_state_t next_state;
    bool is_final;
    bool (*recognize)(char ch);
    unsigned int (*consume)(scanner_t *this);
};
typedef struct scanner_table_cell_s scanner_table_cell_t;

#define SCANNER_TABLE_ROWS 20
#define SCANNER_TABLE_COLS 20

scanner_table_cell_t scanner_table[SCANNER_TABLE_ROWS][SCANNER_TABLE_COLS];


scanner_t *scanner__new(const char *text);
void scanner__delete(scanner_t *this);
bool scanner__scan(scanner_t *this);
void scanner__raise_error(scanner_t *this, error_t error);
void scanner__raise_fatal_error(const char *msg) __attribute((noreturn));



#endif
