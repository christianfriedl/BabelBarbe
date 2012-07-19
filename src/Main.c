#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGString.h>
#include"BBScanner.h"
#include"BBScannerRuleset.h"
#include"BB_RDParser.h"
#include"BBParserRuleset.h"
#include"BBCodeGenerator.h"


CGAppState* appState;

static BBAst* Main__parse_(CGString* text) {
    BBScannerRule* scannerStartRule = BBScannerRuleset__getInstance();
    BBScanner* scanner = BBScanner__new(scannerStartRule, text);
    BB_RDParser* parser = BB_RDParser__new(BBParserRuleset__getInstance());

    CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);

    return ast;
}

void Main__printUsage() {
    printf("Usage: compiler <inputfile>\n");
}
char* Main__readInputFile(char* filename) {
    FILE* inputFile = fopen(filename, "r");
    if (inputFile != NULL) {
        char string[256];
        CGString* returnString = CGString__new("");
        CGString* returnString2 = CGString__new("");
        while(fgets(string, 255, inputFile) != NULL) {
            returnString2 = CGString_append(returnString, string);
            returnString = CGString__new(returnString2);
            CGString_delete(returnString2);
            returnString2 = CGString__new("");
        }
        fclose(inputFile);
        return returnString;
    } else {
        printf("unable to read from '%s'\n", filename);
        exit(1);
        return NULL;
    }
}
int main(int argc, char *argv[]) {
    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();
    if (argc != 2) {
        Main__printUsage();
        return 1;
    }

    CGString* text = Main__readInputFile(argv[1]);
    BBAst* ast = Main__parse_(text);
    BBCodeGenerator* cg = BBCodeGenerator__new(ast);
    CGString* code = BBCodeGenerator_createCode(cg);
    BBCodeGenerator_delete(cg);
    printf("%s", code);
    CGString_delete(code);
    CGAppState__deInit();
	return 0;
}
