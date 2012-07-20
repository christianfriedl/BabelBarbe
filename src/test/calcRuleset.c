#include"/opt/local/include/pcre.h"
#include<bbarbe/BBTokenType.h>
#include<bbarbe/BBToken.h>
#include<bbarbe/BBScannerRule.h>
#include<bbarbe/BB_RDParser.h>

BBTokenType* BBTokenType_nonTerminal = NULL;
BBTokenType* tokenTypeNoise = NULL;
BBTokenType* BBTokenType_expression0 = NULL;
BBTokenType* BBTokenType_expression1 = NULL;
BBTokenType* BBTokenType_term0 = NULL;
BBTokenType* BBTokenType_term1 = NULL;
BBTokenType* BBTokenType_factor0 = NULL;
BBTokenType* BBTokenType_factor1 = NULL;
BBTokenType* BBTokenType_literal0 = NULL;
BBScannerNode* scannerNode0 = NULL;
BBScannerNode* scannerNode1 = NULL;
BBScannerNode* scannerNode2 = NULL;
BBScannerNode* scannerNode3 = NULL;
BBScannerNode* scannerNode4 = NULL;
BBScannerNode* scannerNode5 = NULL;
BBScannerNode* scannerNode6 = NULL;
BBScannerRule* scannerRuleStart = NULL;
BBScannerRule* scannerRuleNoise = NULL;
BBScannerNode* scannerNodeNoise = NULL;
BBSentence* expressionStringLiteral0Sentence = NULL;
BBSentence* expressionStringLiteral1Sentence = NULL;
BBSentence* termStringLiteral0Sentence = NULL;
BBSentence* termStringLiteral1Sentence = NULL;
BBSentence* factorStringLiteral0Sentence = NULL;
BBSentence* factorStringLiteral1Sentence = NULL;
BBSentence* literalRegexLiteral0Sentence = NULL;
BBSentence* expressionSentence = NULL;
BBSentence* termSentence = NULL;
BBSentence* factorSentence = NULL;
BBSentence* literalSentence = NULL;
BBAlternative* expressionSentenceAlternative0 = NULL;
BBAlternative* expressionSentenceAlternative1 = NULL;
BBAlternative* expressionSentenceAlternative2 = NULL;
BBAlternative* termSentenceAlternative0 = NULL;
BBAlternative* termSentenceAlternative1 = NULL;
BBAlternative* factorSentenceAlternative0 = NULL;
BBAlternative* factorSentenceAlternative1 = NULL;
BBAlternative* literalSentenceAlternative0 = NULL;
BBPhrase* expressionSentenceAlternative0Phrase0 = NULL;
BBPhrase* expressionSentenceAlternative0Phrase1 = NULL;
BBPhrase* expressionSentenceAlternative0Phrase2 = NULL;
BBPhrase* expressionSentenceAlternative1Phrase0 = NULL;
BBPhrase* expressionSentenceAlternative1Phrase1 = NULL;
BBPhrase* expressionSentenceAlternative1Phrase2 = NULL;
BBPhrase* expressionSentenceAlternative2Phrase0 = NULL;
BBPhrase* termSentenceAlternative0Phrase0 = NULL;
BBPhrase* termSentenceAlternative0Phrase1 = NULL;
BBPhrase* termSentenceAlternative0Phrase2 = NULL;
BBPhrase* termSentenceAlternative1Phrase0 = NULL;
BBPhrase* termSentenceAlternative1Phrase1 = NULL;
BBPhrase* termSentenceAlternative1Phrase2 = NULL;
BBPhrase* factorSentenceAlternative0Phrase0 = NULL;
BBPhrase* factorSentenceAlternative1Phrase0 = NULL;
BBPhrase* factorSentenceAlternative1Phrase1 = NULL;
BBPhrase* factorSentenceAlternative1Phrase2 = NULL;
BBPhrase* literalSentenceAlternative0Phrase0 = NULL;

