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

#ifndef _SCANNER_H
#define _SCANNER_H

#include<ctype.h>
#include<stdlib.h>
#include"bnf.h"
#include"Token.h"

typedef enum { ScannerState_initial = 0 } ScannerState;

typedef struct { 
    const char* text;
    const char* text_start;
    Token* token;
    ScannerState state;
    Error error;
} Scanner;

Scanner* Scanner__new(const char* text);
void Scanner_delete(Scanner* this);
bool Scanner_scan(Scanner* this);
void Scanner_raiseError(Scanner* this, Error error);
void Scanner__raiseFatalError(const char* msg) __attribute((noreturn));



#endif
