#include"/opt/local/include/pcre.h"
#include<bbarbe/BNFTokenType.h>
#include<bbarbe/BNFToken.h>
#include<bbarbe/BNFScannerRule.h>
#include<bbarbe/BNFAst.h>
#include<bbarbe/BNF_RDParser.h>

CGAppState* appState = NULL;

BNFScannerRule* createScannerRuleset();
BNFSentence* createParserRuleset();

int main() {
    CGAppState__init("calc");
    BNFScannerRule* startRule = createScannerRuleset();
    BNFSentence* startSentence = createParserRuleset();
    CGString* text = CGString__new("1+2");
    BNFScanner* scanner = BNFScanner__new(startRule, text);
    BNF_RDParser* parser = BNF_RDParser__new(startSentence);
    CGArray(BNFToken)* tokenList = BNFScanner_scanAllTokens(scanner);
    BNFAst* ast = BNF_RDParser_parse(parser, tokenList);
    if (ast != NULL)
        BNFAst_print(ast, 0);
    else
        printf("no resulting ast.\n");
    CGAppState__deInit();
    return 0;
}
