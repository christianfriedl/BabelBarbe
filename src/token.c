    /*
    DaSL - Datetime Specific Language, a little DSL for dealing with dates and times

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

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"dasl.h"
#include"token.h"

token_t *token__new() {
    token_t *token = malloc(sizeof(token_t));
    if (token) {
        token->type = t_start;
        token->text = NULL;
    } else
        dasl_raise_fatal_error("Unable to allocate token.");
    return token;
}

token_t *token__new_from_type_string_len(token_type_t type, const char *src, const size_t len) {
    token_t *token = token__new();
    token->type = type;
    token->text = malloc(sizeof(char) * (len + 1));
    if (token->text != NULL) {
        strncpy(token->text, src, len);
        token->text[len] = '\0';
    } else
        dasl_raise_fatal_error("Unable to allocate token.");
    return token;
}

void token__delete(token_t *token) {
    /* TODO ahem: free(token->text); */
    free(token);
}

void token__print(token_t *token) {
    char *type_name = token__get_type_name(token);
    printf("token @%ld: type='%s', text='%s'\n", (long int)token, type_name, token->text);
    free(type_name);
}

char *token__to_string(token_t *token) {
    char *type_name = token__get_type_name(token);
    char *mystrbuf = malloc(255);
    sprintf(mystrbuf, "token @%ld: type='%s', text='%s'", (long int)token, type_name, token->text);
    free(type_name);
    return mystrbuf;
}

char *token__get_type_name(token_t *token) {
    switch (token->type) {
        case t_start: 
            return strdup("t_start");
            break;
        case t_literal_uint: 
            return strdup("t_literal_uint");
            break;
        case t_quote: 
            return strdup("t_quote");
            break;
        case t_identifier: 
            return strdup("t_identifier");
            break;
        case t_symbol: 
            return strdup("t_symbol");
            break;
        case t_eof:
            return strdup("t_eof");
            break;
        default:
            dasl_raise_fatal_error("unknown token type");
            return NULL;
    }
}
