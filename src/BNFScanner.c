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

BNFScanner* BNFScanner__new(CGAppState* appState, BNFScannerRule* startRule, CGString* text) {
    BNFScanner* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->currentRule = startRule;
        this->text = text;
        this->textPtr = CGString_toVector(appState, text);
        this->textEndPtr = this->textPtr + CGString_getSize(appState, this->text);
    } else
        CGAppState_THROW(appState, Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate BNFScanner");
    return this;
}

void BNFScanner_delete(CGAppState* appState, BNFScanner* this) {
    /* TODO delete rules */
    CGString_delete(appState, this->text);
    free(this);
}

BNFToken* BNFScanner_scanNextToken(CGAppState* appState, BNFScanner* this) {
    if (this->textPtr == this->textEndPtr) /* end of text reached, so no more tokens */
        return NULL;
    BNFToken* token = BNFScannerRule_applyToText(appState, this->currentRule, this->textPtr);
    if (token != NULL) {
        BNFScannerNode* node = BNFScannerRule_getNode(appState, this->currentRule);
        this->textPtr += CGString_getSize(appState, BNFToken_getText(appState, token));
        this->currentRule = BNFScannerNode_getFollowupRule(appState, node);
    } else /* no token, but not EOT, therefore there was an error */
        CGAppState_THROW(appState, Severity_error, BNFExceptionID_ScannerError, "Scanner error at %d, near '%s ...'", (this->textPtr - this->text), CGString_createSubstring(appState, this->textPtr, 0, 20));
    return token;
}

