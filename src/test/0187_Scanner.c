#include<assert.h>
#include<stdio.h>
#include<string.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGString.h>
#include"CGArrayOfBNFScannerNode.h"
#include"BNFScannerRule.h"
#include"BNFScanner.h"

CGAppState* appState;

void testNewDelete() {
    printf("%s...\n", __func__);
    CGString* text = CGString__new(appState, "");
    CGArray(BNFScannerNode)* nodes = CGArray__newFromInitializerList(appState, BNFScannerNode, NULL);
    BNFScannerRule* rule = BNFScannerRule__new(appState, nodes);
    BNFScanner* scanner = BNFScanner__new(appState, rule, text);
    BNFScanner_delete(appState, scanner);
    printf("ok\n");
}

void testIdentifier() {
    printf("%s...\n", __func__);

    char* text = CGString__new(appState, "abcde");
    CGArray(BNFScannerNode)* nodes = CGArray__newFromInitializerList(appState, BNFScannerNode, BNFScannerNode__new(appState, BNFScannerNodeType_regex, "\\w+", NULL, BNFTokenType_identifier, false), NULL);
    BNFScannerRule* rule = BNFScannerRule__new(appState, nodes);
    BNFScanner* scanner = BNFScanner__new(appState, rule, text);
    BNFToken* token = BNFScanner_scanNextToken(appState, scanner);
    assert(token != NULL);
    assert(BNFToken_getType(appState, token) == BNFTokenType_identifier);
    CGString* tokenText = BNFToken_getText(appState, token);
    assert(!CGString__compare(appState, tokenText, text));
    assert(BNFScanner_scanNextToken(appState, scanner) == NULL);
    BNFToken_delete(appState, token);
    BNFScanner_delete(appState, scanner);

    printf("ok\n");
}
void testIdentifierWithError() {
    printf("%s...\n", __func__);

    CGString* text = CGString__new(appState, "abcde123");
    CGString* identText = CGString__new(appState, "abcde");
    CGArray(BNFScannerNode)* nodes = CGArray__newFromInitializerList(appState, BNFScannerNode, BNFScannerNode__new(appState, BNFScannerNodeType_regex, "[a-z]+", NULL, BNFTokenType_identifier, false), NULL);
    BNFScannerRule* rule = BNFScannerRule__new(appState, nodes);
    BNFScanner* scanner = BNFScanner__new(appState, rule, text);
    BNFToken* token = BNFScanner_scanNextToken(appState, scanner);
    assert(token != NULL);
    assert(BNFToken_getType(appState, token) == BNFTokenType_identifier);
    CGString* tokenText = BNFToken_getText(appState, token);
    assert(!CGString__compare(appState, tokenText, identText));
    assert(BNFScanner_scanNextToken(appState, scanner) == NULL);
    assert(CGAppState_catchAndDeleteExceptionWithID(appState, BNFExceptionID_ScannerError) == true);
    BNFToken_delete(appState, token);
    BNFScanner_delete(appState, scanner);

    printf("ok\n");
}
void testComplexRuleset() {
    printf("%s...\n", __func__);

    CGString* text = CGString__new(appState, "abcde ::= 123");
    CGString* identText = CGString__new(appState, "abcde");
    CGArray(BNFScannerNode)* nodes = CGArray__newFromInitializerList(appState, BNFScannerNode, BNFScannerNode__new(appState, BNFScannerNodeType_regex, "[a-z]+", NULL, BNFTokenType_identifier, false), NULL);
    BNFScannerRule* rule = BNFScannerRule__new(appState, nodes);
    BNFScanner* scanner = BNFScanner__new(appState, rule, text);
    BNFToken* token = BNFScanner_scanNextToken(appState, scanner);
    assert(token != NULL);
    assert(BNFToken_getType(appState, token) == BNFTokenType_identifier);
    CGString* tokenText = BNFToken_getText(appState, token);
    assert(!CGString__compare(appState, tokenText, identText));
    assert(BNFScanner_scanNextToken(appState, scanner) == NULL);
    assert(CGAppState_catchAndDeleteExceptionWithID(appState, BNFExceptionID_ScannerError) == true);
    BNFToken_delete(appState, token);
    BNFScanner_delete(appState, scanner);

    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);
    
    appState = CGAppState__new(__FILE__);

    testNewDelete();
    testIdentifier();
    testIdentifierWithError();

	return 0;
}
