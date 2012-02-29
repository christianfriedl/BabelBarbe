#include<assert.h>
#include<stdio.h>
#include<string.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGString.h>
#include<cgenerics/CGArray.h>
#include"CGArrayOfBNFScannerNode.h"
#include"BNFToken.h"
#include"BNFScannerRule.h"
#include"BNFScanner.h"

CGAppState* appState;

void testNewDelete() {
    printf("%s... ", __func__);
    CGString* text = CGString__new("");
    CGArray(BNFScannerNode)* nodes = CGArray__newFromInitializerList(BNFScannerNode, NULL);
    BNFScannerRule* rule = BNFScannerRule__new(nodes);
    BNFScanner* scanner = BNFScanner__new(rule, text);
    BNFScanner_delete(scanner);
    printf("ok -- ");
}

void testIdentifier() {
    printf("%s... ", __func__);

    char* text = CGString__new("abcde");
    CGArray(BNFScannerNode)* nodes = CGArray__newFromInitializerList(BNFScannerNode, BNFScannerNode__new(BNFScannerNodeType_regex, "\\w+", NULL, BNFTokenType_identifier, false), NULL);
    BNFScannerRule* rule = BNFScannerRule__new(nodes);
    BNFScanner* scanner = BNFScanner__new(rule, text);
    BNFToken* token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BNFToken_getType(token) == BNFTokenType_identifier);
    CGString* tokenText = BNFToken_getText(token);
    assert(!CGString__compare(tokenText, text));
    assert(BNFScanner_scanNextToken(scanner) == NULL);
    BNFToken_delete(token);
    BNFScanner_delete(scanner);

    printf("ok -- ");
}
void testIdentifierWithError() {
    printf("%s... ", __func__);

    CGString* text = CGString__new("abcde123");
    CGString* identText = CGString__new("abcde");
    CGArray(BNFScannerNode)* nodes = CGArray__newFromInitializerList(BNFScannerNode, BNFScannerNode__new(BNFScannerNodeType_regex, "[a-z]+", NULL, BNFTokenType_identifier, false), NULL);
    BNFScannerRule* rule = BNFScannerRule__new(nodes);
    BNFScanner* scanner = BNFScanner__new(rule, text);
    BNFToken* token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BNFToken_getType(token) == BNFTokenType_identifier);
    CGString* tokenText = BNFToken_getText(token);
    assert(!CGString__compare(tokenText, identText));
    assert(BNFScanner_scanNextToken(scanner) == NULL);
    assert(CGAppState_catchAndDeleteExceptionWithID(appState, BNFExceptionID_ScannerError) == true);
    BNFToken_delete(token);
    BNFScanner_delete(scanner);

    printf("ok -- ");
}
void testNoise() {
    printf("%s... ", __func__);

    CGString* text = CGString__new("     ");
    CGArray(BNFScannerNode)* nodes = CGArray__newFromInitializerList(BNFScannerNode,
                                            BNFScannerNode__new(BNFScannerNodeType_regex, "\\s*", NULL, BNFTokenType_noise, true),
                                            NULL);
    BNFScannerRule* rule = BNFScannerRule__new(nodes);
    BNFScanner* scanner = BNFScanner__new(rule, text);
    BNFToken* token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BNFToken_getType(token) == BNFTokenType_noise);
    BNFToken_delete(token);
    token = BNFScanner_scanNextToken(scanner);
    assert(token == NULL);
    assert(CGAppState_isExceptionRaised(appState) == false);
    BNFScanner_delete(scanner);
}
void testComplexRuleset() {
    printf("%s... ", __func__);

    CGString* text = CGString__new("abcde ::= fghi;");
    CGArray(BNFScannerNode)* startNodes;
    CGArray(BNFScannerNode)* noiseNodes;
    noiseNodes = CGArray__new(BNFScannerNode, 1);
    BNFScannerRule* startRule;
    BNFScannerRule* noiseRule = BNFScannerRule__new(noiseNodes);
    startNodes = CGArray__newFromInitializerList(BNFScannerNode, 
                        BNFScannerNode__new(BNFScannerNodeType_regex, "[a-z]+", noiseRule, BNFTokenType_identifier, false), 
                        BNFScannerNode__new(BNFScannerNodeType_string, "::=", noiseRule, BNFTokenType_definition, false), 
                        BNFScannerNode__new(BNFScannerNodeType_string, ";", noiseRule, BNFTokenType_semicolon, false), 
                        NULL);
    startRule = BNFScannerRule__new(startNodes);
    CGArray_add(BNFScannerNode, noiseNodes, BNFScannerNode__new(BNFScannerNodeType_regex, "\\s*", startRule, BNFTokenType_noise, true));

    BNFScanner* scanner = BNFScanner__new(startRule, text);
    BNFToken* token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BNFToken_getType(token) == BNFTokenType_identifier);
    BNFToken_delete(token);
    token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BNFToken_getType(token) == BNFTokenType_noise);
    BNFToken_delete(token);
    token = BNFScanner_scanNextToken(scanner);
    assert(BNFToken_getType(token) == BNFTokenType_definition);
    BNFToken_delete(token);
    token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BNFToken_getType(token) == BNFTokenType_noise);
    BNFToken_delete(token);
    token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BNFToken_getType(token) == BNFTokenType_identifier);
    BNFToken_delete(token);
    token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BNFToken_getType(token) == BNFTokenType_noise);
    BNFToken_delete(token);
    token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BNFToken_getType(token) == BNFTokenType_semicolon);
    BNFToken_delete(token);
    token = BNFScanner_scanNextToken(scanner);
    assert(token == NULL);
    BNFScanner_delete(scanner);

    printf("ok -- ");
}
void testScanSameToken() {
    printf("%s... ", __func__);

    CGString* text = CGString__new("aaaaaaa");
    CGArray(BNFScannerNode)* startNodes;
    BNFScannerRule* startRule = NULL;
	BNFScannerNode* startNode = BNFScannerNode__new(BNFScannerNodeType_string, "a", NULL, BNFTokenType_identifier, false);
    startNodes = CGArray__newFromInitializerList(BNFScannerNode, 
						startNode,
                        NULL);
    startRule = BNFScannerRule__new(startNodes);
	BNFScannerNode_setFollowupRule(startNode, startRule);

    BNFScanner* scanner = BNFScanner__new(startRule, text);

    BNFToken* token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
	printf("-%s-\n", BNFToken_getText(token));
	assert(!CGString__compare(BNFToken_getText(token), "a"));
    assert(BNFToken_getType(token) == BNFTokenType_identifier);
    BNFToken_delete(token);

    token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
	printf("-%s-\n", BNFToken_getText(token));
	assert(!CGString__compare(BNFToken_getText(token), "a"));
    assert(BNFToken_getType(token) == BNFTokenType_identifier);
    BNFToken_delete(token);

    token = BNFScanner_scanNextToken(scanner);
    assert(token != NULL);
	printf("-%s-\n", BNFToken_getText(token));
	assert(!CGString__compare(BNFToken_getText(token), "a"));
    assert(BNFToken_getType(token) == BNFTokenType_identifier);
    BNFToken_delete(token);

    BNFScanner_delete(scanner);

    printf("%s ok\n", __func__);
}
void testScanAllTokens() {
    printf("%s... ", __func__);

    CGString* text = CGString__new("aaaaaaa");
    CGArray(BNFScannerNode)* startNodes;
    BNFScannerRule* startRule;
    BNFScannerNode* aNode = BNFScannerNode__new(BNFScannerNodeType_string, "a", NULL, BNFTokenType_identifier, false);
    startNodes = CGArray__newFromInitializerList(BNFScannerNode, aNode, NULL);
    startRule = BNFScannerRule__new(startNodes);
    BNFScannerNode_setFollowupRule(aNode, startRule);

    BNFScanner* scanner = BNFScanner__new(startRule, text);
	CGArray(BNFToken)* tokenList = BNFScanner_scanAllTokens(scanner);
    assert(tokenList != NULL);
	printf("%u %u\n", CGArray_getSize(BNFToken, tokenList), CGString_getSize(text));
	printf("-%s- -%s-\n", BNFToken_getText(CGArray_getValueAt(BNFToken, tokenList, 0)),
		BNFToken_getText(CGArray_getValueAt(BNFToken, tokenList, 1)));
	assert(CGArray_getSize(BNFToken, tokenList) == CGString_getSize(text));
	assert(!CGString__compare(BNFToken_getText(CGArray_getValueAt(BNFToken, tokenList, 0)), CGString__new("a")));

    BNFScanner_delete(scanner);

    printf("ok -- ");
}

int main() {
    printf("=== %s ===\n", __FILE__);
    
    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();

    testNewDelete();
    testIdentifier();
    testIdentifierWithError();
    testNoise();
    testComplexRuleset();
    testScanSameToken();
    testScanAllTokens();

    CGAppState__deInit();

	return 0;
}
