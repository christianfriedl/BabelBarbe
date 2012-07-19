/*
BB Parser

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
#include<cgenerics/CGAppState.h>
#include"BB.h"
#include"BBException.h"
#include"BBToken.h"
#include"BBScannerRuleset.h"

DECLARE_ARRAY(BBToken)

typedef struct { 
    CGString* text;
    char* textPtr;
    char* textEndPtr; /* strictly for performance, so we can cache the strlen() at construction time */
    BBScannerRule* currentRule;
} BBScanner;

BBScanner* BBScanner__new(BBScannerRule* startRule, CGString* text);
void BBScanner_delete(BBScanner* this);
BBToken* BBScanner_scanNextToken(BBScanner* this);
CGArray(BBToken)* BBScanner_scanAllTokens(BBScanner* this);

#endif
