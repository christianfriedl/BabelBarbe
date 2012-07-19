#include<assert.h>
#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include"BB_RDParser.h"


CGAppState* appState;
BBScannerRule* startRule = NULL;

void testNewDelete() {
    printf("%s... ", __func__);

    char* text = "xyz";
    BBToken* token = BBToken__new(BBTokenType_identifier, CGString__new(text));
    /*
    BBScannerNode* node = BBScannerNode__new(BBScannerNodeType_string, "", NULL, BBTokenType_nonTerminal, false);
    BBScannerRule* rule = BBScannerRule__new(CGString__new(""), CGArray__newFromInitializerList(BBScannerNode, node, NULL));
    */
    BBPhrase* phrase = BBPhrase__new(BBPhraseRepeat_once, NULL);
    BBAlternative* alternative = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, phrase, NULL));
    BBSentence* sentence = BBSentence__new(CGString__new("test"), BBTokenType_identifier, CGArray__newFromInitializerList(BBAlternative, alternative, NULL));

    BBPhrase_setParts(phrase, CGArray__newFromInitializerList(BBSentence, sentence, NULL));
    BB_RDParser* parser = BB_RDParser__new(sentence);

    BB_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}

void testOneTerminalDefinition() {
    /*
        identifier ::= 'xyz' ;
    */
    printf("%s...\n", __func__);

    CGString* text = CGString__new("xyz");
    BBToken* token = BBToken__new(BBTokenType_identifier, CGString__new(text));
    CGArray(BBToken)* tokenList = CGArray__newFromInitializerList(BBToken, token, NULL);
    CGString* sentenceName = CGString__new("identifierSentence");
    BBSentence* sentence = BBSentence__new(sentenceName, BBTokenType_identifier, NULL);
    BB_RDParser* parser = BB_RDParser__new(sentence);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);
    assert(ast != NULL);
    assert(BBToken_getType(BBAst_getToken(ast)) == BBTokenType_identifier);
    assert(!CGString__compare(BBToken_getText(BBAst_getToken(ast)), text));
    assert(!CGString__compare(BBSentence_getName(BBAst_getSentence(ast)), sentenceName));

    BB_RDParser_delete(parser);

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
    BBToken* token = BBToken__new(BBTokenType_identifier, CGString__new(text));
    CGArray(BBToken)* tokenList = CGArray__newFromInitializerList(BBToken, token, NULL);
    BBPhrase* phrase = BBPhrase__new(BBPhraseRepeat_once, NULL);
    BBAlternative* alternative = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, phrase, NULL));
    BBSentence* identifierSentence = BBSentence__new(CGString__new("identifierSentence"), BBTokenType_identifier, NULL);
    BBPhrase_setParts(phrase, CGArray__newFromInitializerList(BBSentence, identifierSentence, NULL));
    BBSentence* definitionSentence = BBSentence__new(CGString__new("definitionSentence"), BBTokenType_definitionSign, CGArray__newFromInitializerList(BBAlternative, alternative, NULL));

    BB_RDParser* parser = BB_RDParser__new(definitionSentence);
    BB_RDParser_print(parser);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);
    assert(ast != NULL);
    assert(BBToken_getType(BBAst_getToken(ast)) == BBTokenType_definitionSign);
    assert(!CGString__compare(BBSentence_getName(BBAst_getSentence(ast)), "definitionSentence"));
    CGAppState_THROW(CGAppState__getInstance(), Severity_notice, CGExceptionID_GeneralNonfatalException, "please add tests for sub-sentence");
    BBAst_print(ast, 0);

    BB_RDParser_delete(parser);

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
    BBToken* token = BBToken__new(BBTokenType_semicolon, CGString__new(text));
    CGArray(BBToken)* tokenList = CGArray__newFromInitializerList(BBToken, token, NULL);
    BBSentence* identifierSentence = BBSentence__new(CGString__new("identifierSentence"), BBTokenType_identifier, NULL);
    BBSentence* semicolonSentence = BBSentence__new(CGString__new("semicolonSentence"), BBTokenType_semicolon, NULL);
    BBPhrase* identifierPhrase = BBPhrase__new(BBPhraseRepeat_once, NULL);
    BBPhrase* semicolonPhrase = BBPhrase__new(BBPhraseRepeat_once, NULL);
    BBPhrase_setParts(identifierPhrase, CGArray__newFromInitializerList(BBSentence, identifierSentence, NULL));
    BBPhrase_setParts(semicolonPhrase, CGArray__newFromInitializerList(BBSentence, semicolonSentence, NULL));
    BBAlternative* identifierAlternative = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, identifierPhrase, NULL));
    BBAlternative* semicolonAlternative = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, semicolonPhrase, NULL));
    BBSentence* definitionSentence = BBSentence__new(CGString__new("definitionSentence"), BBTokenType_definitionSign, CGArray__newFromInitializerList(BBAlternative, identifierAlternative, semicolonAlternative, NULL));

    BB_RDParser* parser = BB_RDParser__new(definitionSentence);
    BB_RDParser_print(parser);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);
    assert(ast != NULL);
    assert(BBToken_getType(BBAst_getToken(ast)) == BBTokenType_definitionSign);
    assert(!CGString__compare(BBSentence_getName(BBAst_getSentence(ast)), "definitionSentence"));
    CGAppState_THROW(CGAppState__getInstance(), Severity_notice, CGExceptionID_GeneralNonfatalException, "please add tests for sub-sentence");
    BBAst_print(ast, 0);

    BB_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}
