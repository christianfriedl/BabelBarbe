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
    printf("startRule = %ld\n", startRule);
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

    CGAppState_catchAndDeleteException(appState);
	text = CGString__new("straw ::= stem ;");
	scanner = BNFScanner__new(startRule, text);
    tokenList = BNFScanner_scanAllTokens(scanner);
    assert(BNFToken_getType(CGArray_getValueAt(BNFToken, tokenList, 0)) == BNFTokenType_identifier);
    CGString_delete(text);
    CGString_delete(scanner);
    CGString_delete(tokenList);

    printf("ok -- ");
}

int main() {
    printf("=== %s ===\n", __FILE__);

    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();

    testScan();

    CGAppState__deInit();

	return 0;
}
