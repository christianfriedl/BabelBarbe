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

/*
void testIdentifier() {
    printf("%s...\n", __func__);
    char* text = CGString__new("abcde");
    BNFScanner* scanner = BNFScanner__new(appState, text);
    rv = BNFScanner_scan(scanner);
    assert(rv == true);
    assert(scanner->state == BNFScannerState_initial);
    assert(scanner->token->type == BNFTokenType_start);
    assert(scanner->text == text+strlen(text));
    assert(!strcmp(scanner->text, text));
    BNFScanner_delete(scanner);
    printf("ok\n");
}
*/

int main() {
    printf("=== %s ===\n", __FILE__);
    
    appState = CGAppState__new(__FILE__);

    testNewDelete();

	return 0;
}
