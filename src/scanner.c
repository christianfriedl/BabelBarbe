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

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"token.h"
#include"scanner.h"

void scanner__reset(scanner_t *scanner);
bool is_single_character_symbol(int ch);
bool is_double_character_symbol(const char *str);

void scanner__raise_fatal_error(const char *msg) {
    printf("SCANNER FATAL ERROR: %s\n", msg);
    abort();
}

scanner_t *scanner__new(const char *text) {
    scanner_t *scanner = malloc(sizeof(scanner_t));
    if (scanner != NULL) {
        scanner->text = scanner->text_start = text;
        scanner->token = NULL;
        scanner->state = s_initial;
        scanner->error = e_ok;
    } else
        scanner__raise_fatal_error("Unable to allocate scanner.");
    return scanner;
}

void scanner__delete(scanner_t *scanner) {
    free(scanner);
}

void scanner__reset(scanner_t *scanner) {
    scanner->text_start = scanner->text;
}

bool is_single_character_symbol(int ch) {
    char single_character_symbols[] = "().+-:";
    return ((strchr(single_character_symbols, ch)) != NULL);
}

bool is_double_character_symbol(const char *str) {
    const char *double_character_symbols[] = {"::"};
    int i;
    for (i=0; i < 1; ++i)
        if (!strncmp(double_character_symbols[i], str, 2))
            return true;
    return false;
}

bool scanner__scan_empty(scanner_t *scanner, bool *continue_loop);

bool scanner__scan(scanner_t *scanner) {
    bool continue_loop = false;
    bool rv = false;
    scanner__reset(scanner);
    do {
        char ch = *scanner->text;
        switch (scanner->state) {
            case s_initial:
            case s_reset:
                rv = scanner__scan_empty(scanner, &continue_loop);
                break;
            case s_digit:
                if (isdigit(ch)) {
                    (scanner->text)++;
                    continue_loop = true;
                } else if (ch == '\'') {
                    scanner->state = s_quote;
                    scanner->token = token__new_from_type_string_len(t_literal_uint, scanner->text_start, scanner->text - scanner->text_start);
                    continue_loop = false;
                    rv = true;
                } else {
                    scanner->state = s_reset;
                    scanner->token = token__new_from_type_string_len(t_literal_uint, scanner->text_start, scanner->text - scanner->text_start);
                    continue_loop = false;
                    rv = true;
                }
                break;
            case s_single_character_symbol:
                if (is_double_character_symbol(scanner->text)) {
                    scanner->state = s_reset;
                    scanner->text += 2;
                    scanner->token = token__new_from_type_string_len(t_symbol, scanner->text_start, scanner->text - scanner->text_start);
                    continue_loop = false;
                    rv = true;
                } else {
                    scanner->state = s_reset;
                    (scanner->text)++;
                    scanner->token = token__new_from_type_string_len(t_symbol, scanner->text_start, scanner->text - scanner->text_start);
                    continue_loop = false;
                    rv = true;
                }
                break;
            case s_double_character_symbol:
                scanner->state = s_reset;
                scanner->text += 2;
                scanner->token = token__new_from_type_string_len(t_symbol, scanner->text_start, scanner->text - scanner->text_start);
                continue_loop = false;
                rv = true;
                break;
            case s_quote:
                scanner->state = s_reset;
                (scanner->text)++;
                scanner->token = token__new_from_type_string_len(t_quote, scanner->text_start, scanner->text - scanner->text_start);
                continue_loop = false;
                rv = true;
                break;
            case s_alpha:
                if (isalpha(ch)) {
                    (scanner->text)++;
                    continue_loop = true;
                } else {
                    scanner->token = token__new_from_type_string_len(t_identifier, scanner->text_start, scanner->text - scanner->text_start);
                    scanner->state = s_reset;
                    continue_loop = false;
                    rv = true;
                }
                break;
            default:
                scanner__raise_error(scanner, e_unknown_scanner_state);
                return false;
        }
    } while (continue_loop);
    return rv;
}

// TODO find a more suitable name - it's for initial/reset
bool scanner__scan_empty(scanner_t *scanner, bool *continue_loop) {
        char ch = *scanner->text;
        switch (scanner->state) {
            case s_initial:
                if (ch == '\0') {
                    scanner__raise_error(scanner, e_unexpected_eof);
                    *continue_loop = false;
                    return false;
                } else if (ch == '\'') {
                    scanner->state = s_quote;
                    *continue_loop = true;
                    return true;
                }
                break;
            case s_reset:
                if (ch == '\0') {
                    scanner->state = s_initial;
                    scanner->token = token__new_from_type_string_len(t_eof, "", 0);
                    *continue_loop = false;
                    return true;
                } else if (ch == '\'') {
                    scanner->state = s_quote;
                    scanner->text_start = scanner->text;
                    *continue_loop = true;
                    return true;
                }
                break;
            default:
                scanner__raise_error(scanner, e_initial_or_reset_expected);
        }
        if (isspace(ch))
            (scanner->text)++;
        else if (isdigit(ch)) {
            scanner->text_start = scanner->text;
            scanner->state = s_digit;
        } else if (isalpha(ch)) {
            scanner->text_start = scanner->text;
            scanner->state = s_alpha;
        } else if (is_single_character_symbol(ch)) {
            scanner->state = s_single_character_symbol;
            scanner->text_start = scanner->text;
        } else if (is_double_character_symbol(scanner->text)) {
            scanner->state = s_double_character_symbol;
            scanner->text_start = scanner->text;
        } else {
            scanner__raise_error(scanner, e_unexpected_symbol);
            *continue_loop = false;
            return false;
        }
        *continue_loop = true;
        return true;
}

void scanner__raise_error(scanner_t *scanner, error_t error) {
    scanner->error = error;
}
