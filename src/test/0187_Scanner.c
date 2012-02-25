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
void testNoise() {
    printf("%s...\n", __func__);

    CGString* text = CGString__new(appState, "     ");
    CGArray(BNFScannerNode)* nodes = CGArray__newFromInitializerList(appState, BNFScannerNode,
                                            BNFScannerNode__new(appState, BNFScannerNodeType_regex, "\\s*", NULL, BNFTokenType_noise, true),
                                            NULL);
    BNFScannerRule* rule = BNFScannerRule__new(appState, nodes);
    BNFScanner* scanner = BNFScanner__new(appState, rule, text);
    BNFToken* token = BNFScanner_scanNextToken(appState, scanner);
    assert(token != NULL);
    assert(BNFToken_getType(appState, token) == BNFTokenType_noise);
    BNFToken_delete(appState, token);
    token = BNFScanner_scanNextToken(appState, scanner);
    assert(token == NULL);
    assert(CGAppState_isExceptionRaised(appState) == false);
    BNFScanner_delete(appState, scanner);
}
void testComplexRuleset() {
    printf("%s...\n", __func__);

    CGString* text = CGString__new(appState, "abcde ::= fghi;");
    CGArray(BNFScannerNode)* startNodes;
    CGArray(BNFScannerNode)* noiseNodes;
    noiseNodes = CGArray__new(appState, BNFScannerNode, 1);
    BNFScannerRule* startRule;
    BNFScannerRule* noiseRule = BNFScannerRule__new(appState, noiseNodes);
    startNodes = CGArray__newFromInitializerList(appState, BNFScannerNode, 
                        BNFScannerNode__new(appState, BNFScannerNodeType_regex, "[a-z]+", noiseRule, BNFTokenType_identifier, false), 
                        BNFScannerNode__new(appState, BNFScannerNodeType_string, "::=", noiseRule, BNFTokenType_definition, false), 
                        BNFScannerNode__new(appState, BNFScannerNodeType_string, ";", noiseRule, BNFTokenType_semicolon, false), 
                        NULL);
    startRule = BNFScannerRule__new(appState, startNodes);
    CGArray_add(appState, BNFScannerNode, noiseNodes, BNFScannerNode__new(appState, BNFScannerNodeType_regex, "\\s*", startRule, BNFTokenType_noise, true));

    BNFScanner* scanner = BNFScanner__new(appState, startRule, text);
    BNFToken* token = BNFScanner_scanNextToken(appState, scanner);
    assert(token != NULL);
    assert(BNFToken_getType(appState, token) == BNFTokenType_identifier);
    BNFToken_delete(appState, token);
    token = BNFScanner_scanNextToken(appState, scanner);
    assert(token != NULL);
    assert(BNFToken_getType(appState, token) == BNFTokenType_noise);
    BNFToken_delete(appState, token);
    token = BNFScanner_scanNextToken(appState, scanner);
    assert(BNFToken_getType(appState, token) == BNFTokenType_definition);
    BNFToken_delete(appState, token);
    token = BNFScanner_scanNextToken(appState, scanner);
    assert(token != NULL);
    assert(BNFToken_getType(appState, token) == BNFTokenType_noise);
    BNFToken_delete(appState, token);
    token = BNFScanner_scanNextToken(appState, scanner);
    assert(token != NULL);
    assert(BNFToken_getType(appState, token) == BNFTokenType_identifier);
    BNFToken_delete(appState, token);
    token = BNFScanner_scanNextToken(appState, scanner);
    assert(token != NULL);
    assert(BNFToken_getType(appState, token) == BNFTokenType_noise);
    BNFToken_delete(appState, token);
    token = BNFScanner_scanNextToken(appState, scanner);
    assert(token != NULL);
    assert(BNFToken_getType(appState, token) == BNFTokenType_semicolon);
    BNFToken_delete(appState, token);
    token = BNFScanner_scanNextToken(appState, scanner);
    assert(token == NULL);
    BNFScanner_delete(appState, scanner);

    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);
    
    appState = CGAppState__new(__FILE__);

    testNewDelete();
    testIdentifier();
    testIdentifierWithError();
    testNoise();
    testComplexRuleset();

	return 0;
}
