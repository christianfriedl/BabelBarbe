#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include"BNFToken.h"

CGAppState* appState;

void testNewDelete() {
    printf("%s...\n", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(BNFTokenType_identifier, CGString__new(text));
    BNFToken_delete(token);

    printf("ok\n");
}
void testPrint() {
    printf("%s...\n", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(BNFTokenType_identifier, CGString__new(text));
    BNFToken_print(token);
    BNFToken_delete(token);

    printf("ok\n");
}
void testGetters() {
    printf("%s...\n", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(BNFTokenType_identifier, CGString__new(text));
    CGString* string;
    string = BNFToken_getTypeName(token);
    assert(string != NULL);
    printf("'%s' should contain a stringified token type\n", string);
    CGString_delete(string);
    string = BNFToken_toString(token);
    assert(string != NULL);
    printf("'%s' should contain a stringified token\n", string);
    CGString_delete(string);
    string = BNFToken_getText(token);
    assert(string != NULL);
    assert(!CGString__compare(string, CGString__new(text)));
    BNFToken_delete(token);

    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);

    testNewDelete();
    testPrint();
    testGetters();

    printf("=== %s ok ===\n", __FILE__);
	return 0;
}
