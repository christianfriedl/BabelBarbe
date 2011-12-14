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
#include"Parser.h"

int Parser_debug_indent = 0; /* the current indentation for the debug output */
void Parser_debugPrintEnterFunction(Parser* parser, const char* func);
void Parser_debugPrintExitFunction(Parser* parser, const char* func);
void Parser_debugPrintEnterRule(Parser* parser, const char* rule_name);
void Parser_debugPrintExitRule(Parser* parser, const char* rule_name, const char* succ);

bool Parser_scan(Parser* parser);
void Parser__raiseFatalError(const char* msg) __attribute((noreturn));
void Parser_raiseError(Parser* parser, Error error);

void Parser__raiseFatalError(const char* msg) {
    printf("PARSER FATAL ERROR: %s\n", msg);
    abort();
}
void Parser_raiseError(Parser* parser, Error error) {
    parser->error = error;
    strcpy(parser->error_text,  parser->error_texts[error]);
}

bool Parser_isError(Parser* parser) {
    return (parser->error != Error_ok ? true : false);
}

Error Parser_getError(Parser* parser) {
    return parser->error;
}

char* Parser_getErrorText(Parser* parser) {
    return parser->error_text;
}

void Parser_initErrorTexts(Parser* parser);
Parser* Parser__new(Scanner* scanner, ParserRule* start_rule) {
    Parser* parser = NULL;
    parser = malloc(sizeof(*parser));
    if (parser) {
        parser->error = e_ok;
        parser->scanner = scanner;
        parser->token_list = NULL;
        parser->tlc = NULL;
        parser->is_debug = false;
        parser->start_rule = start_rule;
        parser->ast_root = NULL;
        Parser_initErrorTexts(parser);
    } else
        Parser__raiseFatalError("Unable to allocate parser.");
    return parser;
}
void Parser_initErrorTexts(Parser* parser) {
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

void Parser_debug(Parser* parser) {
    parser->is_debug = true;
}

void Parser_delete(Parser* parser) {
    Scanner_delete(parser->scanner);
    if (parser->token_list != NULL)
        token_list__delete(parser->token_list);
    if (parser->ast_root != NULL)
        ast__delete(parser->ast_root);
    free(parser);
}

bool Parser_scan(Parser* parser) {
    bool scanner_ok = false;
    TokenList* prev = NULL;
    TokenType* token = NULL; 
    TokenList* token_list = NULL;
    do {
        scanner_ok = Scanner_scan(parser->scanner);
        if (scanner_ok) {
            token = parser->scanner->token;
            token_list = token_list__new(prev, token);
            if (prev == NULL) {
                parser->token_list = token_list;
                parser->tlc = TLC__new(token_list);
            }
            prev = token_list;
        } else
            parser->error = parser->scanner->error;
    } while (scanner_ok && token->type != t_eof);
    return scanner_ok;
}

bool Parser_parseRule(Parser* parser, ParserRule* current_rule);
bool Parser_parseTerminal(Parser* parser, ParserRule* current_rule);
bool Parser_parseAlternatives(Parser* parser, ParserRule* current_rule);
bool Parser_evaluateIfParsed(Parser* parser, ParserRule* current_rule, bool is_parsed);
bool Parser_evaluateForRule(Parser* parser, ParserRule* current_rule);
bool Parser_parseNonrepeatingAlternative(Parser* parser, ParserRule* current_rule, struct rule* *productions);
bool Parser_parseRepeatingAlternative(Parser* parser, ParserRule* current_rule, alternative_t* alternative);

bool Parser_parse(Parser* parser) {
    if (parser->is_debug) printf("===> starting parser with text: '%s'\n", parser->scanner->text);
    if (!Parser_scan(parser))
        return false;

    return Parser_parseRule(parser, parser->start_rule);
}

bool Parser_evaluateForRule(Parser* parser, ParserRule* current_rule) {
    (current_rule->evaluate)(parser);
    return true;
}

bool Parser_parseRule(Parser* parser, ParserRule* current_rule) {
    bool is_parsed = false;

    if (parser->error != e_ok)
        return false;
    Parser_debug_print_enter_rule(parser, current_rule->name);
    if (current_rule->parse != NULL) { /* terminal symbol */
        is_parsed = Parser_parseTerminal(parser, current_rule);
    } else {
        is_parsed = Parser_parseAlternatives(parser, current_rule);
    }
    Parser_debugPrintExitRule(parser, current_rule->name, bool__to_string(is_parsed));
    return is_parsed;
}
bool Parser_parseTerminal(Parser* parser, ParserRule* current_rule) {
    if ((current_rule->parse)(parser) == false) 
        return false;
    if (current_rule->evaluate != NULL) {
        if (Parser_evaluateForRule(parser, current_rule) == false)
            return false;
    } 
    TLC_moveToNext(parser->tlc);
    return true;
}
bool Parser_parseAlternatives(Parser* parser, ParserRule* current_rule) {
    unsigned int i = 0;
    bool is_parsed = false;

    alternative_t* alternatives = current_rule->alternatives;
    is_parsed = false;
    for (i = 0; alternatives[i].productions[0] != NULL && !is_parsed; ++i) {
        if (alternatives[i].repeat_switch == repeat_on)
            is_parsed = Parser_parseRepeatingAlternative(parser, current_rule, &alternatives[i]);
        else
            is_parsed = Parser_parseNonrepeatingAlternative(parser, current_rule, alternatives[i].productions);
    }
    return is_parsed;
}

bool Parser_parseNonrepeatingAlternative(Parser* parser, ParserRule* current_rule, struct rule* *productions) {
    bool is_parsed = true;
    unsigned int start_index = 0;
    unsigned int i_prod = 0;

    TLC_set_mark(parser->tlc);

    for (i_prod = start_index; productions[i_prod] != NULL && is_parsed; ++i_prod) {
        if (productions[i_prod] != NULL)
            is_parsed &= Parser_parseRule(parser, productions[i_prod]);
    }

    if (is_parsed)
        TLC_unset_mark(parser->tlc);
    else {
        TLC_moveToMark(parser->tlc);
    }

    is_parsed = Parser_evaluateIfParsed(parser, current_rule, is_parsed);
    return is_parsed;
}
bool Parser_parseRepeatingAlternative(Parser* parser, ParserRule* current_rule, alternative_t* alternative) {
    struct rule* *productions = alternative->productions;
    bool is_parsed = true;
    unsigned int start_index = 0;
    unsigned int i_prod = 0;
    bool repeater_is_parsed = false;

    TLC_set_mark(parser->tlc);

    do { 
        for (i_prod = start_index; productions[i_prod] != NULL && is_parsed; ++i_prod) {
            if (productions[i_prod] != NULL)
                is_parsed &= Parser_parseRule(parser, productions[i_prod]);
        }
        start_index = alternative->repeat_from_index;
        repeater_is_parsed |= is_parsed;         /* repeater is seen as parsed the moment it passed the first loop */
        is_parsed = Parser_evaluateIfParsed(parser, current_rule, is_parsed);
        if (is_parsed) {
            TLC_unset_mark(parser->tlc);
            TLC_set_mark(parser->tlc);
        }
    } while (is_parsed);

    is_parsed = repeater_is_parsed;             /* translate back to "normal" is_parsed handling */

    TLC_moveToMark(parser->tlc);

    return is_parsed;
}

bool Parser_evaluateIfParsed(Parser* parser, ParserRule* current_rule, bool is_parsed) {
    if (is_parsed) {
        if (current_rule->evaluate != NULL) {
            return Parser_evaluateForRule(parser, current_rule);
        } else
            return true;
    } else
        return false;
}


/*
    specific productions
*/



void Parser_debugPrintEnterFunction(Parser* parser, const char* func) {
    if (parser->is_debug) {
        int i;
        char* token_string = token__to_string(TLC_getCurrent(parser->tlc));
        for (i = 0; i < Parser_debug_indent; ++i) printf("    ");
        Parser_debug_indent++;
        printf("> %s (%s)\n", func, token_string);
        free(token_string);
    }
}
void Parser_debugPrintExitFunction(Parser* parser, const char* func) {
    if (parser->is_debug) {
        int i;
        char* token_string = token__to_string(TLC_getCurrent(parser->tlc));
        Parser_debug_indent--;
        for (i = 0; i < Parser_debug_indent; ++i) printf("    ");
        printf("< %s (%s)\n", func, token_string);
        free(token_string);
    }
}

void Parser_debugPrintEnterRule(Parser* parser, const char* rule_name) {
    if (parser->is_debug) {
        int i;
        for (i = 0; i < Parser_debug_indent; ++i) printf("    ");
        Parser_debug_indent++;
        printf("> %s\n", rule_name);
    }
}

void Parser_debugPrintExitRule(Parser* parser, const char* rule_name, const char* succ) {
    if (parser->is_debug) {
        int i;
        Parser_debug_indent--;
        for (i = 0; i < Parser_debug_indent; ++i) printf("    ");
        printf("< %s (%s)\n", rule_name, succ);
    }
}
