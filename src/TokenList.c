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

#include<stdio.h>
#include<stdlib.h>
#include"BNF.h"
#include"scanner.h"
#include"token_list.h"

TokenList* token_list__new(TokenList* prev, TokenType* token) {
    TokenList* token_list = malloc(sizeof(TokenList));
    if (token_list) {
        token_list->prev = prev;
        token_list->token = token;
        token_list->next = NULL;
        if (prev)
            prev->next = token_list;
    } else
        Scanner__raiseFatalError("Unable to allocate token_list.");
    return token_list;
}

void token_list__delete(TokenList* token_list) {
    if (token_list->next)
        token_list__delete(token_list->next);
    if (token_list->prev)
        token_list->prev->next = NULL;
    free(token_list);
}

TokenList* token_list__get_prev(TokenList* current) {
    return current->prev;
}

TokenList* token_list__get_next(TokenList* current) {
    return current->next;
}

TokenType* token_list__get_token(TokenList* current) {
    return current->token;
}
