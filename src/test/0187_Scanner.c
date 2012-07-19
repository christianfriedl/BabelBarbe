#include<assert.h>
#include<stdio.h>
#include<string.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGString.h>
#include<cgenerics/CGArray.h>
#include"CGArrayOfBBScannerNode.h"
#include"BBToken.h"
#include"BBScannerRule.h"
#include"BBScanner.h"

CGAppState* appState;
BBScannerRule* startRule = NULL;

void testNewDelete() {
    printf("%s... ", __func__);
    CGString* text = CGString__new("");
    CGArray(BBScannerNode)* nodes = CGArray__newFromInitializerList(BBScannerNode, NULL);
    BBScannerRule* rule = BBScannerRule__new(CGString__new(""), nodes);
    BBScanner* scanner = BBScanner__new(rule, text);
    BBScanner_delete(scanner);
    printf("ok -- ");
}

void testIdentifier() {
    printf("%s... ", __func__);

    char* text = CGString__new("abcde");
    CGArray(BBScannerNode)* nodes = CGArray__newFromInitializerList(BBScannerNode, BBScannerNode__new(BBScannerNodeType_regex, "\\w+", NULL, BBTokenType_identifier, false, NULL), NULL);
    BBScannerRule* rule = BBScannerRule__new(CGString__new(""), nodes);
    BBScanner* scanner = BBScanner__new(rule, text);
    BBToken* token = BBScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BBToken_getType(token) == BBTokenType_identifier);
    CGString* tokenText = BBToken_getText(token);
    assert(!CGString__compare(tokenText, text));
    assert(BBScanner_scanNextToken(scanner) == NULL);
    BBToken_delete(token);
    BBScanner_delete(scanner);

    printf("ok -- ");
}
void testIdentifierWithError() {
    printf("%s... ", __func__);

    CGString* text = CGString__new("abcde123");
    CGString* identText = CGString__new("abcde");
    CGArray(BBScannerNode)* nodes = CGArray__newFromInitializerList(BBScannerNode, BBScannerNode__new(BBScannerNodeType_regex, "[a-z]+", NULL, BBTokenType_identifier, false, NULL), NULL);
    BBScannerRule* rule = BBScannerRule__new(CGString__new(""), nodes);
    BBScanner* scanner = BBScanner__new(rule, text);
    BBToken* token = BBScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BBToken_getType(token) == BBTokenType_identifier);
    CGString* tokenText = BBToken_getText(token);
    assert(!CGString__compare(tokenText, identText));
    assert(BBScanner_scanNextToken(scanner) == NULL);
    assert(CGAppState_catchAndDeleteExceptionWithID(appState, BBExceptionID_ScannerError) == true);
    BBToken_delete(token);
    BBScanner_delete(scanner);

    printf("ok -- ");
}
void testNoise() {
    printf("%s... ", __func__);

    CGString* text = CGString__new("     ");
    CGArray(BBScannerNode)* nodes = CGArray__newFromInitializerList(BBScannerNode,
                                            BBScannerNode__new(BBScannerNodeType_regex, "\\s*", NULL, BBTokenType_noise, true, NULL),
                                            NULL);
    BBScannerRule* rule = BBScannerRule__new(CGString__new(""), nodes);
    BBScanner* scanner = BBScanner__new(rule, text);
    BBToken* token = BBScanner_scanNextToken(scanner);
    assert(token == NULL);
    assert(CGAppState_catchAndDeleteExceptionWithID(appState, BBExceptionID_ScannerError) == true);
    BBScanner_delete(scanner);
}
void testComplexRuleset() {
    printf("%s... ", __func__);

    CGString* text = CGString__new("abcde ::= fghi;");
    CGArray(BBScannerNode)* startNodes;
    CGArray(BBScannerNode)* noiseNodes;
    noiseNodes = CGArray__new(BBScannerNode, 1);
    BBScannerRule* startRule;
    BBScannerRule* noiseRule = BBScannerRule__new(CGString__new(""), noiseNodes);
    startNodes = CGArray__newFromInitializerList(BBScannerNode, 
                        BBScannerNode__new(BBScannerNodeType_regex, "[a-z]+", noiseRule, BBTokenType_identifier, false, NULL), 
                        BBScannerNode__new(BBScannerNodeType_string, "::=", noiseRule, BBTokenType_definitionSign, false, NULL), 
                        BBScannerNode__new(BBScannerNodeType_string, ";", noiseRule, BBTokenType_semicolon, false, NULL), 
                        NULL);
    startRule = BBScannerRule__new(CGString__new(""), startNodes);
    CGArray_add(BBScannerNode, noiseNodes, BBScannerNode__new(BBScannerNodeType_regex, "\\s*", startRule, BBTokenType_noise, true, NULL));

    BBScanner* scanner = BBScanner__new(startRule, text);
    BBToken* token = BBScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BBToken_getType(token) == BBTokenType_identifier);
    BBToken_delete(token);
    token = BBScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BBToken_getType(token) == BBTokenType_definitionSign);
    printf("token: %s\n", BBToken_toString(token));
    BBToken_delete(token);
    token = BBScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BBToken_getType(token) == BBTokenType_identifier);
    BBToken_delete(token);
    token = BBScanner_scanNextToken(scanner);
    assert(token != NULL);
    assert(BBToken_getType(token) == BBTokenType_semicolon);
    BBToken_delete(token);
    token = BBScanner_scanNextToken(scanner);
    assert(token == NULL);
    BBScanner_delete(scanner);

    printf("ok -- ");
}
void testScanSameToken() {
    printf("%s... ", __func__);

    CGString* text = CGString__new("aaaaaaa");
    CGArray(BBScannerNode)* startNodes;
    BBScannerRule* startRule = NULL;
	BBScannerNode* startNode = BBScannerNode__new(BBScannerNodeType_string, "a", NULL, BBTokenType_identifier, false, NULL);
    startNodes = CGArray__newFromInitializerList(BBScannerNode, 
						startNode,
                        NULL);
    startRule = BBScannerRule__new(CGString__new(""), startNodes);
	BBScannerNode_setFollowupRule(startNode, startRule);

    BBScanner* scanner = BBScanner__new(startRule, text);

    BBToken* token = BBScanner_scanNextToken(scanner);
    assert(token != NULL);
	printf("-%s-\n", BBToken_getText(token));
	assert(!CGString__compare(BBToken_getText(token), "a"));
    assert(BBToken_getType(token) == BBTokenType_identifier);
    BBToken_delete(token);

    token = BBScanner_scanNextToken(scanner);
    assert(token != NULL);
	printf("-%s-\n", BBToken_getText(token));
	assert(!CGString__compare(BBToken_getText(token), "a"));
    assert(BBToken_getType(token) == BBTokenType_identifier);
    BBToken_delete(token);

    token = BBScanner_scanNextToken(scanner);
    assert(token != NULL);
	printf("-%s-\n", BBToken_getText(token));
	assert(!CGString__compare(BBToken_getText(token), "a"));
    assert(BBToken_getType(token) == BBTokenType_identifier);
    BBToken_delete(token);

    BBScanner_delete(scanner);

    printf("%s ok\n", __func__);
}
void testScanAllTokens() {
    printf("%s... ", __func__);

    CGString* text = CGString__new("aaaaaaa");
    CGArray(BBScannerNode)* startNodes;
    BBScannerRule* startRule;
    BBScannerNode* aNode = BBScannerNode__new(BBScannerNodeType_string, "a", NULL, BBTokenType_identifier, false, NULL);
    startNodes = CGArray__newFromInitializerList(BBScannerNode, aNode, NULL);
    startRule = BBScannerRule__new(CGString__new(""), startNodes);
    BBScannerNode_setFollowupRule(aNode, startRule);

    BBScanner* scanner = BBScanner__new(startRule, text);
	CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    assert(tokenList != NULL);
	printf("%u %u\n", CGArray_getSize(BBToken, tokenList), CGString_getSize(text));
	printf("-%s- -%s-\n", BBToken_getText(CGArray_getValueAt(BBToken, tokenList, 0)),
		BBToken_getText(CGArray_getValueAt(BBToken, tokenList, 1)));
	assert(CGArray_getSize(BBToken, tokenList) == CGString_getSize(text));
	assert(!CGString__compare(BBToken_getText(CGArray_getValueAt(BBToken, tokenList, 0)), CGString__new("a")));

    BBScanner_delete(scanner);

    printf("ok -- ");
}

int main() {
    printf("=== %s ===\n", __FILE__);
    
    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();
    startRule = BBScannerRuleset__getInstance();

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
