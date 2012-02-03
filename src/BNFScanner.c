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
#include<cgenerics/CGException.h>
#include<cgenerics/CGAppState.h>
#include"BNF.h"
#include"BNFToken.h"
#include"BNFException.h"
#include"BNFScanner.h"

BNFScanner* BNFScanner__new(const char* text) {
    BNFScanner* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->text = this->text_start = text;
        this->token = NULL;
        this->state = BNFScannerState_initial;
    } else
        CGAppState_throwException(this->appState, CGException__new(Severity_fatal, BNFExceptionID_ScannerFatalError, "unable to allocate BNFScanner"));
    return this;
}

void BNFScanner_delete(BNFScanner* this) {
    free(this);
}

bool BNFScanner_scan(BNFScanner* this) {
    bool continue_loop = false;
    bool rv = false;
    do {
        char ch = *this->text;
        switch (this->state) {
            case BNFScannerState_initial:
                break;
            default:
                BNFScanner_raiseError(this, Severity_error, BNFExceptionID_UnknownScannerState, "unknown scanner state");
                return false;
        }
    } while (continue_loop);
    return rv;
}

void BNFScanner_raiseError(BNFScanner* this, Severity severity, int exceptionID, char *msg) {
    CGAppState_throwException(this->appState, CGException__new(severity, exceptionID, msg));
}
