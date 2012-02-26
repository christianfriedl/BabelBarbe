#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include"BNFParser.h"



CGAppState* appState;

void testNewDelete() {
    printf("%s...\n", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(BNFTokenType_identifier, CGString__new(text));
    BNFToken_delete(token);

    printf("%s ok\n", __func__);
}

int main() {
    printf("=== %s ===\n", __FILE__);

    testNewDelete();

    printf("=== %s ok ===\n", __FILE__);
	return 0;
}
