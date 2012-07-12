#ifndef _BNF_PARSER_RULESET_H
#define _BNF_PARSER_RULESET_H

#include"BNF_RDParser.h"

    BNFSentence* stringLiteralSentence;
    BNFSentence* regexLiteralSentence;
    BNFSentence* repetitionMarkerZeroOrOnceSentence;
    BNFSentence* repetitionMarkerZeroOrMoreSentence;
    BNFSentence* repetitionMarkerManySentence;
    BNFSentence* openParenSentence;
    BNFSentence* closeParenSentence;
    BNFSentence* orSignSentence;
    BNFSentence* semicolonSentence;
    BNFSentence* definitionSignSentence;
    BNFSentence* identifierSentence;

    /* nonterminals */

    BNFPhrase* terminalSymbolAlternative1Phrase;
    BNFPhrase* terminalSymbolAlternative2Phrase;
    BNFAlternative* terminalSymbolAlternative1;
    BNFAlternative* terminalSymbolAlternative2;
    BNFSentence* terminalSymbolSentence;

    BNFPhrase* symbolAlternative1Phrase1;
    BNFPhrase* symbolAlternative2Phrase1;
    BNFAlternative* symbolAlternative1;
    BNFAlternative* symbolAlternative2;
    BNFSentence* symbolSentence;

    BNFPhrase* termAlternative1Phrase1;
    BNFSentence* alternativeSentence;
    BNFPhrase* termAlternative2Phrase1;
    BNFAlternative* termAlternative1;
    BNFAlternative* termAlternative2;
    BNFSentence* termSentence;

    BNFPhrase* repetitionMarkerAlternative1Phrase1;
    BNFPhrase* repetitionMarkerAlternative2Phrase1;
    BNFPhrase* repetitionMarkerAlternative3Phrase1;
    BNFAlternative* repetitionMarkerAlternative1;
    BNFAlternative* repetitionMarkerAlternative2;
    BNFAlternative* repetitionMarkerAlternative3;
    BNFSentence* repetitionMarkerSentence;

    BNFPhrase* phraseAlternative1Phrase1;
    BNFPhrase* phraseAlternative1Phrase2;
    BNFAlternative* phraseAlternative1;
    BNFSentence* phraseSentence;

    BNFPhrase* alternativeAlternative1Phrase1;
    BNFAlternative* alternativeAlternative1;

    BNFPhrase* alternativesAlternative1Phrase1;
    BNFPhrase* alternativesAlternative1Phrase2;
    BNFAlternative* alternativesAlternative1;
    BNFSentence* alternativesSentence;
    
    BNFPhrase* ruleAlternative1Phrase1;
    BNFAlternative* ruleAlternative1;
    BNFSentence* ruleSentence;

    BNFPhrase* startAlternative1Phrase1;
    BNFAlternative* startAlternative1;
    BNFSentence* startSentence;


BNFSentence* BNFParserRuleset__getInstance();

#endif
