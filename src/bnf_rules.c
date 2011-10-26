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

#include"parser.h"
#include"bnf_rules.h"

#define NULL_RULE { repeat_off, 0, { NULL } } 
parser_rule_t rule_start = {
    "rule_start",
    NULL,
    NULL,
    {
        { repeat_off, 0, { &rule_rules, NULL } },
        NULL_RULE
    }
};

parser_rule_t rule_rules = {
    "rule_rules",
    NULL,
    NULL,
    {
        { repeat_on, 0, { & rule_rule, NULL } },
        NULL_RULE
    }
};

parser_rule_t rule_rule = {
    "rule_rule",
    NULL,
    NULL,
    {
        { repeat_off, 0, { &rule_rule_lhs, &rule_definition_sign, &rule_rule_rhs, &rule_semicolon, NULL } },
        NULL_RULE
    }
};

parser_rule_t rule_rule_lhs = {
    "rule_rule_lhs",
    NULL,
    NULL,
    {
        { repeat_off, 0, { &rule_identifier, NULL } },
        NULL_RULE
    }
};

parser_rule_t rule_rule_rhs = {
    "rule_rule_rhs",
    NULL,
    NULL,
    {
        { repeat_on, 0, { &rule_rule_phrase, NULL } },
        NULL_RULE
    }
};

parser_rule_t rule_rule_phrase = {
    "rule_rule_phrase",
    NULL,
    NULL,
    {
        { repeat_off, 0, { &rule_term, &rule_repetition_marker, NULL } },
        { repeat_off, 0, { &rule_term, NULL } },
        NULL_RULE
    }
};

parser_rule_t rule_term = {
    "rule_term",
    NULL,
    NULL,
    {
        { repeat_off, 0, { &rule_symbol, NULL } },
        { repeat_off, 0, { &rule_openparen, &rule_rule_rhs, &rule_closeparen, NULL } },
        NULL_RULE
    }
};

parser_rule_t rule_openparen = {
    "rule_openparen",
    parser__parse_openparen,
    NULL,
    {
        NULL_RULE
    }
};

parser_rule_t rule_closeparen = {
    "rule_closeparen",
    parser__parse_closeparen,
    NULL,
    {
        NULL_RULE
    }
};

parser_rule_t rule_repetition_marker = {
    "rule_repetition_marker",
    parser__parse_repetition_marker,
    NULL,
    {
        NULL_RULE
    }
};

parser_rule_t rule_symbol = {
    "rule_symbol",
    NULL,
    NULL,
    {
        { repeat_off, 0, { &rule_terminal, NULL } },
        { repeat_off, 0, { &rule_nonterminal, NULL } },
        NULL_RULE
    }
};

parser_rule_t rule_terminal = {
    "rule_terminal",
    NULL,
    NULL,
    {
        { repeat_off, 0, { &rule_keyword_regex, &rule_literal, NULL } },
        { repeat_off, 0, { &rule_literal, NULL  } },
        NULL_RULE
    }
};

parser_rule_t rule_keyword_regex = {
    "rule_keyword_regex",
    parser__parse_keyword_regex,
    NULL,
    {
        NULL_RULE
    }
};

parser_rule_t rule_literal = {
    "rule_literal",
    parser__parse_literal,
    NULL,
    {
        NULL_RULE
    }
};
parser_rule_t rule_definition_sign = {
    "rule_definition_sign",
    parser__parse_definition_sign,
    NULL,
    {
        NULL_RULE
    }
};
parser_rule_t rule_semicolon = {
    "rule_semicolon",
    parser__parse_semicolon,
    NULL,
    {
        NULL_RULE
    }
};
parser_rule_t rule_identifier = {
    "rule_identifier",
    parser__parse_identifier,
    NULL,
    {
        NULL_RULE
    }
};
