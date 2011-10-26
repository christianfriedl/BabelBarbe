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

#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"parser.h"

int parser__debug_indent = 0;
void parser__debug_print_enter_function(parser_t *parser, const char *func);
void parser__debug_print_exit_function(parser_t *parser, const char *func);
void parser__debug_print_enter_rule(parser_t *parser, const char *rule_name);
void parser__debug_print_exit_rule(parser_t *parser, const char *rule_name, const char *succ);

bool parser__scan(parser_t *parser);
void parser__raise_fatal_error(const char *msg) __attribute((noreturn));
void parser__raise_error(parser_t *parser, error_t error);

void parser__raise_fatal_error(const char *msg) {
    printf("PARSER FATAL ERROR: %s\n", msg);
    abort();
}
void parser__raise_error(parser_t *parser, error_t error) {
    parser->error = error;
    strcpy(parser->error_text,  parser->error_texts[error]);
}

bool parser__is_error(parser_t *parser) {
    return (parser->error != e_ok ? true : false);
}

error_t parser__get_error(parser_t *parser) {
    return parser->error;
}

char *parser__get_error_text(parser_t *parser) {
    return parser->error_text;
}

void parser__init_error_texts(parser_t *parser);
parser_t *parser__new(scanner_t *scanner, parser_rule_t *start_rule) {
    parser_t *parser = NULL;
    parser = malloc(sizeof(*parser));
    if (parser) {
        parser->error = e_ok;
        parser->scanner = scanner;
        parser->token_list = NULL;
        parser->tlc = NULL;
        parser->is_debug = false;
        parser->start_rule = start_rule;
        parser->ast_root = NULL;
        parser__init_error_texts(parser);
    } else
        parser__raise_fatal_error("Unable to allocate parser.");
    return parser;
}
void parser__init_error_texts(parser_t *parser) {
    strcpy(parser->error_texts[e_ok], "No error");

    strcpy(parser->error_texts[e_generic_error], "Something bad happened, giving up");
    strcpy(parser->error_texts[e_unexpected_eof], "Unexpected end of input");
    strcpy(parser->error_texts[e_unknown_scanner_state], "Unknown scanner state");
    strcpy(parser->error_texts[e_initial_or_reset_expected], "Expected initial scanner state");

    strcpy(parser->error_texts[e_cannot_add_unit_to_date], "This unit-type cannot be added to a date");
    strcpy(parser->error_texts[e_cannot_add_types], "The add operation is not defined for these operator types");
    strcpy(parser->error_texts[e_cannot_subtract_unit_from_date], "This unit-type cannot be subtracted from a date");
    strcpy(parser->error_texts[e_cannot_subtract_types], "The subtract operation is not defined for these operator types");
    strcpy(parser->error_texts[e_cannot_cast_to_date], "This type cannot be cast to date");
    strcpy(parser->error_texts[e_cannot_cast_to_datetime], "This type cannot be cast to datetime");
    strcpy(parser->error_texts[e_cannot_cast_to_uint], "This type cannot be cast to uint");
}

void parser__debug(parser_t *parser) {
    parser->is_debug = true;
}

void parser__delete(parser_t *parser) {
    scanner__delete(parser->scanner);
    if (parser->token_list != NULL)
        token_list__delete(parser->token_list);
    if (parser->ast_root != NULL)
        ast__delete(parser->ast_root);
    free(parser);
}

bool parser__scan(parser_t *parser) {
    bool scanner_ok = false;
    token_list_t *prev = NULL;
    token_t *token = NULL; 
    token_list_t *token_list = NULL;
    do {
        scanner_ok = scanner__scan(parser->scanner);
        if (scanner_ok) {
            token = parser->scanner->token;
            token_list = token_list__new(prev, token);
            if (prev == NULL) {
                parser->token_list = token_list;
                parser->tlc = tlc__new(token_list);
            }
            prev = token_list;
        } else
            parser->error = parser->scanner->error;
    } while (scanner_ok && token->type != t_eof);
    return scanner_ok;
}

bool parser__parse_rule(parser_t *parser, parser_rule_t *current_rule);
bool parser__parse_terminal(parser_t *parser, parser_rule_t *current_rule);
bool parser__parse_alternatives(parser_t *parser, parser_rule_t *current_rule);
bool parser__evaluate_if_parsed(parser_t *parser, parser_rule_t *current_rule, bool is_parsed);
bool parser__evaluate_for_rule(parser_t *parser, parser_rule_t *current_rule);
bool parser__parse_nonrepeating_alternative(parser_t *parser, parser_rule_t *current_rule, struct rule **productions);
bool parser__parse_repeating_alternative(parser_t *parser, parser_rule_t *current_rule, alternative_t *alternative);

bool parser__parse(parser_t *parser) {
    if (parser->is_debug) printf("===> starting parser with text: '%s'\n", parser->scanner->text);
    if (!parser__scan(parser))
        return false;

    return parser__parse_rule(parser, parser->start_rule);
}

bool parser__evaluate_for_rule(parser_t *parser, parser_rule_t *current_rule) {
    (current_rule->evaluate)(parser);
    return true;
}

