#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include"BBToken.h"
#include"BBScannerRule.h"
#include"BBException.h"

static CGAppState *appState;

BBToken* tokenAbcd;
BBToken* tokenAbcde;
BBToken* tokenEfgh;
BBToken* tokenAbcxe;
BBToken* tokenAbcxe22;
BBToken* tokenAbcxe22fab;
BBToken* token123;

void setUp() {
    tokenAbcd = BBToken__new(BBTokenType_nonTerminal, CGString__new("abcd"));
    tokenAbcde = BBToken__new(BBTokenType_nonTerminal, CGString__new("abcde"));
    tokenEfgh = BBToken__new(BBTokenType_nonTerminal, CGString__new("efgh"));
    tokenAbcxe = BBToken__new(BBTokenType_nonTerminal, CGString__new("abcxe"));
    tokenAbcxe22 = BBToken__new(BBTokenType_nonTerminal, CGString__new("abcxe22"));
    tokenAbcxe22fab = BBToken__new(BBTokenType_nonTerminal, CGString__new("abcxe22fab"));
    token123 = BBToken__new(BBTokenType_nonTerminal, CGString__new("123"));
}

void _helpAssertEqual(BBToken* token, BBToken* token2) {
    assert(token2 != NULL);
    assert(BBToken_isEQual(token, token2));
}

void testNewDelete() {
    printf("%s... ", __func__);

    BBScannerNode* node = BBScannerNode__new(BBScannerNodeType_string, "", NULL, BBTokenType_nonTerminal, false, NULL);
    BBScannerRule* rule = BBScannerRule__new(CGString__new(""), CGArray__newFromInitializerList(BBScannerNode, node, NULL));
    assert(rule != NULL);
    assert(CGAppState_catchAndDeleteException(appState) == false);
    assert(CGArray_getSize(BBScannerNode, rule->nodes) == 1);
    BBScannerNode_delete(node);
    BBScannerRule_delete(rule);

    printf("ok -- ");
}

void testApplyStringPattern() {
    printf("%s... ", __func__);

    BBScannerNode* node1 = BBScannerNode__new(BBScannerNodeType_string, "abcd", NULL, BBTokenType_nonTerminal, false, NULL);
    BBScannerNode* node2 = BBScannerNode__new(BBScannerNodeType_string, "efgh", NULL, BBTokenType_nonTerminal, false, NULL);

    BBScannerRule* rule = BBScannerRule__new(CGString__new(""), CGArray__newFromInitializerList(BBScannerNode, node1, node2, NULL));
    
    _helpAssertEqual(BBScannerRule_applyToText(rule, "abcde"), tokenAbcd);
    _helpAssertEqual(BBScannerRule_applyToText(rule, "efghi"), tokenEfgh);
    assert(BBScannerRule_applyToText(rule, "xxxefghi") == NULL);

    BBScannerRule_delete(rule);
    BBScannerNode_delete(node2);
    BBScannerNode_delete(node1);

    printf("ok -- ");
}

void testApplyRegexPattern() {
    printf("%s... ", __func__);

    BBScannerNode* node1 = BBScannerNode__new(BBScannerNodeType_regex, "[a-z]+", NULL, BBTokenType_nonTerminal, false, NULL);
    BBScannerNode* node2 = BBScannerNode__new(BBScannerNodeType_regex, "[0-9]+", NULL, BBTokenType_nonTerminal, false, NULL);

    BBScannerRule* rule = BBScannerRule__new(CGString__new(""), CGArray__newFromInitializerList(BBScannerNode, node1, node2, NULL));
    
    _helpAssertEqual(BBScannerRule_applyToText(rule, "abcde"), tokenAbcde);
    _helpAssertEqual(BBScannerRule_applyToText(rule, "123efghi"), token123);
    assert(BBScannerRule_applyToText(rule, " xxxefghi") == NULL);

    BBScannerRule_delete(rule);
    BBScannerNode_delete(node2);
    BBScannerNode_delete(node1);
    
    printf("ok -- ");
}

int main() {
    setUp();

    printf("=== %s ===\n", __FILE__);

    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();

    testNewDelete();
    testApplyStringPattern();
    testApplyRegexPattern();

    CGAppState__deInit();

	return 0;
}
