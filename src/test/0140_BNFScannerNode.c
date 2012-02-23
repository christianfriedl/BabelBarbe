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
#include"BNFException.h"

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

void testApplyComplexRegexPattern() {
    printf("%s...\n", __func__);

    BNFScannerNode* node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "\\w+", NULL, BNFTokenType_start);
    
    assert(BNFScannerNode_applyToText(appState, node, "abcde") == true);
    assert(BNFScannerNode_applyToText(appState, node, "abcxe") == true);

    BNFScannerNode_delete(appState, node);

    node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "[a-z][a-z\\d]+", NULL, BNFTokenType_start);
    assert(BNFScannerNode_applyToText(appState, node, "abcde") == true);
    assert(BNFScannerNode_applyToText(appState, node, "abcxe") == true);
    assert(BNFScannerNode_applyToText(appState, node, "abcxe22") == true);
    assert(BNFScannerNode_applyToText(appState, node, "abcxe22fab") == true);
    assert(BNFScannerNode_applyToText(appState, node, "22a") == false);

    BNFScannerNode_delete(appState, node);

    node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "([a-z]{5,10})[a-z\\d]+", NULL, BNFTokenType_start);
    assert(BNFScannerNode_applyToText(appState, node, "abc") == false);
    BNFScannerNode_delete(appState, node);

    node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "([a-z]{5,10})(Y+)([a-z\\d]+)", NULL, BNFTokenType_start);
    assert(BNFScannerNode_applyToText(appState, node, "abcdeYYYa20") == false);
    assert(CGAppState_catchAndDeleteExceptionWithID(appState, BNFExceptionID_PCRERegexError) == true);
    BNFScannerNode_delete(appState, node);
    
    node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "([a-z]{5,10}[a-z\\d]+", NULL, BNFTokenType_start);
    assert(CGAppState_catchAndDeleteExceptionWithID(appState, BNFExceptionID_PCRERegexError) == true);

    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);

    appState = CGAppState__new();

    testNewDelete();
    testApplyStringPattern();
    testApplyRegexPattern();
    testApplyComplexRegexPattern();

    CGAppState_delete(appState);

	return 0;
}
