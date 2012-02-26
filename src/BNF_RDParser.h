#ifndef _BNF_PARSER_H
#define _BNF_PARSER_H

#include"BNF.h"
#include"BNFScanner.h"
#include"BNFToken.h"
#include"BNFAst.h"

struct BNF_RDParser_struct;
typedef struct BNF_RDParser_struct BNF_RDParser;
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
    BNFAst* (*parse)(BNF_RDParser* parser);
    CGArray(BNFAlternative)* alternatives;
};

struct BNF_RDParser_struct {
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

BNFSentence* BNFSentence__new(CGString* name, BNFAst* (*parse)(BNF_RDParser* parser), CGArray(BNFAlternative)* alternatives);
BNFSentence* BNFSentence_clone(BNFSentence* this);
void BNFSentence_delete(BNFSentence* this);

BNF_RDParser* BNF_RDParser__new(CGArray(BNFToken)* tokenList, BNFSentence* startSentence);
void BNF_RDParser_delete(BNF_RDParser* this);

#endif
