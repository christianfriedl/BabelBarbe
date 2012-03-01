#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include"BNF_RDParser.h"


CGAppState* appState;

void testNewDelete() {
    printf("%s... ", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(BNFTokenType_identifier, CGString__new(text));
    /*
    BNFScannerNode* node = BNFScannerNode__new(BNFScannerNodeType_string, "", NULL, BNFTokenType_start, false);
    BNFScannerRule* rule = BNFScannerRule__new(CGArray__newFromInitializerList(BNFScannerNode, node, NULL));
    */
    BNFPhrase* phrase = BNFPhrase__new(BNFPhraseRepeat_once, NULL);
    BNFAlternative* alternative = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, phrase, NULL));
    BNFSentence* sentence = BNFSentence__new(CGString__new("test"), BNFTokenType_identifier, CGArray__newFromInitializerList(BNFAlternative, alternative, NULL));

    BNFPhrase_setParts(phrase, CGArray__newFromInitializerList(BNFSentence, sentence, NULL));
    BNF_RDParser* parser = BNF_RDParser__new(sentence);

    BNF_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}

void testOneTerminalDefinition() {
    /*
        identifier ::= 'xyz' ;
    */
    printf("%s...\n", __func__);

    CGString* text = CGString__new("xyz");
    BNFToken* token = BNFToken__new(BNFTokenType_identifier, CGString__new(text));
    CGArray(BNFToken)* tokenList = CGArray__newFromInitializerList(BNFToken, token, NULL);
    CGString* sentenceName = CGString__new("identifierSentence");
    BNFSentence* sentence = BNFSentence__new(sentenceName, BNFTokenType_identifier, NULL);
    BNF_RDParser* parser = BNF_RDParser__new(sentence);
    BNFAst* ast = BNF_RDParser_parse(parser, tokenList);
    assert(ast != NULL);
    assert(BNFToken_getType(BNFAst_getToken(ast)) == BNFTokenType_identifier);
    assert(!CGString__compare(BNFToken_getText(BNFAst_getToken(ast)), text));
    assert(!CGString__compare(BNFSentence_getName(BNFAst_getSentence(ast)), sentenceName));

    BNF_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}

/*
    definition ::= identifier;
    identifier ::= 'xyz';

    --> 
*/
void testOneAlternativeWithoutRepetition() {
    printf("%s...\n", __func__);

    CGString* text = CGString__new("xyz");
    BNFToken* token = BNFToken__new(BNFTokenType_identifier, CGString__new(text));
    CGArray(BNFToken)* tokenList = CGArray__newFromInitializerList(BNFToken, token, NULL);
    BNFPhrase* phrase = BNFPhrase__new(BNFPhraseRepeat_once, NULL);
    BNFAlternative* alternative = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, phrase, NULL));
    BNFSentence* identifierSentence = BNFSentence__new(CGString__new("identifierSentence"), BNFTokenType_identifier, NULL);
    BNFPhrase_setParts(phrase, CGArray__newFromInitializerList(BNFSentence, identifierSentence, NULL));
    BNFSentence* definitionSentence = BNFSentence__new(CGString__new("definitionSentence"), BNFTokenType_definition, CGArray__newFromInitializerList(BNFAlternative, alternative, NULL));

    BNF_RDParser* parser = BNF_RDParser__new(definitionSentence);
    BNF_RDParser_print(parser);
    BNFAst* ast = BNF_RDParser_parse(parser, tokenList);
    assert(ast != NULL);
    assert(BNFToken_getType(BNFAst_getToken(ast)) == BNFTokenType_definition);
    assert(!CGString__compare(BNFSentence_getName(BNFAst_getSentence(ast)), "definitionSentence"));
    CGAppState_THROW(CGAppState__getInstance(), Severity_notice, CGExceptionID_GeneralNonfatalException, "please add tests for sub-sentence");
    BNFAst_print(ast, 0);

    BNF_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}
/*
    definition ::= identifier | semicolon ;
    identifier ::= (whatever);
    semicolon ::= (whatever);

    --> 
*/
void testTwoAlternativesWithoutRepetition() {
    printf("%s...\n", __func__);

    CGString* text = CGString__new("xyz");
    BNFToken* token = BNFToken__new(BNFTokenType_semicolon, CGString__new(text));
    CGArray(BNFToken)* tokenList = CGArray__newFromInitializerList(BNFToken, token, NULL);
    BNFPhrase* phrase = BNFPhrase__new(BNFPhraseRepeat_once, NULL);
    BNFAlternative* alternative = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, phrase, NULL));
    BNFSentence* identifierSentence = BNFSentence__new(CGString__new("identifierSentence"), BNFTokenType_identifier, NULL);
    BNFSentence* semicolonSentence = BNFSentence__new(CGString__new("semicolonSentence"), BNFTokenType_identifier, NULL);
    BNFPhrase_setParts(phrase, CGArray__newFromInitializerList(BNFSentence, identifierSentence, NULL));
    BNFSentence* definitionSentence = BNFSentence__new(CGString__new("definitionSentence"), BNFTokenType_definition, CGArray__newFromInitializerList(BNFAlternative, alternative, NULL));

    BNF_RDParser* parser = BNF_RDParser__new(definitionSentence);
    BNF_RDParser_print(parser);
    BNFAst* ast = BNF_RDParser_parse(parser, tokenList);
    assert(ast != NULL);
    assert(BNFToken_getType(BNFAst_getToken(ast)) == BNFTokenType_definition);
    assert(!CGString__compare(BNFSentence_getName(BNFAst_getSentence(ast)), "definitionSentence"));
    CGAppState_THROW(CGAppState__getInstance(), Severity_notice, CGExceptionID_GeneralNonfatalException, "please add tests for sub-sentence");

    BNF_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}


int main() {
    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();
    printf("=== %s ===\n", __FILE__);

    testNewDelete();
    testOneTerminalDefinition();
    testOneAlternativeWithoutRepetition();

    printf("=== %s ok ===\n", __FILE__);
    CGAppState__deInit();
	return 0;
}
