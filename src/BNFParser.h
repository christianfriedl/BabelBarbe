#ifndef _BNF_PARSER_H
#define _BNF_PARSER_H

#include"BNF.h"
#include"BNFScanner.h"
#include"BNFToken.h"
#include"BNFAst.h"

struct BNFParser_struct;
typedef struct BNFParser_struct BNFParser;
struct BNFSentence;

typedef enum { BNFPhraseRepeat_once=0, BNFPhraseRepeat_many } BNFPhraseRepeatSwitch;

typedef struct {
    BNFPhraseRepeatSwitch repeatSwitch;
    CGArray(BNFSentence)* parts;
} BNFPhrase;

typedef struct {
    CGArray(BNFPhrase)* phrases[20];
} BNFAlternative;

struct BNFSentence {
    char name[255]; /* for debugging, and for emitting c-code */
    BNFAst* (*parse)(BNFParser* parser);
    BNFAlternative alternatives[20];
};

struct BNFParser_struct {
    BNFSentence* startSentence;
    BNFAst* rootAst;
    CGArray(BNFToken)* tokens;
}; 

BNFParser* BNFParser__new(CGArray(BNFToken)* tokens, BNFSentence* startSentence);
void BNFParser_delete(Parser* parser);

#endif