BBScannerRule* createScannerRuleset() {
    BBTokenType_nonTerminal = BBTokenType__new(13, "nonTerminal");
    BBTokenType_expression0 = BBTokenType__new(14, "expression0");
    BBTokenType_expression1 = BBTokenType__new(15, "expression1");
    BBTokenType_term0 = BBTokenType__new(16, "term0");
    BBTokenType_term1 = BBTokenType__new(17, "term1");
    BBTokenType_factor0 = BBTokenType__new(18, "factor0");
    BBTokenType_factor1 = BBTokenType__new(19, "factor1");
    BBTokenType_literal0 = BBTokenType__new(20, "literal0");
    scannerRuleStart = BBScannerRule__new(CGString__new("start"), NULL);
    scannerRuleNoise = BBScannerRule__new(CGString__new("noise"), NULL);
    scannerNodeNoise = BBScannerNode__new(BBScannerNodeType_regex, "\\s*", scannerRuleStart, BBTokenType_noise, true, NULL);
    scannerNode0 = BBScannerNode__new(BBScannerNodeType_string, CGString__new("+"), scannerRuleNoise, BBTokenType_expression0, false, NULL);
    scannerNode1 = BBScannerNode__new(BBScannerNodeType_string, CGString__new("-"), scannerRuleNoise, BBTokenType_expression1, false, NULL);
    scannerNode2 = BBScannerNode__new(BBScannerNodeType_string, CGString__new("*"), scannerRuleNoise, BBTokenType_term0, false, NULL);
    scannerNode3 = BBScannerNode__new(BBScannerNodeType_string, CGString__new("/"), scannerRuleNoise, BBTokenType_term1, false, NULL);
    scannerNode4 = BBScannerNode__new(BBScannerNodeType_string, CGString__new("("), scannerRuleNoise, BBTokenType_factor0, false, NULL);
    scannerNode5 = BBScannerNode__new(BBScannerNodeType_string, CGString__new(")"), scannerRuleNoise, BBTokenType_factor1, false, NULL);
    scannerNode6 = BBScannerNode__new(BBScannerNodeType_regex, CGString__new("\\d+"), scannerRuleNoise, BBTokenType_literal0, false, NULL);
    BBScannerRule_setNodes(scannerRuleStart, CGArray__newFromInitializerList(BBScannerNode, scannerNode0, scannerNode1, scannerNode2, scannerNode3, scannerNode4, scannerNode5, scannerNode6, scannerNodeNoise, NULL));
    BBScannerRule_setNodes(scannerRuleNoise, CGArray__newFromInitializerList(BBScannerNode, scannerNodeNoise, NULL));
    return scannerRuleStart;
}

