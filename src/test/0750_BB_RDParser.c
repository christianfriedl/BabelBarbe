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

/*
 * start ::= rule1 | rule2 ;
 * rule1 ::= identifier semicolon ;
 * rule2 ::= identifier definitionSign ;
 */
void testTwoAlternativesWithSameStartKeyword() {
    printf("%s...\n", __func__);

    CGAppState_reset(CGAppState__getInstance());
    CGString* nonterminalName = CGString__new("nonterminal");
    CGString* identName = CGString__new("ident");
    CGString* semicolonName = CGString__new("semicolon");
    CGString* definitionSignName = CGString__new("definitionSign");
    BBToken* tokenNonterminal = BBToken__new(BBTokenType_nonTerminal, CGString__new(identName));
    BBToken* tokenIdent = BBToken__new(BBTokenType_identifier, CGString__new(identName));
    BBToken* tokenSemicolon = BBToken__new(BBTokenType_semicolon, CGString__new(semicolonName));
    BBToken* tokenDefinitionSign = BBToken__new(BBTokenType_definitionSign, CGString__new(definitionSignName));

    BBSentence* identifierSentence = BBSentence__new(identName, BBTokenType_identifier, NULL);
    BBSentence* semicolonSentence = BBSentence__new(semicolonName, BBTokenType_semicolon, NULL);
    BBSentence* definitionSignSentence = BBSentence__new(definitionSignName, BBTokenType_definitionSign, NULL);

    BBPhrase* rule1Alternative0Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, identifierSentence, semicolonSentence, NULL));
    BBAlternative* rule1Alternative0 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, rule1Alternative0Phrase0, NULL));
    BBSentence* rule1Sentence = BBSentence__new(CGString__new("rule1"), BBTokenType_nonTerminal, CGArray__newFromInitializerList(BBAlternative, rule1Alternative0, NULL));

    BBPhrase* rule2Alternative0Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, identifierSentence, definitionSignSentence, NULL));
    BBAlternative* rule2Alternative0 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, rule2Alternative0Phrase0, NULL));
    BBSentence* rule2Sentence = BBSentence__new(CGString__new("rule2"), BBTokenType_nonTerminal, CGArray__newFromInitializerList(BBAlternative, rule2Alternative0, NULL));

    BBPhrase* startAlternative0Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, rule1Sentence, NULL));
    BBPhrase* startAlternative1Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, rule2Sentence, NULL));
    BBAlternative* startAlternative0 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, startAlternative0Phrase0, NULL));
    BBAlternative* startAlternative1 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, startAlternative1Phrase0, NULL));
    BBSentence* startSentence = BBSentence__new(CGString__new("start"), BBTokenType_nonTerminal, CGArray__newFromInitializerList(BBAlternative, startAlternative0, startAlternative1, NULL));

    BB_RDParser* parser = BB_RDParser__new(startSentence);
    BB_RDParser_print(parser);

    /* 1. identifier semicolon */
    CGArray(BBToken)* tokenList = CGArray__newFromInitializerList(BBToken, tokenIdent, tokenSemicolon, NULL);
    BBAst* ast = BB_RDParser_parse(parser, tokenList);
    BBAst_print(ast, 0);
    assert(BBAst_getSentence(ast) == startSentence);
    BBAst* ruleLeaf = BBAst_getSubAstAt(ast, 0);
    assert(BBAst_getSentence(ruleLeaf) == rule1Sentence);
    BBAst* identLeaf = BBAst_getSubAstAt(ruleLeaf, 0);
    assert(BBAst_getSentence(identLeaf) == identifierSentence);
    BBAst* secondLeaf = BBAst_getSubAstAt(ruleLeaf, 1);
    assert(BBAst_getSentence(secondLeaf) == semicolonSentence);

    /* 1. identifier definitionSign */
    tokenList = CGArray__newFromInitializerList(BBToken, tokenIdent, tokenDefinitionSign, NULL);
    ast = BB_RDParser_parse(parser, tokenList);
    BBAst_print(ast, 0);
    assert(BBAst_getSentence(ast) == startSentence);
    ruleLeaf = BBAst_getSubAstAt(ast, 0);
    assert(BBAst_getSentence(ruleLeaf) == rule2Sentence);
    identLeaf = BBAst_getSubAstAt(ruleLeaf, 0);
    assert(BBAst_getSentence(identLeaf) == identifierSentence);
    secondLeaf = BBAst_getSubAstAt(ruleLeaf, 1);
    assert(BBAst_getSentence(secondLeaf) == definitionSignSentence);



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
    testTwoAlternativesWithSameStartKeyword();

    /* testing errors */
    /* testUnexpectedEOF(); */
    printf("WARNING: testUnexpectedEOF has been commented-out.\n");

    printf("=== %s ok ===\n", __FILE__);
    CGAppState__deInit();
	return 0;
}
