#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include"BBToken.h"
#include"BBScannerRuleset.h"

CGAppState* appState;
BBScannerRule* startRule = NULL;

void testNewDelete() {
    printf("%s... ", __func__);

    char* text = "xyz";
    BBToken* token = BBToken__new(BBTokenType_identifier, CGString__new(text));
    BBToken_delete(token);

    printf("ok -- ");
}
void testPrint() {
    printf("%s... ", __func__);

    char* text = "xyz";
    BBToken* token = BBToken__new(BBTokenType_identifier, CGString__new(text));
    BBToken_print(token);
    BBToken_delete(token);

    printf("ok -- ");
}
void testGetters() {
    printf("%s... ", __func__);

    char* text = "xyz";
    BBToken* token = BBToken__new(BBTokenType_identifier, CGString__new(text));
    CGString* string;
    string = BBToken_getTypeName(token);
    assert(string != NULL);
    printf("'%s' should contain a stringified token type\n", string);
    string = BBToken_toString(token);
    assert(string != NULL);
    printf("'%s' should contain a stringified token\n", string);
    string = BBToken_getText(token);
    assert(string != NULL);
    assert(!CGString__compare(string, CGString__new(text)));
    BBToken_delete(token);

    printf("ok -- ");
}

int main() {
    printf("=== %s ===\n", __FILE__);

    CGAppState__init(__FILE__);
    startRule = BBScannerRuleset__getInstance(); /* so that we initialize the token types */

    testNewDelete();
    testPrint();
    testGetters();

    CGAppState__deInit();

    printf("=== %s ok ===\n", __FILE__);
	return 0;
}
