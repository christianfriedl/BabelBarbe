#ifndef _BNF_PARSER_H
#define _BNF_PARSER_H

#include"BNF.h"
#include"BNFScanner.h"
#include"BNFToken.h"
#include"BNFAst.h"

struct BNFParser_struct;
typedef struct BNFParser_struct BNFParser;
struct BNFSentence_struct;
typedef struct BNFSentence_struct BNFSentence;
struct BNFPhrase_struct;
typedef struct BNFPhrase_struct BNFPhrase;
struct BNFAlternative_struct;
typedef struct BNFAlternative_struct BNFAlternative;

DECLARE_ARRAY(BNFSentence);
DECLARE_ARRAY(BNFPhrase);
DECLARE_ARRAY(BNFAlternative);


typedef enum { BNFPhraseRepeat_once=0, BNFPhraseRepeat_many } BNFPhraseRepeatSwitch;

struct BNFPhrase_struct {
    BNFPhraseRepeatSwitch repeatSwitch;
    CGArray(BNFSentence)* parts;
};

struct BNFAlternative_struct {
    CGArray(BNFPhrase)* phrases;
};

struct BNFSentence_struct {
    CGString* name; /* for debugging, and for emitting c-code */
    BNFAst* (*parse)(BNFParser* parser);
    CGArray(BNFAlternative)* alternatives;
};

struct BNFParser_struct {
    BNFSentence* startSentence;
    BNFAst* rootAst;
    CGArray(BNFToken)* tokenList;
}; 

BNFPhrase* BNFPhrase__new(BNFPhraseRepeatSwitch repeatSwitch, CGArray(BNFSentence)* parts);
BNFPhrase* BNFPhrase_clone(BNFPhrase* this);
void BNFPhrase_setParts(BNFPhrase* this, CGArray(BNFSentence)* parts);
void BNFPhrase_delete(BNFPhrase* this);

BNFAlternative* BNFAlternative__new(CGArray(BNFPhrase)* phrases);
BNFAlternative* BNFAlternative_clone(BNFAlternative* this);
void BNFAlternative_delete(BNFAlternative* this);

BNFSentence* BNFSentence__new(CGString* name, BNFAst* (*parse)(BNFParser* parser), CGArray(BNFAlternative)* alternatives);
BNFSentence* BNFSentence_clone(BNFSentence* this);
void BNFSentence_delete(BNFSentence* this);

BNFParser* BNFParser__new(CGArray(BNFToken)* tokenList, BNFSentence* startSentence);
void BNFParser_delete(BNFParser* this);

#endif
