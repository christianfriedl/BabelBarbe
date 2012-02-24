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

void _helpAssertEqual(BNFToken* token, BNFToken* token2) {
    assert(token2 != NULL);
    assert(BNFToken_isEQual(appState, token, token2));
    BNFToken_delete(appState, token2);
}
void testApplyStringPattern() {
    printf("%s...\n", __func__);

    BNFScannerNode* node = BNFScannerNode__new(appState, BNFScannerNodeType_string, "abcd", NULL, BNFTokenType_start);
    BNFToken* token = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "abcd"));
    
    BNFToken* token2 = BNFScannerNode_applyToText(appState, node, "abcde");
    _helpAssertEqual(token, token2);
    token2 = BNFScannerNode_applyToText(appState, node, "abcxe");
    assert(token2 == NULL);

    BNFScannerNode_delete(appState, node);

    printf("ok\n");
}

void testApplyRegexPattern() {
    printf("%s...\n", __func__);

    BNFScannerNode* node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "abcd", NULL, BNFTokenType_start);
    BNFToken* token = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "abcd"));
    
    BNFToken* token2 = BNFScannerNode_applyToText(appState, node, "abcde");
    _helpAssertEqual(token, token2);
    token2 = BNFScannerNode_applyToText(appState, node, "abcxe");
    assert(token2 == NULL);

    BNFScannerNode_delete(appState, node);

    printf("ok\n");
}

void testApplyComplexRegexPattern() {
    printf("%s...\n", __func__);

    BNFScannerNode* node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "\\w+", NULL, BNFTokenType_start);
    BNFToken* tokenAbcde = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "abcde"));
    BNFToken* tokenAbcxe = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "abcxe"));
    BNFToken* tokenAbcxe22 = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "abcxe22"));
    BNFToken* tokenAbcxe22fab = BNFToken__new(appState, BNFTokenType_start, CGString__new(appState, "abcxe22fab"));
    
    BNFToken* token2 = BNFScannerNode_applyToText(appState, node, "abcde");
    _helpAssertEqual(tokenAbcde, token2);

    BNFScannerNode_delete(appState, node);

    node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "[a-z][a-z\\d]+", NULL, BNFTokenType_start);
    token2 = BNFScannerNode_applyToText(appState, node, "abcde");
    _helpAssertEqual(tokenAbcde, token2);
    token2 = BNFScannerNode_applyToText(appState, node, "abcxe");
    _helpAssertEqual(tokenAbcxe, token2);
    token2 = BNFScannerNode_applyToText(appState, node, "abcxe22");
    _helpAssertEqual(tokenAbcxe22, token2);
    token2 = BNFScannerNode_applyToText(appState, node, "abcxe22fab");
    _helpAssertEqual(tokenAbcxe22fab, token2);

    token2 = BNFScannerNode_applyToText(appState, node, "22a");
    assert(token2 == NULL);

    BNFScannerNode_delete(appState, node);

    /* this should fail because text is too short */
    node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "([a-z]{5,10})[a-z\\d]+", NULL, BNFTokenType_start);
    assert(BNFScannerNode_applyToText(appState, node, "abc") == NULL);
    BNFScannerNode_delete(appState, node);

    /* this should fail w/exception because we cannot deal with more than one parentheses pair */
    node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "([a-z]{5,10})(Y+)([a-z\\d]+)", NULL, BNFTokenType_start);
    assert(BNFScannerNode_applyToText(appState, node, "abcdeYYYa20") == NULL);
    assert(CGAppState_catchAndDeleteExceptionWithID(appState, BNFExceptionID_PCRERegexError) == true);
    BNFScannerNode_delete(appState, node);
    
    /* this should fail w/exception because the regex has a syntax error */
    node = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "([a-z]{5,10}[a-z\\d]+", NULL, BNFTokenType_start);
    assert(CGAppState_catchAndDeleteExceptionWithID(appState, BNFExceptionID_PCRERegexError) == true);

    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);

    appState = CGAppState__new(__FILE__);

    testNewDelete();
    testApplyStringPattern();
    testApplyRegexPattern();
    testApplyComplexRegexPattern();

    CGAppState_delete(appState);

	return 0;
}
