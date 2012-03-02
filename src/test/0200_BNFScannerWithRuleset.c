#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGString.h>
#include"BNFToken.h"
#include"BNFScannerRule.h"
#include"BNFScannerRuleset.h"
#include"BNFException.h"
#include"BNFScanner.h"

static CGAppState *appState;


void testScan() {
    printf("%s... ", __func__);

	BNFScannerRule* startRule = BNFScannerRuleset__getInstance();
	CGString* text = CGString__new("");
	BNFScanner* scanner = BNFScanner__new(startRule, text);

    printf("ok -- ");
}

int main() {
    printf("=== %s ===\n", __FILE__);

    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();

    testScan();

    CGAppState__deInit();

	return 0;
}
