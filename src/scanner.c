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

void scanner__reset(scanner_t *this);
bool is_single_character_symbol(int ch);
bool is_double_character_symbol(const char *str);

void scanner__raise_fatal_error(const char *msg) {
    printf("SCANNER FATAL ERROR: %s\n", msg);
    abort();
}

scanner_t *scanner__new(const char *text) {
    scanner_t *this = malloc(sizeof(scanner_t));
    if (this != NULL) {
        this->text = this->text_start = text;
        this->token = NULL;
        this->state = s_initial;
        this->error = e_ok;
    } else
        scanner__raise_fatal_error("Unable to allocate this.");
    return this;
}

void scanner__delete(scanner_t *this) {
    free(this);
}

void scanner__reset(scanner_t *this) {
    this->text_start = this->text;
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

bool scanner__scan_empty(scanner_t *this, bool *continue_loop);

bool scanner__scan(scanner_t *this) {
    bool continue_loop = false;
    bool rv = false;
    scanner__reset(this);
    do {
        char ch = *this->text;
        switch (this->state) {
            case s_initial:
            case s_reset:
                rv = scanner__scan_empty(this, &continue_loop);
                break;
            case s_digit:
                if (isdigit(ch)) {
                    (this->text)++;
                    continue_loop = true;
                } else if (ch == '\'') {
                    this->state = s_quote;
                    this->token = token__new_from_type_string_len(t_literal_uint, this->text_start, this->text - this->text_start);
                    continue_loop = false;
                    rv = true;
                } else {
                    this->state = s_reset;
                    this->token = token__new_from_type_string_len(t_literal_uint, this->text_start, this->text - this->text_start);
                    continue_loop = false;
                    rv = true;
                }
                break;
            case s_single_character_symbol:
                if (is_double_character_symbol(this->text)) {
                    this->state = s_reset;
                    this->text += 2;
                    this->token = token__new_from_type_string_len(t_symbol, this->text_start, this->text - this->text_start);
                    continue_loop = false;
                    rv = true;
                } else {
                    this->state = s_reset;
                    (this->text)++;
                    this->token = token__new_from_type_string_len(t_symbol, this->text_start, this->text - this->text_start);
                    continue_loop = false;
                    rv = true;
                }
                break;
            case s_double_character_symbol:
                this->state = s_reset;
                this->text += 2;
                this->token = token__new_from_type_string_len(t_symbol, this->text_start, this->text - this->text_start);
                continue_loop = false;
                rv = true;
                break;
            case s_quote:
                this->state = s_reset;
                (this->text)++;
                this->token = token__new_from_type_string_len(t_quote, this->text_start, this->text - this->text_start);
                continue_loop = false;
                rv = true;
                break;
            case s_alpha:
                if (isalpha(ch)) {
                    (this->text)++;
                    continue_loop = true;
                } else {
                    this->token = token__new_from_type_string_len(t_identifier, this->text_start, this->text - this->text_start);
                    this->state = s_reset;
                    continue_loop = false;
                    rv = true;
                }
                break;
            default:
                scanner__raise_error(this, e_unknown_scanner_state);
                return false;
        }
    } while (continue_loop);
    return rv;
}

// TODO find a more suitable name - it's for initial/reset
bool scanner__scan_empty(scanner_t *this, bool *continue_loop) {
        char ch = *this->text;
        switch (this->state) {
            case s_initial:
                if (ch == '\0') {
                    scanner__raise_error(this, e_unexpected_eof);
                    *continue_loop = false;
                    return false;
                } else if (ch == '\'') {
                    this->state = s_quote;
                    *continue_loop = true;
                    return true;
                }
                break;
            case s_reset:
                if (ch == '\0') {
                    this->state = s_initial;
                    this->token = token__new_from_type_string_len(t_eof, "", 0);
                    *continue_loop = false;
                    return true;
                } else if (ch == '\'') {
                    this->state = s_quote;
                    this->text_start = this->text;
                    *continue_loop = true;
                    return true;
                }
                break;
            default:
                scanner__raise_error(this, e_initial_or_reset_expected);
        }
        if (isspace(ch))
            (this->text)++;
        else if (isdigit(ch)) {
            this->text_start = this->text;
            this->state = s_digit;
        } else if (isalpha(ch)) {
            this->text_start = this->text;
            this->state = s_alpha;
        } else if (is_single_character_symbol(ch)) {
            this->state = s_single_character_symbol;
            this->text_start = this->text;
        } else if (is_double_character_symbol(this->text)) {
            this->state = s_double_character_symbol;
            this->text_start = this->text;
        } else {
            scanner__raise_error(this, e_unexpected_symbol);
            *continue_loop = false;
            return false;
        }
        *continue_loop = true;
        return true;
}

void scanner__raise_error(scanner_t *this, error_t error) {
    this->error = error;
}
