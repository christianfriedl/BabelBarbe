#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGString.h>
#include"BNFScanner.h"
#include"BNFScannerRuleset.h"
#include"BNF_RDParser.h"
#include"BNFParserRuleset.h"


CGAppState* appState;

void Main__parse(CGString* text) {
    BNFScannerRule* scannerStartRule = BNFScannerRuleset__getInstance();
    BNFScanner* scanner = BNFScanner__new(scannerStartRule, text);
    BNF_RDParser* parser = BNF_RDParser__new(BNFParserRuleset__getInstance());

    CGArray(BNFToken)* tokenList = BNFScanner_scanAllTokens(scanner);
    BNFAst* ast = BNF_RDParser_parse(parser, tokenList);

    printf("Resulting Ast:\n");
    BNFAst_print(ast, 0);

    BNF_RDParser_delete(parser);

    printf("ok\n");
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
    printf("%s\n", argv[1]);

    CGString* text = Main__readInputFile(argv[1]);
    Main__parse(text);
    CGAppState__deInit();
	return 0;
}
