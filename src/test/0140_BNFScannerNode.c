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

#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include"BNFToken.h"
#include"BNFScannerRule.h"

static CGAppState *appState;

void testNewDelete() {
    printf("%s...\n", __func__);

    BNFScannerNode* node = BNFScannerNode__new(appState, BNFScannerNodeType_string, "", NULL, BNFTokenType_start);
    assert(node != NULL);
    assert(BNFScannerNode_getTokenType(appState, node) == BNFTokenType_start);
    BNFScannerNode_delete(appState, node);

    printf("ok\n");
}

void testApplyStringPattern() {
    printf("%s...\n", __func__);

    BNFScannerNode* node = BNFScannerNode__new(appState, BNFScannerNodeType_string, "abcd", NULL, BNFTokenType_start);
    
    assert(BNFScannerNode_applyToText(appState, node, "abcde") == true);
    assert(BNFScannerNode_applyToText(appState, node, "abcxe") == false);

    BNFScannerNode_delete(appState, node);

    printf("ok\n");
}

void testApplyRegexPattern() {
    printf("%s...\n", __func__);

    BNFScannerNode* node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "abcd", NULL, BNFTokenType_start);
    
    assert(BNFScannerNode_applyToText(appState, node, "abcde") == true);
    assert(BNFScannerNode_applyToText(appState, node, "abcxe") == false);

    BNFScannerNode_delete(appState, node);

    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);

    appState = CGAppState__new();

    testNewDelete();
    testApplyStringPattern();
    testApplyRegexPattern();

    CGAppState_delete(appState);

	return 0;
}
