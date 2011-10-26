#ifndef _BNF_RULES_H
#define _BNF_RULES_H

#include"parser.h"

struct parser_rule;
typedef struct parser_rule parser_rule_t;

extern parser_rule_t rule_start;
extern parser_rule_t rule_rules;
extern parser_rule_t rule_rule;
extern parser_rule_t rule_rule_lhs;
extern parser_rule_t rule_rule_rhs;
extern parser_rule_t rule_rule_phrase;
extern parser_rule_t rule_term;
extern parser_rule_t rule_openparen;
extern parser_rule_t rule_closeparen;
extern parser_rule_t rule_repetition_marker;
extern parser_rule_t rule_symbol;
extern parser_rule_t rule_keyword_regex;
extern parser_rule_t rule_literal;
extern parser_rule_t rule_definition_sign;
extern parser_rule_t rule_semicolon;
extern parser_rule_t rule_identifier;
extern parser_rule_t rule_terminal;
extern parser_rule_t rule_nonterminal;


bool parser__parse_openparen(parser_t *parser);
bool parser__parse_closeparen(parser_t *parser);
bool parser__parse_repetition_marker(parser_t *parser);
bool parser__parse_keyword_regex(parser_t *parser);
bool parser__parse_literal(parser_t *parser);
bool parser__parse_definition_sign(parser_t *parser);
bool parser__parse_semicolon(parser_t *parser);
bool parser__parse_identifier(parser_t *parser);

#endif
