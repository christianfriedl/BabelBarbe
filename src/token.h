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

#ifndef _TOKEN_H
#define _TOKEN_H

typedef enum { t_start, t_literal_uint, t_quote, t_identifier, t_symbol, t_eof } token_type_t;

typedef struct {
    token_type_t type;
    char *text;
} token_t;

token_t *token__new();
token_t *token__new_from_type_string_len(token_type_t type, const char *src, const size_t len);
void token__delete(token_t *token);
void token__print(token_t *token);
char *token__get_type_name(token_t *token);
char *token__to_string(token_t *token);

#endif
