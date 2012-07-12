#include"BNFParserRuleset.h"

BNFSentence* BNFParserRuleset__getInstance() {

    /* terminals */
    stringLiteralSentence = BNFSentence__new(CGString__new("stringLiteralSentence"), BNFTokenType_stringLiteral, NULL);
    regexLiteralSentence = BNFSentence__new(CGString__new("regexLiteralSentence"), BNFTokenType_regexLiteral, NULL);
    repetitionMarkerZeroOrOnceSentence = BNFSentence__new(CGString__new("repetitionMarkerZeroOrOnceSentence"), BNFTokenType_repeatZeroOrOnce, NULL);
    repetitionMarkerZeroOrMoreSentence = BNFSentence__new(CGString__new("repetitionMarkerZeroOrMoreSentence"), BNFTokenType_repeatZeroOrMore, NULL);
    repetitionMarkerManySentence = BNFSentence__new(CGString__new("repetitionMarkerManySentence"), BNFTokenType_repeatMany, NULL);
    openParenSentence = BNFSentence__new(CGString__new("openParenSentence"), BNFTokenType_openParen, NULL);
    closeParenSentence = BNFSentence__new(CGString__new("closeParenSentence"), BNFTokenType_closeParen, NULL);
    orSignSentence = BNFSentence__new(CGString__new("orSignSentence"), BNFTokenType_OrSign, NULL);
    semicolonSentence = BNFSentence__new(CGString__new("semicolonSentence"), BNFTokenType_semicolon, NULL);
    definitionSignSentence = BNFSentence__new(CGString__new("definitionSignSentence"), BNFTokenType_definitionSign, NULL);
    identifierSentence = BNFSentence__new(CGString__new("identifierSentence"), BNFTokenType_identifier, NULL);

    /* nonterminals */

    terminalSymbolAlternative1Phrase = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, stringLiteralSentence, NULL));
    terminalSymbolAlternative2Phrase = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, regexLiteralSentence, NULL));
    terminalSymbolAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, terminalSymbolAlternative1Phrase, NULL));
    terminalSymbolAlternative2 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, terminalSymbolAlternative2Phrase, NULL));
    terminalSymbolSentence = BNFSentence__new(CGString__new("terminalSymbolSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, terminalSymbolAlternative1, terminalSymbolAlternative2, NULL));

    symbolAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, terminalSymbolSentence, NULL));
    symbolAlternative2Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, identifierSentence, NULL));
    symbolAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, symbolAlternative1Phrase1, NULL));
    symbolAlternative2 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, symbolAlternative2Phrase1, NULL));
    symbolSentence = BNFSentence__new(CGString__new("symbolSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, symbolAlternative1, symbolAlternative2, NULL));

    termAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, symbolSentence, NULL));
    alternativeSentence = BNFSentence__new(CGString__new("alternativeSentence"), BNFTokenType_nonTerminal, NULL);
    termAlternative2Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, openParenSentence, alternativeSentence, closeParenSentence, NULL)); /* recursion to alternativeSentence! */
    termAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, termAlternative1Phrase1, NULL));
    termAlternative2 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, termAlternative2Phrase1, NULL));
    termSentence = BNFSentence__new(CGString__new("termSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, termAlternative1, termAlternative2, NULL));

    repetitionMarkerAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, repetitionMarkerZeroOrOnceSentence, NULL));
    repetitionMarkerAlternative2Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, repetitionMarkerZeroOrMoreSentence, NULL));
    repetitionMarkerAlternative3Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, repetitionMarkerManySentence, NULL));
    repetitionMarkerAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, repetitionMarkerAlternative1Phrase1, NULL));
    repetitionMarkerAlternative2 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, repetitionMarkerAlternative2Phrase1, NULL));
    repetitionMarkerAlternative3 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, repetitionMarkerAlternative3Phrase1, NULL));
    repetitionMarkerSentence = BNFSentence__new(CGString__new("repetitionMarkerSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, repetitionMarkerAlternative1, repetitionMarkerAlternative2, repetitionMarkerAlternative3, NULL));

    phraseAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, termSentence, NULL));
    phraseAlternative1Phrase2 = BNFPhrase__new(BNFPhraseRepeat_zeroOrOnce, CGArray__newFromInitializerList(BNFSentence, repetitionMarkerSentence, NULL));
    phraseAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, phraseAlternative1Phrase1, phraseAlternative1Phrase2, NULL));
    phraseSentence = BNFSentence__new(CGString__new("phraseSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, phraseAlternative1, NULL));

    alternativeAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_many, CGArray__newFromInitializerList(BNFSentence, phraseSentence, NULL));
    alternativeAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, alternativeAlternative1Phrase1, NULL));
    BNFSentence_setAlternatives(alternativeSentence, CGArray__newFromInitializerList(BNFAlternative, alternativeAlternative1, NULL));

    alternativesAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, alternativeSentence, NULL));
    alternativesAlternative1Phrase2 = BNFPhrase__new(BNFPhraseRepeat_many, CGArray__newFromInitializerList(BNFSentence, orSignSentence, alternativeSentence, NULL));
    alternativesAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, alternativesAlternative1Phrase1, alternativesAlternative1Phrase2, NULL));
    alternativesSentence = BNFSentence__new(CGString__new("alternativesSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, alternativesAlternative1, NULL));
    
    ruleAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_once, CGArray__newFromInitializerList(BNFSentence, identifierSentence, definitionSignSentence, alternativesSentence, semicolonSentence, NULL));
    ruleAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, ruleAlternative1Phrase1, NULL));
    ruleSentence = BNFSentence__new(CGString__new("ruleSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, ruleAlternative1, NULL));

    startAlternative1Phrase1 = BNFPhrase__new(BNFPhraseRepeat_many, CGArray__newFromInitializerList(BNFSentence, ruleSentence, NULL));
    startAlternative1 = BNFAlternative__new(CGArray__newFromInitializerList(BNFPhrase, startAlternative1Phrase1, NULL));
    startSentence = BNFSentence__new(CGString__new("startSentence"), BNFTokenType_nonTerminal, CGArray__newFromInitializerList(BNFAlternative, startAlternative1, NULL));

    return startSentence;
}

