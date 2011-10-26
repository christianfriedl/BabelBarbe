#ifndef _BNF_RULES_H
#define _BNF_RULES_H

extern rule_t rule_start;
extern rule_t rule_rules;
extern rule_t rule_rule;
extern rule_t rule_rule_lhs;
extern rule_t rule_rule_rhs;
extern rule_t rule_rule_phrase;
extern rule_t rule_term;
extern rule_t rule_openparen;
extern rule_t rule_closeparen;
extern rule_t rule_repetition_marker;
extern rule_t rule_symbol;
extern rule_t rule_keyword_regex;
extern rule_t rule_literal;
extern rule_t rule_definition_sign;
extern rule_t rule_semicolon;
extern rule_t rule_identifier;
extern rule_t rule_terminal;
extern rule_t rule_nonterminal;


bool parser__parse_openparen(parser_t *parser);
bool parser__parse_closeparen(parser_t *parser);
bool parser__parse_repetition_marker(parser_t *parser);
bool parser__parse_keyword_regex(parser_t *parser);
bool parser__parse_literal(parser_t *parser);
bool parser__parse_definition_sign(parser_t *parser);
bool parser__parse_semicolon(parser_t *parser);
bool parser__parse_identifier(parser_t *parser);

#endif
