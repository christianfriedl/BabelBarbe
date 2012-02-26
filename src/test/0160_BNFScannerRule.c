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
    tokenAbcd = BNFToken__new(BNFTokenType_start, CGString__new("abcd"));
    tokenAbcde = BNFToken__new(BNFTokenType_start, CGString__new("abcde"));
    tokenEfgh = BNFToken__new(BNFTokenType_start, CGString__new("efgh"));
    tokenAbcxe = BNFToken__new(BNFTokenType_start, CGString__new("abcxe"));
    tokenAbcxe22 = BNFToken__new(BNFTokenType_start, CGString__new("abcxe22"));
    tokenAbcxe22fab = BNFToken__new(BNFTokenType_start, CGString__new("abcxe22fab"));
    token123 = BNFToken__new(BNFTokenType_start, CGString__new("123"));
}

void _helpAssertEqual(BNFToken* token, BNFToken* token2) {
    assert(token2 != NULL);
    assert(BNFToken_isEQual(token, token2));
}

void testNewDelete() {
    printf("%s...\n", __func__);

    BNFScannerNode* node = BNFScannerNode__new(BNFScannerNodeType_string, "", NULL, BNFTokenType_start, false);
    BNFScannerRule* rule = BNFScannerRule__new(CGArray__newFromInitializerList(BNFScannerNode, node, NULL));
    assert(rule != NULL);
    assert(CGAppState_catchAndDeleteException(appState) == false);
    assert(CGArray_getSize(BNFScannerNode, rule->nodes) == 1);
    BNFScannerNode_delete(node);
    BNFScannerRule_delete(rule);

    printf("ok\n");
}

void testApplyStringPattern() {
    printf("%s...\n", __func__);

    BNFScannerNode* node1 = BNFScannerNode__new(BNFScannerNodeType_string, "abcd", NULL, BNFTokenType_start, false);
    BNFScannerNode* node2 = BNFScannerNode__new(BNFScannerNodeType_string, "efgh", NULL, BNFTokenType_start, false);

    BNFScannerRule* rule = BNFScannerRule__new(CGArray__newFromInitializerList(BNFScannerNode, node1, node2, NULL));
    
    _helpAssertEqual(BNFScannerRule_applyToText(rule, "abcde"), tokenAbcd);
    _helpAssertEqual(BNFScannerRule_applyToText(rule, "efghi"), tokenEfgh);
    assert(BNFScannerRule_applyToText(rule, "xxxefghi") == NULL);

    BNFScannerRule_delete(rule);
    BNFScannerNode_delete(node2);
    BNFScannerNode_delete(node1);

    printf("ok\n");
}

void testApplyRegexPattern() {
    printf("%s...\n", __func__);

    BNFScannerNode* node1 = BNFScannerNode__new(BNFScannerNodeType_regex, "[a-z]+", NULL, BNFTokenType_start, false);
    BNFScannerNode* node2 = BNFScannerNode__new(BNFScannerNodeType_regex, "[0-9]+", NULL, BNFTokenType_start, false);

    BNFScannerRule* rule = BNFScannerRule__new(CGArray__newFromInitializerList(BNFScannerNode, node1, node2, NULL));
    
    _helpAssertEqual(BNFScannerRule_applyToText(rule, "abcde"), tokenAbcde);
    _helpAssertEqual(BNFScannerRule_applyToText(rule, "123efghi"), token123);
    assert(BNFScannerRule_applyToText(rule, " xxxefghi") == NULL);

    BNFScannerRule_delete(rule);
    BNFScannerNode_delete(node2);
    BNFScannerNode_delete(node1);
    
    printf("ok\n");
}

int main() {
    setUp();

    printf("=== %s ===\n", __FILE__);

    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();

    testNewDelete();
    testApplyStringPattern();
    testApplyRegexPattern();

    CGAppState__deInit();

	return 0;
}