void testUnexpectedEOF() {
    printf("%s...\n", __func__);

    CGAppState_catchAndDeleteException(CGAppState__getInstance());
    CGString* text = CGString__new("xyz");
    BBToken* token = BBToken__new(BBTokenType_identifier, CGString__new(text));
    CGArray(BBToken)* tokenList = CGArray__newFromInitializerList(BBToken, token, NULL);
    BBSentence* identifierSentence = BBSentence__new(CGString__new("identifierSentence"), BBTokenType_identifier, NULL);
    BBSentence* semicolonSentence = BBSentence__new(CGString__new("semicolonSentence"), BBTokenType_semicolon, NULL);
    BBPhrase* identifierPhrase = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, identifierSentence, NULL));
    CGAppState_catchAndDeleteException(CGAppState__getInstance());
    BBPhrase* semicolonPhrase = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, semicolonSentence, NULL));
    BBPhrase* identifierSemicolonPhrase = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, identifierSentence, semicolonSentence, NULL));
    BBAlternative* identifierSemicolonAlternative = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, identifierSemicolonPhrase, NULL));
    BBAlternative* semicolonAlternative = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, semicolonPhrase, NULL));
    BBSentence* definitionSentence = BBSentence__new(CGString__new("definitionSentence"), BBTokenType_definitionSign, CGArray__newFromInitializerList(BBAlternative, identifierSemicolonAlternative, semicolonAlternative, NULL));

    BB_RDParser* parser = BB_RDParser__new(definitionSentence);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);
    assert(ast == NULL);

    assert(CGAppState_catchAndDeleteExceptionWithID(CGAppState__getInstance(), BBExceptionID_ScannerError) == true);

    BB_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}

