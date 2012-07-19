#include<assert.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"BBAst.h"

/*
    positive tests
*/

CGAppState* appState;

void testNewDelete() {
    printf("%s... ", __func__);

    BBToken* token1 = BBToken__new(BBTokenType_nonTerminal, CGString__new("token1"));
    BBToken* token2 = BBToken__new(BBTokenType_nonTerminal, CGString__new("token2"));
    BBAst* ast1 = BBAst__new(NULL, token1, NULL);
    assert(ast1 != NULL);
    BBAst* ast2 = BBAst__new(ast1, token2, NULL);
    assert(ast2 != NULL);
    assert(BBAst_getToken(ast1) == token1);
    assert(BBAst_getToken(ast2) == token2);

    BBAst_delete(ast1);

    printf("%s ok\n", __func__);
}

void testGetters() {
    printf("%s...\n", __func__);

    BBToken* token1 = BBToken__new(BBTokenType_nonTerminal, NULL);
    BBToken* token2 = BBToken__new(BBTokenType_nonTerminal, NULL);
    BBSentence* sentence1 = BBSentence__new(NULL, BBTokenType_nonTerminal, NULL);
    BBSentence* sentence2 = BBSentence__new(NULL, BBTokenType_nonTerminal, NULL);
    BBAst* ast1 = BBAst__new(NULL, token1, sentence1);
    assert(ast1 != NULL);
    BBAst* ast2 = BBAst__new(ast1, token2, sentence2);
    assert(ast2 != NULL);
    assert(BBAst_getToken(ast1) == token1);
    assert(BBAst_getToken(ast2) == token2);
    assert(BBAst_getParent(ast1) == NULL);
    assert(BBAst_getParent(ast2) == ast1);
    assert(BBAst_getSentence(ast1) == sentence1);
    assert(BBAst_getSentence(ast2) == sentence2);

    BBAst_delete(ast1);

    printf("%s ok\n", __func__);
}

int main() {
    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();
    printf("=== %s... ===\n", __FILE__);

    testNewDelete();

    printf("=== %s ok ===\n", __FILE__);
    CGAppState__deInit();
	return 0;
}
