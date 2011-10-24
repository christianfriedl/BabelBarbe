#include"parser.h"
#include"bnf_rules.h"

#define NULL_RULE { repeat_off, 0, { NULL } } 
rule_t rule_start = {
    "rule_start",
    NULL,
    NULL,
    repeat_off, 0,
    {
        { &rule_rules, NULL },
        NULL_RULE
    }
};

rule_t rule_rules = {
    "rule_rules",
    NULL,
    NULL,
    repeat_on, 0,
    {
        { & rule_rule, NULL },
        NULL_RULE
    }
};

rule_t rule_rule = {
    "rule_rule",
    NULL,
    NULL,
    repeat_off, 0,
    {
        { &rule_rule_lhs, &rule_definition_sign, &rule_rule_rhs, &rule_semicolon, NULL },
        NULL_RULE
    }
};

rule_t rule_rule_lhs = {
    "rule_rule_lhs",
    NULL,
    NULL,
    repeat_off, 0,
    {
        { &rule_identifier, NULL },
        NULL_RULE
    }
};

rule_t rule_rule_rhs = {
    "rule_rule_rhs",
    NULL,
    NULL,
    repeat_on, 0,
    {
        { &rule_rule_phrase, NULL },
        NULL_RULE
    }
};

rule_t rule_rule_phrase = {
    "rule_rule_phrase",
    NULL,
    NULL,
    repeat_off, 0,
    {
        { &rule_term, &rule_repetition_marker, NULL },
        { &rule_term, NULL },
        NULL_RULE
};

rule_t rule_term = {
    "rule_term",
    NULL,
    NULL,
    repeat_off, 0,
    {
        { &rule_symbol, NULL },
        { &rule_openparen, &rule_rule_rhs, &rule_closeparen, NULL },
        NULL_RULE
    }
};

rule_t rule_openparen = {
    "rule_openparen",
    parser__parse_openparen,
    NULL,
    repeat_off, 0,
    {
        NULL_RULE
    }
};

rule_t rule_closeparen = {
    "rule_closeparen",
    parser__parse_closeparen,
    NULL,
    repeat_off, 0,
    {
        NULL_RULE
    }
};

rule_t rule_repetition_marker = {
    "rule_repetition_marker",
    parser__parse_repetition_marker,
    NULL,
    repeat_off, 0,
    {
        NULL_RULE
    }
};

rule_t rule_symbol = {
    "rule_symbol",
    NULL,
    NULL,
    repeat_off, 0,
    {
        { &rule_terminal, NULL },
        { &rule_nonterminal, NULL },
        NULL_RULE
    }
};

rule_t rule_terminal = {
    "rule_terminal",
    NULL,
    NULL,
    repeat_off, 0,
    {
        { &rule_keyword_regex, &rule_literal, NULL },
        { &rule_literal, NULL }
    }
};

rule_t rule_keyword_regex = {
    "rule_keyword_regex",
    parser__parse_regex,
    NULL,
    repeat_off, 0,
    {
        NULL_RULE
    }
};

rule_t rule_literal = {
    "rule_literal",
    parser__parse_literal,
    NULL,
    repeat_off, 0,
    {
        NULL_RULE
    }
};
