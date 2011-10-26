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
#include"tlc.h"

tlc_t *tlc__new(token_list_t *token_list) {
    tlc_t *tlc = malloc(sizeof(tlc_t));
    if (tlc != NULL) {
        int i;
        tlc->current_token_list = token_list;
        tlc->token_list_start = token_list;
        for (i=0; i < MAX_TLC_MARKS; ++i)
            tlc->token_list_marks[i] = NULL;
        tlc->count_token_list_marks = 0;
    } else
        bnf__raise_fatal_error("Unable to allocate tlc_t");
    return tlc;
}
void tlc__delete(tlc_t *tlc) {
    if (tlc->token_list_start != NULL)
        token_list__delete(tlc->token_list_start);
    free(tlc);
}
token_t *tlc__get_current(tlc_t *tlc) {
    return token_list__get_token(tlc->current_token_list);
}
token_list_t *tlc__get_current_token_list(tlc_t *tlc) {
    return tlc->current_token_list;
}
token_t *tlc__move_to_next(tlc_t *tlc) {
    if (token_list__get_next(tlc->current_token_list) != NULL) {
        tlc->current_token_list = token_list__get_next(tlc->current_token_list);
        return token_list__get_token(tlc->current_token_list);
    } else
        return NULL;
        
}
void tlc__set_mark(tlc_t *tlc) {
    if (tlc->count_token_list_marks < MAX_TLC_MARKS - 1)
        tlc->token_list_marks[(tlc->count_token_list_marks)++] = tlc->current_token_list;
    else
        bnf__raise_fatal_error("Maximum number of token_list_marks reached.");
}
void tlc__unset_mark(tlc_t *tlc) {
    --(tlc->count_token_list_marks);
}
token_t *tlc__move_to_mark(tlc_t *tlc) {
    if (tlc->count_token_list_marks == 0)
        bnf__raise_fatal_error("Mark not set.");
    tlc->current_token_list = tlc->token_list_marks[--(tlc->count_token_list_marks)];
    return token_list__get_token(tlc->current_token_list);
}
token_t *tlc__move_to_prev(tlc_t *tlc) {
    if (token_list__get_prev(tlc->current_token_list) != NULL) {
        tlc->current_token_list = token_list__get_prev(tlc->current_token_list);
        return token_list__get_token(tlc->current_token_list);
    } else
        return NULL;
}

token_t *tlc__move_to_start(tlc_t *tlc) {
    tlc->current_token_list = tlc->token_list_start;
    if (tlc->current_token_list)
        return token_list__get_token(tlc->current_token_list);
    else
        return NULL;
}
