#include<assert.h>
#include<stdio.h>
#include<string.h>
#include"Scanner.h"

/*
    tests that should scan ok
*/
void test_scanner_new_delete() {
    char* text = "";
    printf("%s...\n", __func__);
    Scanner* scanner = Scanner__new(text);
    Scanner_delete(scanner);
    printf("ok\n");
}

void test_scanner_identifier() {
    char* text = "abcde";
    Scanner* scanner;
    bool rv;
    printf("%s...\n", __func__);
    scanner = Scanner__new(text);
    rv = Scanner_scan(scanner);
    assert(rv == true);
    assert(scanner->error == Error_ok);
    assert(scanner->state == ScannerState_initial);
    assert(scanner->token->type == TokenType_start);
    assert(scanner->text == text+strlen(text));
    assert(!strcmp(scanner->text, text));
    Scanner_delete(scanner);
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
