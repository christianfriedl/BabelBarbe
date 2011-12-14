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

#ifndef _TOKEN_LIST_CONTAINER_H
#define _TOKEN_LIST_CONTAINER_H

#include"TokenList.h"

#define MAX_TLC_MARKS 255

typedef struct {
    TokenList* current_token_list;
    TokenList* token_list_start;
    TokenList* token_list_marks[MAX_TLC_MARKS];
    unsigned int count_token_list_marks;
} TLC;

TLC* TLC__new(TokenList* token_list);
void TLC_delete(TLC* tlc);
TokenType* TLC_getCurrent(TLC* tlc);
TokenList* TLC_getCurrentTokenList(TLC* tlc);
void TLC_set_mark();
void TLC_unset_mark(TLC* tlc);
TokenType* TLC_moveToMark(TLC* tlc);
TokenType* TLC_moveToNext(TLC* tlc);
TokenType* TLC_moveToPrevious(TLC* tlc);
TokenType* TLC_moveToStart(TLC* tlc);

#endif