bool parser__parse_rule(parser_t *parser, parser_rule_t *current_rule) {
    bool is_parsed = false;

    if (parser->error != e_ok)
        return false;
    parser__debug_print_enter_rule(parser, current_rule->name);
    if (current_rule->parse != NULL) { /* terminal symbol */
        is_parsed = parser__parse_terminal(parser, current_rule);
    } else {
        is_parsed = parser__parse_alternatives(parser, current_rule);
    }
    parser__debug_print_exit_rule(parser, current_rule->name, bool__to_string(is_parsed));
    return is_parsed;
}
bool parser__parse_terminal(parser_t *parser, parser_rule_t *current_rule) {
    if ((current_rule->parse)(parser) == false) 
        return false;
    if (current_rule->evaluate != NULL) {
        if (parser__evaluate_for_rule(parser, current_rule) == false)
            return false;
    } 
    tlc__move_to_next(parser->tlc);
    return true;
}
bool parser__parse_alternatives(parser_t *parser, parser_rule_t *current_rule) {
    unsigned int i = 0;
    bool is_parsed = false;

    alternative_t *alternatives = current_rule->alternatives;
    is_parsed = false;
    for (i = 0; alternatives[i].productions[0] != NULL && !is_parsed; ++i) {
        if (alternatives[i].repeat_switch == repeat_on)
            is_parsed = parser__parse_repeating_alternative(parser, current_rule, &alternatives[i]);
        else
            is_parsed = parser__parse_nonrepeating_alternative(parser, current_rule, alternatives[i].productions);
    }
    return is_parsed;
}

bool parser__parse_nonrepeating_alternative(parser_t *parser, parser_rule_t *current_rule, struct rule **productions) {
    bool is_parsed = true;
    unsigned int start_index = 0;
    unsigned int i_prod = 0;

    tlc__set_mark(parser->tlc);

    for (i_prod = start_index; productions[i_prod] != NULL && is_parsed; ++i_prod) {
        if (productions[i_prod] != NULL)
            is_parsed &= parser__parse_rule(parser, productions[i_prod]);
    }

    if (is_parsed)
        tlc__unset_mark(parser->tlc);
    else {
        tlc__move_to_mark(parser->tlc);
    }

    is_parsed = parser__evaluate_if_parsed(parser, current_rule, is_parsed);
    return is_parsed;
}
bool parser__parse_repeating_alternative(parser_t *parser, parser_rule_t *current_rule, alternative_t *alternative) {
    struct rule **productions = alternative->productions;
    bool is_parsed = true;
    unsigned int start_index = 0;
    unsigned int i_prod = 0;
    bool repeater_is_parsed = false;

    tlc__set_mark(parser->tlc);

    do { 
        for (i_prod = start_index; productions[i_prod] != NULL && is_parsed; ++i_prod) {
            if (productions[i_prod] != NULL)
                is_parsed &= parser__parse_rule(parser, productions[i_prod]);
        }
        start_index = alternative->repeat_from_index;
        repeater_is_parsed |= is_parsed;         /* repeater is seen as parsed the moment it passed the first loop */
        is_parsed = parser__evaluate_if_parsed(parser, current_rule, is_parsed);
        if (is_parsed) {
            tlc__unset_mark(parser->tlc);
            tlc__set_mark(parser->tlc);
        }
    } while (is_parsed);

    is_parsed = repeater_is_parsed;             /* translate back to "normal" is_parsed handling */

    tlc__move_to_mark(parser->tlc);

    return is_parsed;
}

bool parser__evaluate_if_parsed(parser_t *parser, parser_rule_t *current_rule, bool is_parsed) {
    if (is_parsed) {
        if (current_rule->evaluate != NULL) {
            return parser__evaluate_for_rule(parser, current_rule);
        } else
            return true;
    } else
        return false;
}


/*
    specific productions
*/



void parser__debug_print_enter_function(parser_t *parser, const char *func) {
    if (parser->is_debug) {
        int i;
        char *token_string = token__to_string(tlc__get_current(parser->tlc));
        for (i = 0; i < parser__debug_indent; ++i) printf("    ");
        parser__debug_indent++;
        printf("> %s (%s)\n", func, token_string);
        free(token_string);
    }
}
void parser__debug_print_exit_function(parser_t *parser, const char *func) {
    if (parser->is_debug) {
        int i;
        char *token_string = token__to_string(tlc__get_current(parser->tlc));
        parser__debug_indent--;
        for (i = 0; i < parser__debug_indent; ++i) printf("    ");
        printf("< %s (%s)\n", func, token_string);
        free(token_string);
    }
}

void parser__debug_print_enter_rule(parser_t *parser, const char *rule_name) {
    if (parser->is_debug) {
        int i;
        for (i = 0; i < parser__debug_indent; ++i) printf("    ");
        parser__debug_indent++;
        printf("> %s\n", rule_name);
    }
}

void parser__debug_print_exit_rule(parser_t *parser, const char *rule_name, const char *succ) {
    if (parser->is_debug) {
        int i;
        parser__debug_indent--;
        for (i = 0; i < parser__debug_indent; ++i) printf("    ");
        printf("< %s (%s)\n", rule_name, succ);
    }
}
