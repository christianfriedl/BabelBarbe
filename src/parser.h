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

#ifndef _PARSER_H
#define _PARSER_H

#include"bnf.h"
#include"scanner.h"
#include"token_list.h"
#include"tlc.h"

struct parser_s;
typedef struct parser_s parser_t;
struct parser_rule;
#include"bnf_rules.h"

typedef enum { repeat_on=0, repeat_off } repeat_switch_t;

typedef struct {
    repeat_switch_t repeat_switch;
    unsigned int repeat_from_index; /* index into production */
    struct parser_rule *productions[20];
} alternative_t;

struct parser_rule {
    char name[255];
    bool (*parse)(parser_t *parser);
    void (*evaluate)(parser_t *parser);
    alternative_t alternatives[20];
};

struct parser_s {
    error_t error;
    char error_text[255];
    char error_texts[255][255];
    scanner_t *scanner;
    token_list_t *token_list;
    tlc_t *tlc;
    bool is_debug;
    parser_rule_t *start_rule;
}; 

parser_t *parser__new(scanner_t *scanner, parser_rule_t *start_rule);
void parser__delete(parser_t *parser);
void parser__debug(parser_t *parser);
bool parser__parse(parser_t *parser);
bool parser__is_error(parser_t *parser);
error_t parser__get_error(parser_t *parser);
char *parser__get_error_text(parser_t *parser);
#endif