BBSentence* createParserRuleset() {
    expressionStringLiteral0Sentence = BBSentence__newTerminalSymbol(CGString__new("expressionStringLiteral0"), BBTokenType_expression0);
    expressionStringLiteral1Sentence = BBSentence__newTerminalSymbol(CGString__new("expressionStringLiteral1"), BBTokenType_expression1);
    termStringLiteral0Sentence = BBSentence__newTerminalSymbol(CGString__new("termStringLiteral0"), BBTokenType_term0);
    termStringLiteral1Sentence = BBSentence__newTerminalSymbol(CGString__new("termStringLiteral1"), BBTokenType_term1);
    factorStringLiteral0Sentence = BBSentence__newTerminalSymbol(CGString__new("factorStringLiteral0"), BBTokenType_factor0);
    factorStringLiteral1Sentence = BBSentence__newTerminalSymbol(CGString__new("factorStringLiteral1"), BBTokenType_factor1);
    literalRegexLiteral0Sentence = BBSentence__newTerminalSymbol(CGString__new("literalRegexLiteral0"), BBTokenType_literal0);
    expressionSentence = BBSentence__new(CGString__new("expression"), BBTokenType_nonTerminal, CGArray__new(BBAlternative, 1));
    expressionSentenceAlternative0 = BBAlternative__new(CGArray__new(BBPhrase, 1));
    expressionSentenceAlternative1 = BBAlternative__new(CGArray__new(BBPhrase, 1));
    expressionSentenceAlternative2 = BBAlternative__new(CGArray__new(BBPhrase, 1));
    expressionSentenceAlternative0Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    expressionSentenceAlternative0Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    expressionSentenceAlternative0Phrase2 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    expressionSentenceAlternative1Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    expressionSentenceAlternative1Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    expressionSentenceAlternative1Phrase2 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    expressionSentenceAlternative2Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    BBAlternative_addPhrase(expressionSentenceAlternative0, expressionSentenceAlternative0Phrase0);
    BBAlternative_addPhrase(expressionSentenceAlternative0, expressionSentenceAlternative0Phrase1);
    BBAlternative_addPhrase(expressionSentenceAlternative0, expressionSentenceAlternative0Phrase2);
    BBAlternative_addPhrase(expressionSentenceAlternative1, expressionSentenceAlternative1Phrase0);
    BBAlternative_addPhrase(expressionSentenceAlternative1, expressionSentenceAlternative1Phrase1);
    BBAlternative_addPhrase(expressionSentenceAlternative1, expressionSentenceAlternative1Phrase2);
    BBAlternative_addPhrase(expressionSentenceAlternative2, expressionSentenceAlternative2Phrase0);
    termSentence = BBSentence__new(CGString__new("term"), BBTokenType_nonTerminal, CGArray__new(BBAlternative, 1));
    termSentenceAlternative0 = BBAlternative__new(CGArray__new(BBPhrase, 1));
    termSentenceAlternative1 = BBAlternative__new(CGArray__new(BBPhrase, 1));
    termSentenceAlternative0Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    termSentenceAlternative0Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    termSentenceAlternative0Phrase2 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    termSentenceAlternative1Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    termSentenceAlternative1Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    termSentenceAlternative1Phrase2 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    BBAlternative_addPhrase(termSentenceAlternative0, termSentenceAlternative0Phrase0);
    BBAlternative_addPhrase(termSentenceAlternative0, termSentenceAlternative0Phrase1);
    BBAlternative_addPhrase(termSentenceAlternative0, termSentenceAlternative0Phrase2);
    BBAlternative_addPhrase(termSentenceAlternative1, termSentenceAlternative1Phrase0);
    BBAlternative_addPhrase(termSentenceAlternative1, termSentenceAlternative1Phrase1);
    BBAlternative_addPhrase(termSentenceAlternative1, termSentenceAlternative1Phrase2);
    factorSentence = BBSentence__new(CGString__new("factor"), BBTokenType_nonTerminal, CGArray__new(BBAlternative, 1));
    factorSentenceAlternative0 = BBAlternative__new(CGArray__new(BBPhrase, 1));
    factorSentenceAlternative1 = BBAlternative__new(CGArray__new(BBPhrase, 1));
    factorSentenceAlternative0Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    factorSentenceAlternative1Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    factorSentenceAlternative1Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    factorSentenceAlternative1Phrase2 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    BBAlternative_addPhrase(factorSentenceAlternative0, factorSentenceAlternative0Phrase0);
    BBAlternative_addPhrase(factorSentenceAlternative1, factorSentenceAlternative1Phrase0);
    BBAlternative_addPhrase(factorSentenceAlternative1, factorSentenceAlternative1Phrase1);
    BBAlternative_addPhrase(factorSentenceAlternative1, factorSentenceAlternative1Phrase2);
    literalSentence = BBSentence__new(CGString__new("literal"), BBTokenType_nonTerminal, CGArray__new(BBAlternative, 1));
    literalSentenceAlternative0 = BBAlternative__new(CGArray__new(BBPhrase, 1));
    literalSentenceAlternative0Phrase0 = BBPhrase__new(BBPhraseRepeat_once, CGArray__new(BBSentence, 1));
    BBAlternative_addPhrase(literalSentenceAlternative0, literalSentenceAlternative0Phrase0);
    BBSentence_addAlternative(expressionSentence, expressionSentenceAlternative0);
    BBSentence_addAlternative(expressionSentence, expressionSentenceAlternative1);
    BBSentence_addAlternative(expressionSentence, expressionSentenceAlternative2);
    BBPhrase_addPart(expressionSentenceAlternative0Phrase0, termSentence);
    BBPhrase_addPart(expressionSentenceAlternative0Phrase1, expressionStringLiteral0Sentence);
    BBPhrase_addPart(expressionSentenceAlternative0Phrase2, termSentence);
    BBPhrase_addPart(expressionSentenceAlternative1Phrase0, termSentence);
    BBPhrase_addPart(expressionSentenceAlternative1Phrase1, expressionStringLiteral1Sentence);
    BBPhrase_addPart(expressionSentenceAlternative1Phrase2, termSentence);
    BBPhrase_addPart(expressionSentenceAlternative2Phrase0, termSentence);
    BBSentence_addAlternative(termSentence, termSentenceAlternative0);
    BBSentence_addAlternative(termSentence, termSentenceAlternative1);
    BBPhrase_addPart(termSentenceAlternative0Phrase0, factorSentence);
    BBPhrase_addPart(termSentenceAlternative0Phrase1, termStringLiteral0Sentence);
    BBPhrase_addPart(termSentenceAlternative0Phrase2, factorSentence);
    BBPhrase_addPart(termSentenceAlternative1Phrase0, factorSentence);
    BBPhrase_addPart(termSentenceAlternative1Phrase1, termStringLiteral1Sentence);
    BBPhrase_addPart(termSentenceAlternative1Phrase2, factorSentence);
    BBSentence_addAlternative(factorSentence, factorSentenceAlternative0);
    BBSentence_addAlternative(factorSentence, factorSentenceAlternative1);
    BBPhrase_addPart(factorSentenceAlternative0Phrase0, literalSentence);
    BBPhrase_addPart(factorSentenceAlternative1Phrase0, factorStringLiteral0Sentence);
    BBPhrase_addPart(factorSentenceAlternative1Phrase1, expressionSentence);
    BBPhrase_addPart(factorSentenceAlternative1Phrase2, factorStringLiteral1Sentence);
    BBSentence_addAlternative(literalSentence, literalSentenceAlternative0);
    BBPhrase_addPart(literalSentenceAlternative0Phrase0, literalRegexLiteral0Sentence);
    return expressionSentence;
}
