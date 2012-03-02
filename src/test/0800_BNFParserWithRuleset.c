#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include"BNF_RDParser.h"
#include"BNFParserRuleset.h"


CGAppState* appState;

void testParse() {
    printf("%s... ", __func__);

    char* text = "xyz";
    CGArray(BNFSentence)* seenSentences = CGArray__new(BNFSentence, 1);
    BNFSentence_print(BNFParserRuleset__getInstance(), 0, seenSentences);

    BNF_RDParser* parser = BNF_RDParser__new(BNFParserRuleset__getInstance());

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
