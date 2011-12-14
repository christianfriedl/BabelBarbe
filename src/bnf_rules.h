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

#ifndef _BNF_RULES_H
#define _BNF_RULES_H

#include"Parser.h"

struct ParserRule_struct;
typedef struct ParserRule_struct ParserRule;

extern ParserRule rule_start;
extern ParserRule rule_rules;
extern ParserRule rule_rule;
extern ParserRule rule_rule_lhs;
extern ParserRule rule_rule_rhs;
extern ParserRule rule_rule_phrase;
extern ParserRule rule_term;
extern ParserRule rule_openparen;
extern ParserRule rule_closeparen;
extern ParserRule rule_repetition_marker;
extern ParserRule rule_symbol;
extern ParserRule rule_keyword_regex;
extern ParserRule rule_literal;
extern ParserRule rule_definition_sign;
extern ParserRule rule_semicolon;
extern ParserRule rule_identifier;
extern ParserRule rule_terminal;
extern ParserRule rule_nonterminal;


bool Parser_parse_openparen(parser_t* parser);
bool Parser_parse_closeparen(parser_t* parser);
bool Parser_parse_repetition_marker(parser_t* parser);
bool Parser_parse_keyword_regex(parser_t* parser);
bool Parser_parse_literal(parser_t* parser);
bool Parser_parse_definition_sign(parser_t* parser);
bool Parser_parse_semicolon(parser_t* parser);
bool Parser_parse_identifier(parser_t* parser);

#endif
