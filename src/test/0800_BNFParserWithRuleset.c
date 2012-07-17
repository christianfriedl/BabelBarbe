#include<assert.h>
#include<stdio.h>
#define DEBUG 
#include<cgenerics/CGAppState.h>
#include"BNFScanner.h"
#include"BNFScannerRuleset.h"
#include"BNF_RDParser.h"
#include"BNFParserRuleset.h"



CGAppState* appState;
BNFScannerRule* startRule = NULL;

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

void testParseTwoSentences() {
    printf("%s... ", __func__);

    char* text = "xyz ::= abc ; def ::= ghi;";
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
void testParseWithLiterals() {
    printf("%s... ", __func__);

    char* text = "xyz ::= 'a\\'bc' ; def ::= /g\\/hi+/ ;";
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
static char* getBNFGrammar_() {
    FILE* file = fopen("bnf.bnf", "r");
    char* text = malloc(1025 * sizeof(char));
    size_t size = fread(text, 1, 1024, file);
    text[size] = '\0';
    fclose(file);
    return text;
}
void testParseCompleteBNFGrammar() {
    printf("%s... ", __func__);

    char* text = getBNFGrammar_();
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
    free(text);

    printf("%s ok\n", __func__);
}

static char* getCalcGrammar_() {
    FILE* file = fopen("calc.bnf", "r");
    char* text = malloc(1025 * sizeof(char));
    size_t size = fread(text, 1, 1024, file);
    text[size] = '\0';
    fclose(file);
    return text;
}
void testParseCompleteCalcGrammar() {
    printf("%s... ", __func__);

    char* text = getCalcGrammar_();
    CGArray(BNFSentence)* seenSentences = CGArray__new(BNFSentence, 1);
    BNFSentence_print(BNFParserRuleset__getInstance(), 0, seenSentences);

    BNFScannerRule* scannerStartRule = BNFScannerRuleset__getInstance();
    BNFScanner* scanner = BNFScanner__new(scannerStartRule, text);
    BNF_RDParser* parser = BNF_RDParser__new(BNFParserRuleset__getInstance());

    CGArray(BNFToken)* tokenList = BNFScanner_scanAllTokens(scanner);
    BNF_RDParser__printTokenList(tokenList);
    BNFAst* ast = BNF_RDParser_parse(parser, tokenList);

    printf("Resulting Ast:\n");
    BNFAst_print(ast, 0);

    BNF_RDParser_delete(parser);
    free(text);

    printf("%s ok\n", __func__);
}

int main() {
    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();
    printf("=== %s ===\n", __FILE__);
    startRule = BNFScannerRuleset__getInstance();

    testParse();
    testParseTwoSentences();
    testParseWithLiterals();
    testParseCompleteBNFGrammar();
    testParseCompleteCalcGrammar();

    printf("=== %s ok ===\n", __FILE__);
    CGAppState__deInit();
	return 0;
}
