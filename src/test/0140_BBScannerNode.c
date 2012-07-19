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
#include"BBToken.h"
#include"BBScannerRule.h"
#include"BBException.h"

static CGAppState *appState;

void testNewDelete() {
    printf("%s... ", __func__);

    BBScannerNode* node = BBScannerNode__new(BBScannerNodeType_string, "", NULL, BBTokenType_nonTerminal, false, NULL);
    assert(node != NULL);
    assert(BBScannerNode_getTokenType(node) == BBTokenType_nonTerminal);
    BBScannerNode_delete(node);

    printf("ok -- ");
}

void _helpAssertEqual(BBToken* token, BBToken* token2) {
    assert(token2 != NULL);
    assert(BBToken_isEQual(token, token2));
    BBToken_delete(token2);
}
void testApplyStringPattern() {
    printf("%s... ", __func__);

    BBScannerNode* node = BBScannerNode__new(BBScannerNodeType_string, "abcd", NULL, BBTokenType_nonTerminal, false, NULL);
    BBToken* token = BBToken__new(BBTokenType_nonTerminal, CGString__new("abcd"));
    
    BBToken* token2 = BBScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(token, token2);
    token2 = BBScannerNode_applyToText(node, "abcxe");
    assert(token2 == NULL);

    BBScannerNode_delete(node);

    /* edge case: empty string */
    node = BBScannerNode__new(BBScannerNodeType_string, "", NULL, BBTokenType_nonTerminal, false, NULL);
    token = BBToken__new(BBTokenType_nonTerminal, CGString__new(""));
    token2 = BBScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(token, token2);

    BBScannerNode_delete(node);

    printf("ok -- ");
}

void testApplyRegexPattern() {
    printf("%s... ", __func__);

    BBScannerNode* node = BBScannerNode__new(BBScannerNodeType_regex, "abcd", NULL, BBTokenType_nonTerminal, false, NULL);
    BBToken* token = BBToken__new(BBTokenType_nonTerminal, CGString__new("abcd"));
    
    BBToken* token2 = BBScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(token, token2);
    token2 = BBScannerNode_applyToText(node, "abcxe");
    assert(token2 == NULL);

    BBScannerNode_delete(node);

    node = BBScannerNode__new(BBScannerNodeType_regex, "Y*", NULL, BBTokenType_nonTerminal, false, NULL);
    token = BBToken__new(BBTokenType_nonTerminal, CGString__new(""));
    
    token2 = BBScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(token, token2);

    BBScannerNode_delete(node);

    printf("ok -- ");
}

void testApplyComplexRegexPattern() {
    printf("%s... ", __func__);

    BBScannerNode* node = BBScannerNode__new(BBScannerNodeType_regex, "\\w+", NULL, BBTokenType_nonTerminal, false, NULL);
    BBToken* tokenAbcde = BBToken__new(BBTokenType_nonTerminal, CGString__new("abcde"));
    BBToken* tokenAbcxe = BBToken__new(BBTokenType_nonTerminal, CGString__new("abcxe"));
    BBToken* tokenAbcxe22 = BBToken__new(BBTokenType_nonTerminal, CGString__new("abcxe22"));
    BBToken* tokenAbcxe22fab = BBToken__new(BBTokenType_nonTerminal, CGString__new("abcxe22fab"));
    
    BBToken* token2 = BBScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(tokenAbcde, token2);

    BBScannerNode_delete(node);

    node = BBScannerNode__new(BBScannerNodeType_regex, "[a-z][a-z\\d]+", NULL, BBTokenType_nonTerminal, false, NULL);
    token2 = BBScannerNode_applyToText(node, "abcde");
    _helpAssertEqual(tokenAbcde, token2);
    token2 = BBScannerNode_applyToText(node, "abcxe");
    _helpAssertEqual(tokenAbcxe, token2);
    token2 = BBScannerNode_applyToText(node, "abcxe22");
    _helpAssertEqual(tokenAbcxe22, token2);
    token2 = BBScannerNode_applyToText(node, "abcxe22fab");
    _helpAssertEqual(tokenAbcxe22fab, token2);

    token2 = BBScannerNode_applyToText(node, "22a");
    assert(token2 == NULL);

    BBScannerNode_delete(node);

    /* this should fail because text is too short */
    node = BBScannerNode__new(BBScannerNodeType_regex, "([a-z]{5,10})[a-z\\d]+", NULL, BBTokenType_nonTerminal, false, NULL);
    assert(BBScannerNode_applyToText(node, "abc") == NULL);
    BBScannerNode_delete(node);

    node = BBScannerNode__new(BBScannerNodeType_regex, "([a-z]{5,10})(Y+)([a-z\\d]+)", NULL, BBTokenType_nonTerminal, false, NULL);
    assert(BBScannerNode_applyToText(node, "abcdeYYYa20") != NULL);
    BBScannerNode_delete(node);
    
    /* this should fail w/exception because the regex has a syntax error */
    node = BBScannerNode__new(BBScannerNodeType_regex, "([a-z]{5,10}[a-z\\d]+", NULL, BBTokenType_nonTerminal, false, NULL);
    assert(CGAppState_catchAndDeleteExceptionWithID(appState, BBExceptionID_PCRERegexError) == true);

    printf("ok -- ");
}

BBToken* postProduction(BBToken* token) {
    CGString* text = BBToken_getText(token);
    CGString* newText = CGString_createSubstring(text, 0, CGString_getSize(text)-1);
    return BBToken__new(BBToken_getType(token), newText);
}
int main() {
    printf("=== %s ===\n", __FILE__);

    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();

    testNewDelete();
    testApplyStringPattern();
    testApplyRegexPattern();
    testApplyComplexRegexPattern();

    CGAppState__deInit(appState);

    printf("=== %s === ok\n", __FILE__);
	return 0;
}