/*
    definition ::= identifier semicolon | semicolon ;
    identifier ::= (whatever);
    semicolon ::= (whatever);

    --> 
*/
void testTwoAlternativesAndTwoPhrasesWithoutRepetition() {
    printf("%s...\n", __func__);

    CGAppState_catchAndDeleteException(CGAppState__getInstance());
    CGString* text = CGString__new("ident");
    CGString* text2 = CGString__new(";");
    BBToken* token = BBToken__new(BBTokenType_identifier, CGString__new(text));
    BBToken* token2 = BBToken__new(BBTokenType_semicolon, CGString__new(text2));
    CGArray(BBToken)* tokenList = CGArray__newFromInitializerList(BBToken, token, token2, NULL);
    BBSentence* identifierSentence = BBSentence__new(CGString__new("identifierSentence"), BBTokenType_identifier, NULL);
    BBSentence* semicolonSentence = BBSentence__new(CGString__new("semicolonSentence"), BBTokenType_semicolon, NULL);
    BBPhrase* identifierPhrase = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, identifierSentence, NULL));
    CGAppState_catchAndDeleteException(CGAppState__getInstance());
    BBPhrase* semicolonPhrase = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, semicolonSentence, NULL));
    BBPhrase* identifierSemicolonPhrase = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, identifierSentence, semicolonSentence, NULL));
    BBAlternative* identifierSemicolonAlternative = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, identifierSemicolonPhrase, NULL));
    BBAlternative* semicolonAlternative = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, semicolonPhrase, NULL));
    BBSentence* definitionSentence = BBSentence__new(CGString__new("definitionSentence"), BBTokenType_definitionSign, CGArray__newFromInitializerList(BBAlternative, identifierSemicolonAlternative, semicolonAlternative, NULL));

    BB_RDParser* parser = BB_RDParser__new(definitionSentence);
    BB_RDParser_print(parser);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);
    assert(ast != NULL);
    assert(BBToken_getType(BBAst_getToken(ast)) == BBTokenType_definitionSign);
    assert(!CGString__compare(BBSentence_getName(BBAst_getSentence(ast)), "definitionSentence"));
    CGAppState_THROW(CGAppState__getInstance(), Severity_notice, CGExceptionID_GeneralNonfatalException, "please add tests for sub-sentence");
    BBAst_print(ast, 0);

    BB_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}
/*
    definition ::= identifier+ ;
*/
void testRepetition() {
    printf("%s...\n", __func__);

    CGAppState_catchAndDeleteException(CGAppState__getInstance());
    CGString* text = CGString__new("ident1");
    CGString* text2 = CGString__new("ident2");
    BBToken* token = BBToken__new(BBTokenType_identifier, CGString__new(text));
    BBToken* token2 = BBToken__new(BBTokenType_identifier, CGString__new(text2));
    CGArray(BBToken)* tokenList = CGArray__newFromInitializerList(BBToken, token, token2, NULL);
    BBSentence* identifierSentence = BBSentence__new(CGString__new("identifierSentence"), BBTokenType_identifier, NULL);
    BBPhrase* identifierPhrase = BBPhrase__new(BBPhraseRepeat_many, CGArray__newFromInitializerList(BBSentence, identifierSentence, NULL));
    BBAlternative* identifierAlternative = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, identifierPhrase, NULL));
    BBSentence* definitionSentence = BBSentence__new(CGString__new("definitionSentence"), BBTokenType_definitionSign, CGArray__newFromInitializerList(BBAlternative, identifierAlternative, NULL));

    BB_RDParser* parser = BB_RDParser__new(definitionSentence);
    BB_RDParser_print(parser);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);
    assert(ast != NULL);
    assert(BBToken_getType(BBAst_getToken(ast)) == BBTokenType_definitionSign);
    assert(!CGString__compare(BBSentence_getName(BBAst_getSentence(ast)), "definitionSentence"));
    CGAppState_THROW(CGAppState__getInstance(), Severity_notice, CGExceptionID_GeneralNonfatalException, "please add tests for sub-sentence");
    BBAst_print(ast, 0);

    BB_RDParser_delete(parser);

    printf("%s ok\n", __func__);
}


int main() {
    CGAppState__init(__FILE__);
    appState = CGAppState__getInstance();
    printf("=== %s ===\n", __FILE__);
    startRule = BBScannerRuleset__getInstance();

    testNewDelete();
    testOneTerminalDefinition();
    testOneAlternativeWithoutRepetition();
    testTwoAlternativesWithoutRepetition();
    testTwoAlternativesAndTwoPhrasesWithoutRepetition();
    testRepetition();

    /* testing errors */
    /* testUnexpectedEOF(); */
    printf("WARNING: testUnexpectedEOF has been commented-out.\n");

    printf("=== %s ok ===\n", __FILE__);
    CGAppState__deInit();
	return 0;
}
