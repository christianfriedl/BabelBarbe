#include<assert.h>
#include<stdio.h>
#include<string.h>
#include"BNFScanner.h"

/*
    tests that should scan ok
*/
void test_scanner_new_delete() {
    char* text = "";
    printf("%s...\n", __func__);
    BNFScanner* scanner = BNFScanner__new(text);
    BNFScanner_delete(scanner);
    printf("ok\n");
}

void test_scanner_identifier() {
    char* text = "abcde";
    BNFScanner* scanner;
    bool rv;
    printf("%s...\n", __func__);
    scanner = BNFScanner__new(text);
    rv = BNFScanner_scan(scanner);
    assert(rv == true);
    assert(scanner->state == BNFScannerState_initial);
    assert(scanner->token->type == BNFTokenType_start);
    assert(scanner->text == text+strlen(text));
    assert(!strcmp(scanner->text, text));
    BNFScanner_delete(scanner);
    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_scanner_new_delete();
    test_scanner_identifier();

    /*
        negative tests
    */

	return 0;
}
