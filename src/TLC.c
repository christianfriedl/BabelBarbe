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
#include"TLC.h"

TLC* TLC__new(TokenList* token_list) {
    TLC* tlc = malloc(sizeof(TLC));
    if (tlc != NULL) {
        int i;
        tlc->current_token_list = token_list;
        tlc->token_list_start = token_list;
        for (i=0; i < MAX_TLC_MARKS; ++i)
            tlc->token_list_marks[i] = NULL;
        tlc->count_token_list_marks = 0;
    } else
        bnf__raise_fatal_error("Unable to allocate TLC");
    return tlc;
}
void TLC_delete(TLC* tlc) {
    if (tlc->token_list_start != NULL)
        token_list__delete(tlc->token_list_start);
    free(tlc);
}
TokenType* TLC_getCurrent(TLC* tlc) {
    return token_list__get_token(tlc->current_token_list);
}
TokenList* TLC_getCurrentTokenList(TLC* tlc) {
    return tlc->current_token_list;
}
TokenType* TLC_moveToNext(TLC* tlc) {
    if (token_list__get_next(tlc->current_token_list) != NULL) {
        tlc->current_token_list = token_list__get_next(tlc->current_token_list);
        return token_list__get_token(tlc->current_token_list);
    } else
        return NULL;
        
}
void TLC_set_mark(TLC* tlc) {
    if (tlc->count_token_list_marks < MAX_TLC_MARKS - 1)
        tlc->token_list_marks[(tlc->count_token_list_marks)++] = tlc->current_token_list;
    else
        bnf__raise_fatal_error("Maximum number of token_list_marks reached.");
}
void TLC_unset_mark(TLC* tlc) {
    --(tlc->count_token_list_marks);
}
TokenType* TLC_moveToMark(TLC* tlc) {
    if (tlc->count_token_list_marks == 0)
        bnf__raise_fatal_error("Mark not set.");
    tlc->current_token_list = tlc->token_list_marks[--(tlc->count_token_list_marks)];
    return token_list__get_token(tlc->current_token_list);
}
TokenType* TLC_moveToPrevious(TLC* tlc) {
    if (token_list__get_prev(tlc->current_token_list) != NULL) {
        tlc->current_token_list = token_list__get_prev(tlc->current_token_list);
        return token_list__get_token(tlc->current_token_list);
    } else
        return NULL;
}

TokenType* TLC_moveToStart(TLC* tlc) {
    tlc->current_token_list = tlc->token_list_start;
    if (tlc->current_token_list)
        return token_list__get_token(tlc->current_token_list);
    else
        return NULL;
}
