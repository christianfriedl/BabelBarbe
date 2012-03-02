#include"BNFParserRuleset.h"

BNFSentence* BNFParserRuleset__getInstance() {

    /* terminals */
    BNFSentence* stringLiteralSentence = BNFSentence__new(CGString__new("stringLiteralSentence"), BNFTokenType_stringLiteral, NULL);
    BNFSentence* regexLiteralSentence = BNFSentence__new(CGString__new("regexLiteralSentence"), BNFTokenType_regexLiteral, NULL);
    BNFSentence* repetitionMarkerZeroOrOnceSentence = BNFSentence__new(CGString__new("repetitionMarkerZeroOrOnceSentence"), BNFTokenType_repeatZeroOrOnce, NULL);
    BNFSentence* repetitionMarkerZeroOrMoreSentence = BNFSentence__new(CGString__new("repetitionMarkerZeroOrMoreSentence"), BNFTokenType_repeatZeroOrMore, NULL);
    BNFSentence* repetitionMarkerManySentence = BNFSentence__new(CGString__new("repetitionMarkerManySentence"), BNFTokenType_repeatMany, NULL);
    BNFSentence* openParenSentence = BNFSentence__new(CGString__new("openParenSentence"), BNFTokenType_openParen, NULL);
    BNFSentence* closeParenSentence = BNFSentence__new(CGString__new("closeParenSentence"), BNFTokenType_closeParen, NULL);
    BNFSentence* orSignSentence = BNFSentence__new(CGString__new("orSignSentence"), BNFTokenType_OrSign, NULL);
    BNFSentence* semicolonSentence = BNFSentence__new(CGString__new("semicolonSentence"), BNFTokenType_semicolon, NULL);
    BNFSentence* definitionSignSentence = BNFSentence__new(CGString__new("definitionSignSentence"), BNFTokenType_definitionSign, NULL);
    BNFSentence* identifierSentence = BNFSentence__new(CGString__new("identifierSentence"), BNFTokenType_identifier, NULL);

    /* nonterminals */

    BNFPhrase* terminalSymbolAlternative1Phrase = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, stringLiteralSentence, NULL));
    BNFPhrase* terminalSymbolAlternative2Phrase = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, regexLiteralSentence, NULL));
    BNFAlternative* terminalSymbolAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, terminalSymbolAlternative1Phrase, NULL));
    BNFAlternative* terminalSymbolAlternative2 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, terminalSymbolAlternative2Phrase, NULL));
    BNFSentence* terminalSymbolSentence = BNFSentence__new(CGString__new("terminalSymbolSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, terminalSymbolAlternative1, terminalSymbolAlternative2, NULL));

    BNFPhrase* symbolAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, terminalSymbolSentence, NULL));
    BNFPhrase* symbolAlternative2Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, identifierSentence, NULL));
    BNFAlternative* symbolAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, symbolAlternative1Phrase1, NULL));
    BNFAlternative* symbolAlternative2 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, symbolAlternative2Phrase1, NULL));
    BNFSentence* symbolSentence = BNFSentence__new(CGString__new("symbolSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, symbolAlternative1, symbolAlternative2, NULL));

    BNFPhrase* termAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, symbolSentence, NULL));
    BNFSentence* alternativeSentence = BNFSentence__new(CGString__new("alternativeSentence"), BNFTokenType_nonTerminal, NULL);
    BNFPhrase* termAlternative2Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, openParenSentence, alternativeSentence, closeParenSentence, NULL)); /* recursion to alternativeSentence! */
    BNFAlternative* termAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, termAlternative1Phrase1, NULL));
    BNFAlternative* termAlternative2 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, termAlternative2Phrase1, NULL));
    BNFSentence* termSentence = BNFSentence__new(CGString__new("termSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, termAlternative1, termAlternative2, NULL));

    BNFPhrase* repetitionMarkerAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, repetitionMarkerZeroOrOnceSentence, NULL));
    BNFPhrase* repetitionMarkerAlternative2Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, repetitionMarkerZeroOrMoreSentence, NULL));
    BNFPhrase* repetitionMarkerAlternative3Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, repetitionMarkerManySentence, NULL));
    BNFAlternative* repetitionMarkerAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, repetitionMarkerAlternative1Phrase1, NULL));
    BNFAlternative* repetitionMarkerAlternative2 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, repetitionMarkerAlternative2Phrase1, NULL));
    BNFAlternative* repetitionMarkerAlternative3 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, repetitionMarkerAlternative3Phrase1, NULL));
    BNFSentence* repetitionMarkerSentence = BNFSentence__new(CGString__new("repetitionMarkerSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, repetitionMarkerAlternative1, repetitionMarkerAlternative2, repetitionMarkerAlternative3, NULL));

    BNFPhrase* phraseAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, termSentence, NULL));
    BNFPhrase* phraseAlternative1Phrase2 = BNFPhrase__new(BNFPhraseRepeat_zeroOrOnce, CGArray__newFromInitializerList(BNFSentence, repetitionMarkerSentence, NULL));
    BNFAlternative* phraseAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, phraseAlternative1Phrase1, phraseAlternative1Phrase2, NULL));
    BNFSentence* phraseSentence = BNFSentence__new(CGString__new("phraseSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, phraseAlternative1, NULL));

    BNFPhrase* alternativeAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_many, CGArray__newFromInitializerList(BNFSentence, phraseSentence, NULL));
    BNFAlternative* alternativeAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, alternativeAlternative1Phrase1, NULL));
    BNFSentence_setAlternatives(alternativeSentence, CGArray__newFromInitializerList(BNFAlternative, alternativeAlternative1, NULL));

    BNFPhrase* alternativesAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, alternativeSentence, NULL));
    BNFPhrase* alternativesAlternative1Phrase2 = BNFPhrase__new(BNFPhraseRepeat_many, CGArray__newFromInitializerList(BNFSentence, orSignSentence, alternativeSentence, NULL));
    BNFAlternative* alternativesAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, alternativesAlternative1Phrase1, alternativesAlternative1Phrase2, NULL));
    BNFSentence* alternativesSentence = BNFSentence__new(CGString__new("alternativesSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, alternativesAlternative1, NULL));
    
    BNFPhrase* ruleAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, identifierSentence, definitionSignSentence, alternativesSentence, semicolonSentence, NULL));
    BNFAlternative* ruleAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, ruleAlternative1Phrase1, NULL));
    BNFSentence* ruleSentence = BNFSentence__new(CGString__new("ruleSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, ruleAlternative1, NULL));

    BNFPhrase* startAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_many, CGArray__newFromInitializerList(BNFSentence, ruleSentence, NULL));
    BNFAlternative* startAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, startAlternative1Phrase1, NULL));
    BNFSentence* startSentence = BNFSentence__new(CGString__new("startSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, startAlternative1, NULL));

    return startSentence;
}

