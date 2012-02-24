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

BNFToken* tokenAbcd;
BNFToken* tokenAbcde;
BNFToken* tokenEfgh;
BNFToken* tokenAbcxe;
BNFToken* tokenAbcxe22;
BNFToken* tokenAbcxe22fab;
BNFToken* token123;

void setUp() {
    tokenAbcd = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "abcd"));
    tokenAbcde = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "abcde"));
    tokenEfgh = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "efgh"));
    tokenAbcxe = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "abcxe"));
    tokenAbcxe22 = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "abcxe22"));
    tokenAbcxe22fab = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "abcxe22fab"));
    token123 = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "123"));
}

void _helpAssertEqual(BNFToken* token, BNFToken* token2) {
    assert(token2 != NULL);
    assert(BNFToken_isEQual(appState, token, token2));
}

void testNewDelete() {
    printf("%s...\n", __func__);

    BNFScannerNode* node = BNFScannerNode__new(appState, BNFScannerNodeType_string, "", NULL, BNFTokenType_start);
    BNFScannerRule* rule = BNFScannerRule__new(appState, CGArray__newFromInitializerList(appState, BNFScannerNode, node, NULL));
    assert(rule != NULL);
    assert(CGAppState_catchAndDeleteException(appState) == false);
    assert(CGArray_getSize(appState, BNFScannerNode, rule->nodes) == 1);
    BNFScannerNode_delete(appState, node);
    BNFScannerRule_delete(appState, rule);

    printf("ok\n");
}

void testApplyStringPattern() {
    printf("%s...\n", __func__);

    BNFScannerNode* node1 = BNFScannerNode__new(appState, BNFScannerNodeType_string, "abcd", NULL, BNFTokenType_start);
    BNFScannerNode* node2 = BNFScannerNode__new(appState, BNFScannerNodeType_string, "efgh", NULL, BNFTokenType_start);

    BNFScannerRule* rule = BNFScannerRule__new(appState, CGArray__newFromInitializerList(appState, BNFScannerNode, node1, node2, NULL));
    
    _helpAssertEqual(BNFScannerRule_applyToText(appState, rule, "abcde"), tokenAbcd);
    _helpAssertEqual(BNFScannerRule_applyToText(appState, rule, "efghi"), tokenEfgh);
    assert(BNFScannerRule_applyToText(appState, rule, "xxxefghi") == NULL);

    BNFScannerRule_delete(appState, rule);
    BNFScannerNode_delete(appState, node2);
    BNFScannerNode_delete(appState, node1);

    printf("ok\n");
}

void testApplyRegexPattern() {
    printf("%s...\n", __func__);

    BNFScannerNode* node1 = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "[a-z]+", NULL, BNFTokenType_start);
    BNFScannerNode* node2 = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "[0-9]+", NULL, BNFTokenType_start);

    BNFScannerRule* rule = BNFScannerRule__new(appState, CGArray__newFromInitializerList(appState, BNFScannerNode, node1, node2, NULL));
    
    _helpAssertEqual(BNFScannerRule_applyToText(appState, rule, "abcde"), tokenAbcde);
    _helpAssertEqual(BNFScannerRule_applyToText(appState, rule, "123efghi"), token123);
    assert(BNFScannerRule_applyToText(appState, rule, " xxxefghi") == NULL);

    BNFScannerRule_delete(appState, rule);
    BNFScannerNode_delete(appState, node2);
    BNFScannerNode_delete(appState, node1);
    
    printf("ok\n");
}

int main() {
    setUp();

    printf("=== %s ===\n", __FILE__);

    appState = CGAppState__new(__FILE__);

    testNewDelete();
    testApplyStringPattern();
    testApplyRegexPattern();

    CGAppState_delete(appState);

	return 0;
}
