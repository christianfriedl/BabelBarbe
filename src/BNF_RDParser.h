#ifndef _BNF_RDPARSER_H
#define _BNF_RDPARSER_H

#include"BNF.h"
#include"BNFScanner.h"
#include"BNFToken.h"

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
DECLARE_ARRAY_ITERATOR(BNFToken);
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
    BNFTokenType tokenType;
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
void BNFPhrase_delete(BNFPhrase* this);
void BNFPhrase_print(BNFPhrase* this, unsigned int indentationLevel, CGArray(BNFSentence)* seenSentences);

BNFAlternative* BNFAlternative__new(CGArray(BNFPhrase)* phrases);
BNFAlternative* BNFAlternative_clone(BNFAlternative* this);
void BNFAlternative_delete(BNFAlternative* this);
void BNFAlternative_print(BNFAlternative* this, unsigned int indentationLevel, CGArray(BNFSentence)* seenSentences);

BNFSentence* BNFSentence__new(CGString* name, BNFTokenType tokenType, CGArray(BNFAlternative)* alternatives);
CGString* BNFSentence_getName(BNFSentence* this);
BNFSentence* BNFSentence_clone(BNFSentence* this);
void BNFSentence_delete(BNFSentence* this);
void BNFSentence_print(BNFSentence* this, unsigned int indentationLevel, CGArray(BNFSentence)* seenSentences);
void BNFSentence_setAlternatives(BNFSentence* this, CGArray(BNFAlternative)* alternatives);

BNF_RDParser* BNF_RDParser__new(BNFSentence* startSentence);
BNFAst* BNF_RDParser_parse(BNF_RDParser* this, CGArray(BNFToken)* tokenList);
void BNF_RDParser_delete(BNF_RDParser* this);
void BNF_RDParser_print(BNF_RDParser* this);


#endif
