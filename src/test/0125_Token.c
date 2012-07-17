#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include"BNFToken.h"
#include"BNFScannerRuleset.h"

CGAppState* appState;
BNFScannerRule* startRule = NULL;

void testNewDelete() {
    printf("%s... ", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(BNFTokenType_identifier, CGString__new(text));
    BNFToken_delete(token);

    printf("ok -- ");
}
void testPrint() {
    printf("%s... ", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(BNFTokenType_identifier, CGString__new(text));
    BNFToken_print(token);
    BNFToken_delete(token);

    printf("ok -- ");
}
void testGetters() {
    printf("%s... ", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(BNFTokenType_identifier, CGString__new(text));
    CGString* string;
    string = BNFToken_getTypeName(token);
    assert(string != NULL);
    printf("'%s' should contain a stringified token type\n", string);
    string = BNFToken_toString(token);
    assert(string != NULL);
    printf("'%s' should contain a stringified token\n", string);
    string = BNFToken_getText(token);
    assert(string != NULL);
    assert(!CGString__compare(string, CGString__new(text)));
    BNFToken_delete(token);

    printf("ok -- ");
}

int main() {
    printf("=== %s ===\n", __FILE__);

    CGAppState__init(__FILE__);
    startRule = BNFScannerRuleset__getInstance(); /* so that we initialize the token types */

    testNewDelete();
    testPrint();
    testGetters();

    CGAppState__deInit();

    printf("=== %s ok ===\n", __FILE__);
	return 0;
}
