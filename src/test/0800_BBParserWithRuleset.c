#include<assert.h>
#include<stdio.h>
#define DEBUG 
#include<cgenerics/CGAppState.h>
#include"BBScanner.h"
#include"BBScannerRuleset.h"
#include"BB_RDParser.h"
#include"BBParserRuleset.h"



CGAppState* appState;
BBScannerRule* startRule = NULL;

void testParse() {
    printf("%s... ", __func__);

    char* text = "xyz ::= abc ; ";
    CGArray(BBSentence)* seenSentences = CGArray__new(BBSentence, 1);
    BBSentence_print(BBParserRuleset__getInstance(), 0, seenSentences);

    BBScannerRule* scannerStartRule = BBScannerRuleset__getInstance();
    BBScanner* scanner = BBScanner__new(scannerStartRule, text);
    BB_RDParser* parser = BB_RDParser__new(BBParserRuleset__getInstance());

    CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);

    printf("Resulting Ast:\n");
    BBAst_print(ast, 0);

    BB_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}

void testParseTwoSentences() {
    printf("%s... ", __func__);

    char* text = "xyz ::= abc ; def ::= ghi;";
    CGArray(BBSentence)* seenSentences = CGArray__new(BBSentence, 1);
    BBSentence_print(BBParserRuleset__getInstance(), 0, seenSentences);

    BBScannerRule* scannerStartRule = BBScannerRuleset__getInstance();
    BBScanner* scanner = BBScanner__new(scannerStartRule, text);
    BB_RDParser* parser = BB_RDParser__new(BBParserRuleset__getInstance());

    CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);

    printf("Resulting Ast:\n");
    BBAst_print(ast, 0);

    BB_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}
void testParseWithLiterals() {
    printf("%s... ", __func__);

    char* text = "xyz ::= 'a\\'bc' ; def ::= /g\\/hi+/ ;";
    CGArray(BBSentence)* seenSentences = CGArray__new(BBSentence, 1);
    BBSentence_print(BBParserRuleset__getInstance(), 0, seenSentences);

    BBScannerRule* scannerStartRule = BBScannerRuleset__getInstance();
    BBScanner* scanner = BBScanner__new(scannerStartRule, text);
    BB_RDParser* parser = BB_RDParser__new(BBParserRuleset__getInstance());

    CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);

    printf("Resulting Ast:\n");
    BBAst_print(ast, 0);

    BB_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}
static char* getBBGrammar_() {
    FILE* file = fopen("bnf.bnf", "r");
    char* text = malloc(1025 * sizeof(char));
    size_t size = fread(text, 1, 1024, file);
    text[size] = '\0';
    fclose(file);
    return text;
}
void testParseCompleteBBGrammar() {
    printf("%s... ", __func__);

    char* text = getBBGrammar_();
    CGArray(BBSentence)* seenSentences = CGArray__new(BBSentence, 1);
    BBSentence_print(BBParserRuleset__getInstance(), 0, seenSentences);

    BBScannerRule* scannerStartRule = BBScannerRuleset__getInstance();
    BBScanner* scanner = BBScanner__new(scannerStartRule, text);
    BB_RDParser* parser = BB_RDParser__new(BBParserRuleset__getInstance());

    CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);

    printf("Resulting Ast:\n");
    BBAst_print(ast, 0);

    BB_RDParser_delete(parser);
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
    CGArray(BBSentence)* seenSentences = CGArray__new(BBSentence, 1);
    BBSentence_print(BBParserRuleset__getInstance(), 0, seenSentences);

    BBScannerRule* scannerStartRule = BBScannerRuleset__getInstance();
    BBScanner* scanner = BBScanner__new(scannerStartRule, text);
    BB_RDParser* parser = BB_RDParser__new(BBParserRuleset__getInstance());

    CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    BB_RDParser__printTokenList(tokenList);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);

    printf("Resulting Ast:\n");
    BBAst_print(ast, 0);

    BB_RDParser_delete(parser);
    free(text);

    printf("%s ok\n", __func__);
}

int main() {
    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();
    printf("=== %s ===\n", __FILE__);
    startRule = BBScannerRuleset__getInstance();

    testParse();
    testParseTwoSentences();
    testParseWithLiterals();
    testParseCompleteBBGrammar();
    testParseCompleteCalcGrammar();

    printf("=== %s ok ===\n", __FILE__);
    CGAppState__deInit();
	return 0;
}
