#ifndef _BB_RDPARSER_H
#define _BB_RDPARSER_H

#include"BB.h"
#include"BBScanner.h"
#include"BBToken.h"
#include"BBTokenType.h"

struct BBSentence_struct;
typedef struct BBSentence_struct BBSentence;
#include"BBAst.h"
struct BB_RDParser_struct;
typedef struct BB_RDParser_struct BB_RDParser;
struct BBPhrase_struct;
typedef struct BBPhrase_struct BBPhrase;
struct BBAlternative_struct;
typedef struct BBAlternative_struct BBAlternative;

DECLARE_ARRAY(BBSentence);
DECLARE_ARRAY(BBPhrase);
DECLARE_ARRAY(BBAlternative);
DECLARE_ARRAY(BBTokenType);
DECLARE_ARRAY_ITERATOR(BBToken);
DECLARE_ARRAY_ITERATOR(BBTokenType);
DECLARE_ARRAY_ITERATOR(BBAlternative);
DECLARE_ARRAY_ITERATOR(BBSentence);
DECLARE_ARRAY_ITERATOR(BBPhrase);


typedef enum { BBPhraseRepeat_once=0, BBPhraseRepeat_zeroOrOnce, BBPhraseRepeat_zeroOrMore, BBPhraseRepeat_many } BBPhraseRepeatSwitch;

struct BBPhrase_struct {
    BBPhraseRepeatSwitch repeatSwitch;
    CGArray(BBSentence)* parts;
};

struct BBAlternative_struct {
    CGArray(BBPhrase)* phrases;
};

struct BBSentence_struct {
    CGString* name; /* for debugging, and for emitting c-code */
    BBTokenType* tokenType;
    CGArray(BBAlternative)* alternatives;
};

struct BB_RDParser_struct {
    BBSentence* startSentence;
    CGArrayIterator(BBToken)* tokenListIterator;
}; 

CGString* BBPhraseRepeatSwitch_toString(BBPhraseRepeatSwitch this);

BBPhrase* BBPhrase__new(BBPhraseRepeatSwitch repeatSwitch, CGArray(BBSentence)* parts);
BBPhrase* BBPhrase_clone(BBPhrase* this);
void BBPhrase_setParts(BBPhrase* this, CGArray(BBSentence)* parts);
void BBPhrase_addPart(BBPhrase* this, BBSentence* part);
void BBPhrase_delete(BBPhrase* this);
void BBPhrase_print(BBPhrase* this, unsigned int indentationLevel, CGArray(BBSentence)* seenSentences);

BBAlternative* BBAlternative__new(CGArray(BBPhrase)* phrases);
BBAlternative* BBAlternative_clone(BBAlternative* this);
void BBAlternative_addPhrase(BBAlternative* this, BBPhrase* phrase);
void BBAlternative_delete(BBAlternative* this);
void BBAlternative_print(BBAlternative* this, unsigned int indentationLevel, CGArray(BBSentence)* seenSentences);

BBSentence* BBSentence__new(CGString* name, BBTokenType* tokenType, CGArray(BBAlternative)* alternatives);
BBSentence* BBSentence__newTerminalSymbol(CGString* name, BBTokenType* tokenType);
BBSentence* BBSentence__newNonTerminalSymbol(CGString* name, BBTokenType* tokenType);
CGString* BBSentence_getName(BBSentence* this);
BBTokenType* BBSentence_getTokenType(BBSentence* this);
BBSentence* BBSentence_clone(BBSentence* this);
void BBSentence_delete(BBSentence* this);
void BBSentence_print(BBSentence* this, unsigned int indentationLevel, CGArray(BBSentence)* seenSentences);
void BBSentence_setAlternatives(BBSentence* this, CGArray(BBAlternative)* alternatives);
void BBSentence_addAlternative(BBSentence* this, BBAlternative* alternative);

BB_RDParser* BB_RDParser__new(BBSentence* startSentence);
BBAst* BB_RDParser_parse(BB_RDParser* this, CGArray(BBToken)* tokenList);
void BB_RDParser_delete(BB_RDParser* this);
void BB_RDParser_print(BB_RDParser* this);
void BB_RDParser__printTokenList(CGArray(BBToken)* tokenList);

CGString* BBSentence_createCDeclaration(BBSentence* this);
CGString* BBSentence_createCConstructor(BBSentence* this);
CGString* BBSentence_createCAddAlternatives(BBSentence* this);
CGString* BBAlternative_createCConstructor(BBAlternative* this, CGString* sentenceName, unsigned index);
CGString* BBSentence_createCAlternativesConstructors(BBSentence* this);
CGString* BBAlternative_createCDeclaration(BBAlternative* this, CGString* sentenceName, unsigned index);
CGString* BBSentence_createCAlternativesDeclarations(BBSentence* this);
CGString* BBSentence_createCAlternativesConstructors(BBSentence* this);
CGString* BBPhrase_createCDeclaration(BBPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex);
CGString* BBPhrase_createCConstructor(BBPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex);
CGString* BBPhrase_createCAddParts(BBPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex);
CGString* BBAlternative_createCAddPhrases(BBAlternative* this, CGString* sentenceName, unsigned int alternativeIndex);
CGString* BBAlternative_createCPhrasesAddParts(BBAlternative* this, CGString* sentenceName, unsigned int alternativeIndex);
CGString* BBAlternative_createCPhrasesDeclarations(BBAlternative* this, CGString* sentenceName, unsigned int alternativeIndex);
CGString* BBAlternative_createCPhrasesConstructors(BBAlternative* this, CGString* sentenceName, unsigned int alternativeIndex);
CGString* BBSentence_createCAlternativesPhrasesAddParts(BBSentence* this);
CGString* BBSentence_createCAlternativesPhrasesDeclarations(BBSentence* this);
CGString* BBSentence_createCAlternativesPhrasesConstructors(BBSentence* this);
CGString* BBSentence_createCAlternativesAddPhrases(BBSentence* this);

#endif
