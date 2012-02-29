#include<assert.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"BNFAst.h"

/*
    positive tests
*/

CGAppState* appState;

void testNewDelete() {
    printf("%s... ", __func__);

    BNFToken* token1 = BNFToken__new(BNFTokenType_start, CGString__new("token1"));
    BNFToken* token2 = BNFToken__new(BNFTokenType_start, CGString__new("token2"));
    BNFAst* ast1 = BNFAst__new(NULL, token1, NULL);
    assert(ast1 != NULL);
    BNFAst* ast2 = BNFAst__new(ast1, token2, NULL);
    assert(ast2 != NULL);
    assert(BNFAst_getToken(ast1) == token1);
    assert(BNFAst_getToken(ast2) == token2);

    BNFAst_delete(ast1);

    printf("%s ok\n", __func__);
}

void testGetters() {
    printf("%s...\n", __func__);

    BNFToken* token1 = BNFToken__new(BNFTokenType_start, NULL);
    BNFToken* token2 = BNFToken__new(BNFTokenType_start, NULL);
    BNFSentence* sentence1 = BNFSentence__new(NULL, BNFTokenType_start, NULL);
    BNFSentence* sentence2 = BNFSentence__new(NULL, BNFTokenType_start, NULL);
    BNFAst* ast1 = BNFAst__new(NULL, token1, sentence1);
    assert(ast1 != NULL);
    BNFAst* ast2 = BNFAst__new(ast1, token2, sentence2);
    assert(ast2 != NULL);
    assert(BNFAst_getToken(ast1) == token1);
    assert(BNFAst_getToken(ast2) == token2);
    assert(BNFAst_getParent(ast1) == NULL);
    assert(BNFAst_getParent(ast2) == ast1);
    assert(BNFAst_getSentence(ast1) == sentence1);
    assert(BNFAst_getSentence(ast2) == sentence2);

    BNFAst_delete(ast1);

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
