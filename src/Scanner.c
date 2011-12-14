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

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"Token.h"
#include"Scanner.h"

void Scanner_reset(Scanner* this);

void Scanner__raiseFatalError(const char* msg) {
    printf("SCANNER FATAL ERROR: %s\n", msg);
    abort();
}

Scanner* Scanner__new(const char* text) {
    Scanner* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->text = this->text_start = text;
        this->token = NULL;
        this->state = ScannerState_initial;
        this->error = Error_ok;
    } else
        Scanner__raiseFatalError("Unable to allocate this.");
    return this;
}

void Scanner_delete(Scanner* this) {
    free(this);
}

void Scanner_reset(Scanner* this) {
    this->text_start = this->text;
}

bool Scanner_scanEmpty(Scanner* this, bool* continue_loop);

bool Scanner_scan(Scanner* this) {
    bool continue_loop = false;
    bool rv = false;
    Scanner_reset(this);
    do {
        char ch = *this->text;
        switch (this->state) {
            case ScannerState_initial:
                break;
            default:
                Scanner_raiseError(this, Error_unknownScannerState);
                return false;
        }
    } while (continue_loop);
    return rv;
}

void Scanner_raiseError(Scanner* this, Error error) {
    this->error = error;
}
