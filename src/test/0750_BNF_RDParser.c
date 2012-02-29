#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include"BNF_RDParser.h"


DEFINE_ARRAY(BNFSentence);
DEFINE_ARRAY(BNFPhrase);
DEFINE_ARRAY(BNFAlternative);

CGAppState* appState;

BNFAst* parseForTestNewDelete(BNF_RDParser* parser) {
    return NULL;
}

void testNewDelete() {
    printf("%s... ", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(BNFTokenType_identifier, CGString__new(text));
    /*
    BNFScannerNode* node = BNFScannerNode__new(BNFScannerNodeType_string, "", NULL, BNFTokenType_start, false);
    BNFScannerRule* rule = BNFScannerRule__new(CGArray__newFromInitializerList(BNFScannerNode, node, NULL));
    */
    CGArray(BNFToken)* tokenList = CGArray__newFromInitializerList(BNFToken, token, NULL);
    BNFPhrase* phrase = BNFPhrase__new(BNFPhraseRepeat_once, NULL);
    BNFAlternative* alternative = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, phrase, NULL));
    BNFSentence* sentence = BNFSentence__new(CGString__new("test"), parseForTestNewDelete, CGArray__newFromInitializerList(BNFAlternative, alternative, NULL));

    BNFPhrase_setParts(phrase, CGArray__newFromInitializerList(BNFSentence, sentence, NULL));
    BNF_RDParser* parser = BNF_RDParser__new(tokenList, sentence);

    BNF_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}

int main() {
    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();
    printf("=== %s ===\n", __FILE__);

    testNewDelete();

    printf("=== %s ok ===\n", __FILE__);
    CGAppState__deInit();
	return 0;
}
