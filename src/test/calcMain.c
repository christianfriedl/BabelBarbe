#include"/opt/local/include/pcre.h"
#include<bbarbe/BBTokenType.h>
#include<bbarbe/BBToken.h>
#include<bbarbe/BBScannerRule.h>
#include<bbarbe/BBAst.h>
#include<bbarbe/BB_RDParser.h>

CGAppState* appState = NULL;

BBScannerRule* createScannerRuleset();
BBSentence* createParserRuleset();

int main() {
    CGAppState__init("calc");
    BBScannerRule* startRule = createScannerRuleset();
    BBSentence* startSentence = createParserRuleset();
    CGString* text = CGString__new("(1+2)*3");
    BBScanner* scanner = BBScanner__new(startRule, text);
    BB_RDParser* parser = BB_RDParser__new(startSentence);
    CGArray(BBToken)* tokenList = BBScanner_scanAllTokens(scanner);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);
    if (ast != NULL)
        BBAst_print(ast, 0);
    else
        printf("no resulting ast.\n");
    CGAppState__deInit();
    return 0;
}
