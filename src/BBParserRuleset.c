#include"BBParserRuleset.h"

BBSentence* BBParserRuleset__getInstance() {

    /* terminals */
    stringLiteralSentence = BBSentence__new(CGString__new("stringLiteralSentence"), BBTokenType_stringLiteral, NULL);
    regexLiteralSentence = BBSentence__new(CGString__new("regexLiteralSentence"), BBTokenType_regexLiteral, NULL);
    repetitionMarkerZeroOrOnceSentence = BBSentence__new(CGString__new("repetitionMarkerZeroOrOnceSentence"), BBTokenType_repeatZeroOrOnce, NULL);
    repetitionMarkerZeroOrMoreSentence = BBSentence__new(CGString__new("repetitionMarkerZeroOrMoreSentence"), BBTokenType_repeatZeroOrMore, NULL);
    repetitionMarkerManySentence = BBSentence__new(CGString__new("repetitionMarkerManySentence"), BBTokenType_repeatMany, NULL);
    openParenSentence = BBSentence__new(CGString__new("openParenSentence"), BBTokenType_openParen, NULL);
    closeParenSentence = BBSentence__new(CGString__new("closeParenSentence"), BBTokenType_closeParen, NULL);
    orSignSentence = BBSentence__new(CGString__new("orSignSentence"), BBTokenType_OrSign, NULL);
    semicolonSentence = BBSentence__new(CGString__new("semicolonSentence"), BBTokenType_semicolon, NULL);
    definitionSignSentence = BBSentence__new(CGString__new("definitionSignSentence"), BBTokenType_definitionSign, NULL);
    identifierSentence = BBSentence__new(CGString__new("identifierSentence"), BBTokenType_identifier, NULL);

    /* nonterminals */

    terminalSymbolAlternative1Phrase = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, stringLiteralSentence, NULL));
    terminalSymbolAlternative2Phrase = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, regexLiteralSentence, NULL));
    terminalSymbolAlternative1 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, terminalSymbolAlternative1Phrase, NULL));
    terminalSymbolAlternative2 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, terminalSymbolAlternative2Phrase, NULL));
    terminalSymbolSentence = BBSentence__new(CGString__new("terminalSymbolSentence"), BBTokenType_nonTerminal, CGArray__newFromInitializerList(BBAlternative, terminalSymbolAlternative1, terminalSymbolAlternative2, NULL));

    symbolAlternative1Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, terminalSymbolSentence, NULL));
    symbolAlternative2Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, identifierSentence, NULL));
    symbolAlternative1 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, symbolAlternative1Phrase1, NULL));
    symbolAlternative2 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, symbolAlternative2Phrase1, NULL));
    symbolSentence = BBSentence__new(CGString__new("symbolSentence"), BBTokenType_nonTerminal, CGArray__newFromInitializerList(BBAlternative, symbolAlternative1, symbolAlternative2, NULL));

    termAlternative1Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, symbolSentence, NULL));
    alternativeSentence = BBSentence__new(CGString__new("alternativeSentence"), BBTokenType_nonTerminal, NULL);
    termAlternative2Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, openParenSentence, alternativeSentence, closeParenSentence, NULL)); /* recursion to alternativeSentence! */
    termAlternative1 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, termAlternative1Phrase1, NULL));
    termAlternative2 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, termAlternative2Phrase1, NULL));
    termSentence = BBSentence__new(CGString__new("termSentence"), BBTokenType_nonTerminal, CGArray__newFromInitializerList(BBAlternative, termAlternative1, termAlternative2, NULL));

    repetitionMarkerAlternative1Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, repetitionMarkerZeroOrOnceSentence, NULL));
    repetitionMarkerAlternative2Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, repetitionMarkerZeroOrMoreSentence, NULL));
    repetitionMarkerAlternative3Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, repetitionMarkerManySentence, NULL));
    repetitionMarkerAlternative1 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, repetitionMarkerAlternative1Phrase1, NULL));
    repetitionMarkerAlternative2 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, repetitionMarkerAlternative2Phrase1, NULL));
    repetitionMarkerAlternative3 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, repetitionMarkerAlternative3Phrase1, NULL));
    repetitionMarkerSentence = BBSentence__new(CGString__new("repetitionMarkerSentence"), BBTokenType_nonTerminal, CGArray__newFromInitializerList(BBAlternative, repetitionMarkerAlternative1, repetitionMarkerAlternative2, repetitionMarkerAlternative3, NULL));

    phraseAlternative1Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, termSentence, NULL));
    phraseAlternative1Phrase2 = BBPhrase__new(BBPhraseRepeat_zeroOrOnce, CGArray__newFromInitializerList(BBSentence, repetitionMarkerSentence, NULL));
    phraseAlternative1 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, phraseAlternative1Phrase1, phraseAlternative1Phrase2, NULL));
    phraseSentence = BBSentence__new(CGString__new("phraseSentence"), BBTokenType_nonTerminal, CGArray__newFromInitializerList(BBAlternative, phraseAlternative1, NULL));

    alternativeAlternative1Phrase1 = BBPhrase__new(BBPhraseRepeat_many, CGArray__newFromInitializerList(BBSentence, phraseSentence, NULL));
    alternativeAlternative1 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, alternativeAlternative1Phrase1, NULL));
    BBSentence_setAlternatives(alternativeSentence, CGArray__newFromInitializerList(BBAlternative, alternativeAlternative1, NULL));

    alternativesAlternative1Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, alternativeSentence, NULL));
    alternativesAlternative1Phrase2 = BBPhrase__new(BBPhraseRepeat_many, CGArray__newFromInitializerList(BBSentence, orSignSentence, alternativeSentence, NULL));
    alternativesAlternative1 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, alternativesAlternative1Phrase1, alternativesAlternative1Phrase2, NULL));
    alternativesSentence = BBSentence__new(CGString__new("alternativesSentence"), BBTokenType_nonTerminal, CGArray__newFromInitializerList(BBAlternative, alternativesAlternative1, NULL));
    
    ruleAlternative1Phrase1 = BBPhrase__new(BBPhraseRepeat_once, CGArray__newFromInitializerList(BBSentence, identifierSentence, definitionSignSentence, alternativesSentence, semicolonSentence, NULL));
    ruleAlternative1 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, ruleAlternative1Phrase1, NULL));
    ruleSentence = BBSentence__new(CGString__new("ruleSentence"), BBTokenType_nonTerminal, CGArray__newFromInitializerList(BBAlternative, ruleAlternative1, NULL));

    startAlternative1Phrase1 = BBPhrase__new(BBPhraseRepeat_many, CGArray__newFromInitializerList(BBSentence, ruleSentence, NULL));
    startAlternative1 = BBAlternative__new(CGArray__newFromInitializerList(BBPhrase, startAlternative1Phrase1, NULL));
    startSentence = BBSentence__new(CGString__new("startSentence"), BBTokenType_nonTerminal, CGArray__newFromInitializerList(BBAlternative, startAlternative1, NULL));

    return startSentence;
}

