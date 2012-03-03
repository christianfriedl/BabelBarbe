#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGString.h>
#include"BNFToken.h"
#include"BNFScannerRule.h"
#include"BNFScannerRuleset.h"
#include"BNFException.h"
#include"BNFScanner.h"

static CGAppState *appState;


void testScan() {
    printf("%s... ", __func__);

    CGAppState_catchAndDeleteException(appState);
	BNFScannerRule* startRule = BNFScannerRuleset__getInstance();
	CGString* text = CGString__new("  straw");
	BNFScanner* scanner = BNFScanner__new(startRule, text);
    CGArray(BNFToken)* tokenList = BNFScanner_scanAllTokens(scanner);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 0)) == BNFTokenType_identifier);
    CGString_delete(text);
    CGString_delete(scanner);
    CGString_delete(tokenList);

    CGAppState_catchAndDeleteException(appState);
	text = CGString__new("straw");
	scanner = BNFScanner__new(startRule, text);
    tokenList = BNFScanner_scanAllTokens(scanner);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 0)) == BNFTokenType_identifier);
    CGString_delete(text);
    CGString_delete(scanner);
    CGString_delete(tokenList);

    printf("%s ok -- ", __func__);
}

void testScan2() {
    printf("%s... ", __func__);

    CGAppState_catchAndDeleteException(appState);
	BNFScannerRule* startRule = BNFScannerRuleset__getInstance();
	CGString* text = CGString__new("straw ::= stem ;");
	BNFScanner* scanner = BNFScanner__new(startRule, text);
    CGArray(BNFToken)* tokenList = BNFScanner_scanAllTokens(scanner);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 0)) == BNFTokenType_identifier);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 1)) == BNFTokenType_definitionSign);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 2)) == BNFTokenType_identifier);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 3)) == BNFTokenType_semicolon);
    CGString_delete(text);
    CGString_delete(scanner);
    CGString_delete(tokenList);

    printf("%s ok -- ", __func__);
}

void testScan3() {
    printf("%s... ", __func__);

    CGAppState_catchAndDeleteException(appState);
	BNFScannerRule* startRule = BNFScannerRuleset__getInstance();
	CGString* text = CGString__new("straw ::= stem ; stem ::= (leaf haulm)* ; leaf ::= \"LEAF\" ; haulm ::= /[a-f]+[0-3]*/ ;");
	BNFScanner* scanner = BNFScanner__new(startRule, text);
    CGArray(BNFToken)* tokenList = BNFScanner_scanAllTokens(scanner);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 0)) == BNFTokenType_identifier);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 1)) == BNFTokenType_definitionSign);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 2)) == BNFTokenType_identifier);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 3)) == BNFTokenType_semicolon);

    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 4)) == BNFTokenType_identifier);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 5)) == BNFTokenType_definitionSign);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 6)) == BNFTokenType_openParen);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 7)) == BNFTokenType_identifier);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 8)) == BNFTokenType_identifier);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 9)) == BNFTokenType_closeParen);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 10)) == BNFTokenType_repeatZeroOrMore);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 11)) == BNFTokenType_semicolon);

    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 12)) == BNFTokenType_identifier);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 13)) == BNFTokenType_definitionSign);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 14)) == BNFTokenType_stringLiteral);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 15)) == BNFTokenType_semicolon);

    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 16)) == BNFTokenType_identifier);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 17)) == BNFTokenType_definitionSign);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 18)) == BNFTokenType_regexLiteral);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 19)) == BNFTokenType_semicolon);
    CGString_delete(text);
    CGString_delete(scanner);
    CGString_delete(tokenList);

    printf("%s ok -- ", __func__);
}

void testScanWithNoiseAtEnd() {
    printf("%s... ", __func__);

    CGAppState_catchAndDeleteException(appState);
	BNFScannerRule* startRule = BNFScannerRuleset__getInstance();
	CGString* text = CGString__new("  straw ::=\tirxn \n; ");
	BNFScanner* scanner = BNFScanner__new(startRule, text);
    CGArray(BNFToken)* tokenList = BNFScanner_scanAllTokens(scanner);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 0)) == BNFTokenType_identifier);
    int i;
    for (i=0;i<CGArray_getSize(BNFToken, tokenList); ++i)
        printf("%s: '%s'\n", BNFToken_getTypeName(CGArray_getValueAt(BNFToken, tokenList, i)), BNFToken_getText(CGArray_getValueAt(BNFToken, tokenList, i)));
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
