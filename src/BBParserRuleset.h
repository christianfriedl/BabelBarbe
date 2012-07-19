#ifndef _BB_PARSER_RULESET_H
#define _BB_PARSER_RULESET_H

#include"BB_RDParser.h"

BBSentence* stringLiteralSentence;
BBSentence* regexLiteralSentence;
BBSentence* repetitionMarkerZeroOrOnceSentence;
BBSentence* repetitionMarkerZeroOrMoreSentence;
BBSentence* repetitionMarkerManySentence;
BBSentence* openParenSentence;
BBSentence* closeParenSentence;
BBSentence* orSignSentence;
BBSentence* semicolonSentence;
BBSentence* definitionSignSentence;
BBSentence* identifierSentence;

/* nonterminals */

BBPhrase* terminalSymbolAlternative1Phrase;
BBPhrase* terminalSymbolAlternative2Phrase;
BBAlternative* terminalSymbolAlternative1;
BBAlternative* terminalSymbolAlternative2;
BBSentence* terminalSymbolSentence;

BBPhrase* symbolAlternative1Phrase1;
BBPhrase* symbolAlternative2Phrase1;
BBAlternative* symbolAlternative1;
BBAlternative* symbolAlternative2;
BBSentence* symbolSentence;

BBPhrase* termAlternative1Phrase1;
BBSentence* alternativeSentence;
BBPhrase* termAlternative2Phrase1;
BBAlternative* termAlternative1;
BBAlternative* termAlternative2;
BBSentence* termSentence;

BBPhrase* repetitionMarkerAlternative1Phrase1;
BBPhrase* repetitionMarkerAlternative2Phrase1;
BBPhrase* repetitionMarkerAlternative3Phrase1;
BBAlternative* repetitionMarkerAlternative1;
BBAlternative* repetitionMarkerAlternative2;
BBAlternative* repetitionMarkerAlternative3;
BBSentence* repetitionMarkerSentence;

BBPhrase* phraseAlternative1Phrase1;
BBPhrase* phraseAlternative1Phrase2;
BBAlternative* phraseAlternative1;
BBSentence* phraseSentence;

BBPhrase* alternativeAlternative1Phrase1;
BBAlternative* alternativeAlternative1;

BBPhrase* alternativesAlternative1Phrase1;
BBPhrase* alternativesAlternative1Phrase2;
BBAlternative* alternativesAlternative1;
BBSentence* alternativesSentence;

BBPhrase* ruleAlternative1Phrase1;
BBAlternative* ruleAlternative1;
BBSentence* ruleSentence;

BBPhrase* startAlternative1Phrase1;
BBAlternative* startAlternative1;
BBSentence* startSentence;


BBSentence* BBParserRuleset__getInstance();

#endif
