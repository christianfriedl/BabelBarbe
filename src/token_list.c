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
#include"bnf.h"
#include"scanner.h"
#include"token_list.h"

token_list_t* token_list__new(token_list_t* prev, TokenType* token) {
    token_list_t* token_list = malloc(sizeof(token_list_t));
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

void token_list__delete(token_list_t* token_list) {
    if (token_list->next)
        token_list__delete(token_list->next);
    if (token_list->prev)
        token_list->prev->next = NULL;
    free(token_list);
}

token_list_t* token_list__get_prev(token_list_t* current) {
    return current->prev;
}

token_list_t* token_list__get_next(token_list_t* current) {
    return current->next;
}

TokenType* token_list__get_token(token_list_t* current) {
    return current->token;
}
