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

#include"BNF.h"
#include"Scanner.h"
#include"TokenList.h"
#include"TLC.h"

struct parser_s;
typedef struct Parser_struct Parser;
struct ParserRule;
#include"bnf_rules.h"

typedef enum { Repeat_on=0, Repeat_off } RepeatSwitch;

typedef struct {
    RepeatSwitch repeat_switch;
    unsigned int repeat_from_index; /* index into production */
    struct ParserRule* productions[20];
} alternative_t;

struct ParserRule {
    char name[255];
    bool (*parse)(Parser* parser);
    void (*evaluate)(Parser* parser);
    alternative_t alternatives[20];
};

struct Parser_struct {
    Error error;
    char error_text[255];
    char error_texts[255][255];
    Scanner* scanner;
    TokenList* token_list;
    TLC* tlc;
    bool is_debug;
    ParserRule* start_rule;
    ASTLeaf* ast_root;
}; 

Parser* Parser__new(Scanner* scanner, ParserRule* start_rule);
void Parser_delete(Parser* parser);
void Parser_debug(Parser* parser);
bool Parser_parse(Parser* parser);
bool Parser_isError(Parser* parser);
Error Parser_getError(Parser* parser);
char* Parser_getErrorText(Parser* parser);
#endif
