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

#ifndef _TOKEN_LIST_H
#define _TOKEN_LIST_H

#include"token.h"

struct token_list_struct {
    TokenType* token;
    struct token_list_struct* prev;
    struct token_list_struct* next;
};

typedef struct token_list_struct TokenList;

TokenList* token_list__new(TokenList* prev, TokenType* token);
void token_list__delete(TokenList* token_list);
TokenList* token_list__get_prev(TokenList* current);
TokenList* token_list__get_next(TokenList* current);
TokenType* token_list__get_token(TokenList* current);

#endif
