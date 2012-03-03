#include<assert.h>
#include<stdio.h>
#define DEBUG 
#include<cgenerics/CGAppState.h>
#include"BNFScanner.h"
#include"BNFScannerRuleset.h"
#include"BNF_RDParser.h"
#include"BNFParserRuleset.h"



CGAppState* appState;

void testParse() {
    printf("%s... ", __func__);

    char* text = "xyz ::= abc ; ";
    CGArray(BNFSentence)* seenSentences = CGArray__new(BNFSentence, 1);
    BNFSentence_print(BNFParserRuleset__getInstance(), 0, seenSentences);

    BNFScannerRule* scannerStartRule = BNFScannerRuleset__getInstance();
    BNFScanner* scanner = BNFScanner__new(scannerStartRule, text);
    BNF_RDParser* parser = BNF_RDParser__new(BNFParserRuleset__getInstance());

    CGArray(BNFToken)* tokenList = BNFScanner_scanAllTokens(scanner);
    BNFAst* ast = BNF_RDParser_parse(parser, tokenList);

    printf("Resulting Ast:\n");
    BNFAst_print(ast, 0);

    BNF_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}

int main() {
    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();
    printf("=== %s ===\n", __FILE__);

    testParse();

    printf("=== %s ok ===\n", __FILE__);
    CGAppState__deInit();
	return 0;
}
