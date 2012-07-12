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
    printf("%s... ", __func__);

    BNFScannerNode* node = BNFScannerNode__new(BNFScannerNodeType_string, "", NULL, BNFTokenType_nonTerminal, false, NULL);
    assert(node != NULL);
    assert(BNFScannerNode_getTokenType(node) == BNFTokenType_nonTerminal);
    BNFScannerNode_delete(node);

    printf("ok -- ");
}

void _helpAssertEqual(BNFToken* token, BNFToken* token2) {
    assert(token2 != NULL);
    assert(BNFToken_isEQual(token, token2));
    BNFToken_delete(token2);
}
void testApplyStringPattern() {
    printf("%s... ", __func__);

    BNFScannerNode* node = BNFScannerNode__new(BNFScannerNodeType_string, "abcd", NULL, BNFTokenType_nonTerminal, false, NULL);
    BNFToken* token = BNFToken__new(BNFTokenType_nonTerminal, CGString__new("abcd"));
    
    BNFToken* token2 = BNFScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(token, token2);
    token2 = BNFScannerNode_applyToText(node, "abcxe");
    assert(token2 == NULL);

    BNFScannerNode_delete(node);

    /* edge case: empty string */
    node = BNFScannerNode__new(BNFScannerNodeType_string, "", NULL, BNFTokenType_nonTerminal, false, NULL);
    token = BNFToken__new(BNFTokenType_nonTerminal, CGString__new(""));
    token2 = BNFScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(token, token2);

    BNFScannerNode_delete(node);

    printf("ok -- ");
}

void testApplyRegexPattern() {
    printf("%s... ", __func__);

    BNFScannerNode* node = BNFScannerNode__new(BNFScannerNodeType_regex, "abcd", NULL, BNFTokenType_nonTerminal, false, NULL);
    BNFToken* token = BNFToken__new(BNFTokenType_nonTerminal, CGString__new("abcd"));
    
    BNFToken* token2 = BNFScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(token, token2);
    token2 = BNFScannerNode_applyToText(node, "abcxe");
    assert(token2 == NULL);

    BNFScannerNode_delete(node);

    node = BNFScannerNode__new(BNFScannerNodeType_regex, "Y*", NULL, BNFTokenType_nonTerminal, false, NULL);
    token = BNFToken__new(BNFTokenType_nonTerminal, CGString__new(""));
    
    token2 = BNFScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(token, token2);

    BNFScannerNode_delete(node);

    printf("ok -- ");
}

void testApplyComplexRegexPattern() {
    printf("%s... ", __func__);

    BNFScannerNode* node = BNFScannerNode__new(BNFScannerNodeType_regex, "\\w+", NULL, BNFTokenType_nonTerminal, false, NULL);
    BNFToken* tokenAbcde = BNFToken__new(BNFTokenType_nonTerminal, CGString__new("abcde"));
    BNFToken* tokenAbcxe = BNFToken__new(BNFTokenType_nonTerminal, CGString__new("abcxe"));
    BNFToken* tokenAbcxe22 = BNFToken__new(BNFTokenType_nonTerminal, CGString__new("abcxe22"));
    BNFToken* tokenAbcxe22fab = BNFToken__new(BNFTokenType_nonTerminal, CGString__new("abcxe22fab"));
    
    BNFToken* token2 = BNFScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(tokenAbcde, token2);

    BNFScannerNode_delete(node);

    node = BNFScannerNode__new(BNFScannerNodeType_regex, "[a-z][a-z\\d]+", NULL, BNFTokenType_nonTerminal, false, NULL);
    token2 = BNFScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(tokenAbcde, token2);
    token2 = BNFScannerNode_applyToText(node, "abcxe");
    _helpAssertEqual(tokenAbcxe, token2);
    token2 = BNFScannerNode_applyToText(node, "abcxe22");
    _helpAssertEqual(tokenAbcxe22, token2);
    token2 = BNFScannerNode_applyToText(node, "abcxe22fab");
    _helpAssertEqual(tokenAbcxe22fab, token2);

    token2 = BNFScannerNode_applyToText(node, "22a");
    assert(token2 == NULL);

    BNFScannerNode_delete(node);

    /* this should fail because text is too short */
    node = BNFScannerNode__new(BNFScannerNodeType_regex, "([a-z]{5,10})[a-z\\d]+", NULL, BNFTokenType_nonTerminal, false, NULL);
    assert(BNFScannerNode_applyToText(node, "abc") == NULL);
    BNFScannerNode_delete(node);

    node = BNFScannerNode__new(BNFScannerNodeType_regex, "([a-z]{5,10})(Y+)([a-z\\d]+)", NULL, BNFTokenType_nonTerminal, false, NULL);
    assert(BNFScannerNode_applyToText(node, "abcdeYYYa20") != NULL);
    BNFScannerNode_delete(node);
    
    /* this should fail w/exception because the regex has a syntax error */
    node = BNFScannerNode__new(BNFScannerNodeType_regex, "([a-z]{5,10}[a-z\\d]+", NULL, BNFTokenType_nonTerminal, false, NULL);
    assert(CGAppState_catchAndDeleteExceptionWithID(appState, BNFExceptionID_PCRERegexError) == true);

    printf("ok -- ");
}

BNFToken* postProduction(BNFToken* token) {
    CGString* text = BNFToken_getText(token);
    CGString* newText = CGString_createSubstring(text, 0, CGString_getSize(text)-1);
    return BNFToken__new(BNFToken_getType(token), newText);
}
void testApplyStringPatternWithOnAfterScanFunction() {
    printf("%s... ", __func__);

    BNFScannerNode* node = BNFScannerNode__new(BNFScannerNodeType_string, "abcde", NULL, BNFTokenType_nonTerminal, false, postProduction);
    BNFToken* token = BNFToken__new(BNFTokenType_nonTerminal, CGString__new("abcd"));
    
    BNFToken* token2 = BNFScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(token, token2);

    BNFScannerNode_delete(node);

    printf("ok -- ");
}

int main() {
    printf("=== %s ===\n", __FILE__);

    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();

    testNewDelete();
    testApplyStringPattern();
    testApplyRegexPattern();
    testApplyComplexRegexPattern();
    testApplyStringPatternWithOnAfterScanFunction();

    CGAppState__deInit(appState);

    printf("=== %s === ok\n", __FILE__);
	return 0;
}
