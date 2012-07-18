#ifndef _BNF_RDPARSER_H
#define _BNF_RDPARSER_H

#include"BNF.h"
#include"BNFScanner.h"
#include"BNFToken.h"
#include"BNFTokenType.h"

struct BNFSentence_struct;
typedef struct BNFSentence_struct BNFSentence;
#include"BNFAst.h"
struct BNF_RDParser_struct;
typedef struct BNF_RDParser_struct BNF_RDParser;
struct BNFPhrase_struct;
typedef struct BNFPhrase_struct BNFPhrase;
struct BNFAlternative_struct;
typedef struct BNFAlternative_struct BNFAlternative;

DECLARE_ARRAY(BNFSentence);
DECLARE_ARRAY(BNFPhrase);
DECLARE_ARRAY(BNFAlternative);
DECLARE_ARRAY(BNFTokenType);
DECLARE_ARRAY_ITERATOR(BNFToken);
DECLARE_ARRAY_ITERATOR(BNFTokenType);
DECLARE_ARRAY_ITERATOR(BNFAlternative);
DECLARE_ARRAY_ITERATOR(BNFSentence);
DECLARE_ARRAY_ITERATOR(BNFPhrase);


typedef enum { BNFPhraseRepeat_once=0, BNFPhraseRepeat_zeroOrOnce, BNFPhraseRepeat_zeroOrMore, BNFPhraseRepeat_many } BNFPhraseRepeatSwitch;

struct BNFPhrase_struct {
    BNFPhraseRepeatSwitch repeatSwitch;
    CGArray(BNFSentence)* parts;
};

struct BNFAlternative_struct {
    CGArray(BNFPhrase)* phrases;
};

struct BNFSentence_struct {
    CGString* name; /* for debugging, and for emitting c-code */
    BNFTokenType* tokenType;
    CGArray(BNFAlternative)* alternatives;
};

struct BNF_RDParser_struct {
    BNFSentence* startSentence;
    CGArrayIterator(BNFToken)* tokenListIterator;
}; 

CGString* BNFPhraseRepeatSwitch_toString(BNFPhraseRepeatSwitch this);

BNFPhrase* BNFPhrase__new(BNFPhraseRepeatSwitch repeatSwitch, CGArray(BNFSentence)* parts);
BNFPhrase* BNFPhrase_clone(BNFPhrase* this);
void BNFPhrase_setParts(BNFPhrase* this, CGArray(BNFSentence)* parts);
void BNFPhrase_addPart(BNFPhrase* this, BNFSentence* part);
void BNFPhrase_delete(BNFPhrase* this);
void BNFPhrase_print(BNFPhrase* this, unsigned int indentationLevel, CGArray(BNFSentence)* seenSentences);

BNFAlternative* BNFAlternative__new(CGArray(BNFPhrase)* phrases);
BNFAlternative* BNFAlternative_clone(BNFAlternative* this);
void BNFAlternative_addPhrase(BNFAlternative* this, BNFPhrase* phrase);
void BNFAlternative_delete(BNFAlternative* this);
void BNFAlternative_print(BNFAlternative* this, unsigned int indentationLevel, CGArray(BNFSentence)* seenSentences);

BNFSentence* BNFSentence__new(CGString* name, BNFTokenType* tokenType, CGArray(BNFAlternative)* alternatives);
BNFSentence* BNFSentence__newTerminalSymbol(CGString* name, BNFTokenType* tokenType);
BNFSentence* BNFSentence__newNonTerminalSymbol(CGString* name, BNFTokenType* tokenType);
CGString* BNFSentence_getName(BNFSentence* this);
BNFTokenType* BNFSentence_getTokenType(BNFSentence* this);
BNFSentence* BNFSentence_clone(BNFSentence* this);
void BNFSentence_delete(BNFSentence* this);
void BNFSentence_print(BNFSentence* this, unsigned int indentationLevel, CGArray(BNFSentence)* seenSentences);
void BNFSentence_setAlternatives(BNFSentence* this, CGArray(BNFAlternative)* alternatives);
void BNFSentence_addAlternative(BNFSentence* this, BNFAlternative* alternative);

BNF_RDParser* BNF_RDParser__new(BNFSentence* startSentence);
BNFAst* BNF_RDParser_parse(BNF_RDParser* this, CGArray(BNFToken)* tokenList);
void BNF_RDParser_delete(BNF_RDParser* this);
void BNF_RDParser_print(BNF_RDParser* this);
void BNF_RDParser__printTokenList(CGArray(BNFToken)* tokenList);

CGString* BNFSentence_createCDeclaration(BNFSentence* this);
CGString* BNFSentence_createCConstructor(BNFSentence* this);
CGString* BNFSentence_createCAddAlternatives(BNFSentence* this);
CGString* BNFAlternative_createCConstructor(BNFAlternative* this, CGString* sentenceName, unsigned index);
CGString* BNFSentence_createCAlternativesConstructors(BNFSentence* this);
CGString* BNFAlternative_createCDeclaration(BNFAlternative* this, CGString* sentenceName, unsigned index);
CGString* BNFSentence_createCAlternativesDeclarations(BNFSentence* this);
CGString* BNFSentence_createCAlternativesConstructors(BNFSentence* this);
CGString* BNFPhrase_createCDeclaration(BNFPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex);
CGString* BNFPhrase_createCConstructor(BNFPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex);
CGString* BNFPhrase_createCAddParts(BNFPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex);
CGString* BNFAlternative_createCAddPhrases(BNFAlternative* this, CGString* sentenceName, unsigned int alternativeIndex);
CGString* BNFAlternative_createCPhrasesAddParts(BNFAlternative* this, CGString* sentenceName, unsigned int alternativeIndex);
CGString* BNFAlternative_createCPhrasesDeclarations(BNFAlternative* this, CGString* sentenceName, unsigned int alternativeIndex);
CGString* BNFAlternative_createCPhrasesConstructors(BNFAlternative* this, CGString* sentenceName, unsigned int alternativeIndex);
CGString* BNFSentence_createCAlternativesPhrasesAddParts(BNFSentence* this);
CGString* BNFSentence_createCAlternativesPhrasesDeclarations(BNFSentence* this);
CGString* BNFSentence_createCAlternativesPhrasesConstructors(BNFSentence* this);

#endif
