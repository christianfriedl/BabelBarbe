#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGString.h>
#include"BBToken.h"
#include"BBScannerRule.h"
#include"BBScannerRuleset.h"
#include"BBException.h"
#include"BBScanner.h"

static CGAppState *appState;


void testScan() {
    printf("%s... ", __func__);

    CGAppState_reset(appState);
	BBScannerRule* startRule = BBScannerRuleset__getInstance();
	CGString* text = CGString__new("  straw");
	BBScanner* scanner = BBScanner__new(startRule, text);
    CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 0)) == BBTokenType_identifier);
    CGString_delete(text);
    CGString_delete(scanner);
    CGString_delete(tokenList);

    CGAppState_reset(appState);
	text = CGString__new("straw");
	scanner = BBScanner__new(startRule, text);
    tokenList = BBScanner_scanAllTokens(scanner);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 0)) == BBTokenType_identifier);
    CGString_delete(text);
    CGString_delete(scanner);
    CGString_delete(tokenList);

    printf("%s ok -- ", __func__);
}

void testScan2() {
    printf("%s... ", __func__);

    CGAppState_reset(appState);
	BBScannerRule* startRule = BBScannerRuleset__getInstance();
	CGString* text = CGString__new("straw ::= stem ;");
	BBScanner* scanner = BBScanner__new(startRule, text);
    CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 0)) == BBTokenType_identifier);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 1)) == BBTokenType_definitionSign);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 2)) == BBTokenType_identifier);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 3)) == BBTokenType_semicolon);
    CGString_delete(text);
    CGString_delete(scanner);
    CGString_delete(tokenList);

    printf("%s ok -- ", __func__);
}

void testScan3() {
    printf("%s... ", __func__);

    CGAppState_reset(appState);
	BBScannerRule* startRule = BBScannerRuleset__getInstance();
	CGString* text = CGString__new("straw ::= stem ; stem ::= (leaf haulm)* ; leaf ::= 'LEAF' ; haulm ::= /[a-f]+[0-3]*/ ;");
	BBScanner* scanner = BBScanner__new(startRule, text);
    CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 0)) == BBTokenType_identifier);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 1)) == BBTokenType_definitionSign);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 2)) == BBTokenType_identifier);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 3)) == BBTokenType_semicolon);

    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 4)) == BBTokenType_identifier);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 5)) == BBTokenType_definitionSign);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 6)) == BBTokenType_openParen);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 7)) == BBTokenType_identifier);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 8)) == BBTokenType_identifier);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 9)) == BBTokenType_closeParen);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 10)) == BBTokenType_repeatZeroOrMore);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 11)) == BBTokenType_semicolon);

    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 12)) == BBTokenType_identifier);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 13)) == BBTokenType_definitionSign);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 14)) == BBTokenType_stringLiteral);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 15)) == BBTokenType_semicolon);

    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 16)) == BBTokenType_identifier);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 17)) == BBTokenType_definitionSign);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 18)) == BBTokenType_regexLiteral);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 19)) == BBTokenType_semicolon);
    CGString_delete(text);
    CGString_delete(scanner);
    CGString_delete(tokenList);

    printf("%s ok -- ", __func__);
}

void testScanWithNoiseAtEnd() {
    printf("%s... ", __func__);

    CGAppState_reset(appState);
	BBScannerRule* startRule = BBScannerRuleset__getInstance();
	CGString* text = CGString__new("  straw ::=\tirxn \n; ");
	BBScanner* scanner = BBScanner__new(startRule, text);
    CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    assert(BBToken_getType(CGArray_getValueAt(BBToken, tokenList, 0)) == BBTokenType_identifier);
    int i;
    for (i=0;i<CGArray_getSize(BBToken, tokenList); ++i)
        printf("%s: '%s'\n", BBToken_getTypeName(CGArray_getValueAt(BBToken, tokenList, i)), BBToken_getText(CGArray_getValueAt(BBToken, tokenList, i)));
    CGString_delete(text);
    CGString_delete(scanner);
    CGString_delete(tokenList);

    printf("%s ok -- ", __func__);
}

int main() {
    printf("=== %s ===\n", __FILE__);

    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();

    testScan();
    testScan2();
    testScan3();
    testScanWithNoiseAtEnd();

    CGAppState__deInit();

	return 0;
}
