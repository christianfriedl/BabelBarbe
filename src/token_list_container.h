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

#ifndef _TOKEN_LIST_CONTAINER_H
#define _TOKEN_LIST_CONTAINER_H

#include"token_list.h"

#define MAX_TLC_MARKS 255

typedef struct {
    token_list_t *current_token_list;
    token_list_t *token_list_start;
    token_list_t *token_list_marks[MAX_TLC_MARKS];
    unsigned int count_token_list_marks;
} token_list_container_t;

token_list_container_t *tlc__new(token_list_t *token_list);
void tlc__delete(token_list_container_t *tlc);
token_t *tlc__get_current(token_list_container_t *tlc);
token_list_t *tlc__get_current_token_list(token_list_container_t *tlc);
void tlc__set_mark();
void tlc__unset_mark(token_list_container_t *tlc);
token_t *tlc__move_to_mark(token_list_container_t *tlc);
token_t *tlc__move_to_next(token_list_container_t *tlc);
token_t *tlc__move_to_prev(token_list_container_t *tlc);
token_t *tlc__move_to_start(token_list_container_t *tlc);

#endif
