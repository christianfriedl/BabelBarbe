#ifndef _BNF_SCANNER_RULE_H
#define _BNF_SCANNER_RULE_H

#include<stdio.h>
#include<pcre.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGArray.h>
#include<cgenerics/CGArrayIterator.h>
#include<cgenerics/CGString.h>
#include"BNFToken.h"

#define BNFScannerNode__PCRE_OVECTOR_COUNT (30)

struct BNFScannerRule_struct;
typedef struct BNFScannerRule_struct BNFScannerRule;

typedef enum { BNFScannerNodeType_string = 0, BNFScannerNodeType_regex = 1 } BNFScannerNodeType;

typedef struct {
    BNFScannerNodeType type;        /* this scanner node can represent a static string literal, or regular expression */
    CGString* pattern;              /* the pattern, represented as a string - either literal, or regex */
    pcre* regex;                    /* the compiled regex, used internally */
    BNFScannerRule* followupRule;   /* if this node matches, then the scanner should move to this rule */
    BNFTokenType* tokenType;         /* if this node matches, then this is its resulting token type */
    bool isNoise;                   /* true if this node represents "noise", i.e. whitespace */
    BNFToken* (*onAfterScanToken)(BNFToken*);    /* callback function to do postprocessing on the token; can be NULL
                                                        NOTE: this HAS to create a new token from the old one - the old one gets delete'd in the scanner
                                                  */
} BNFScannerNode;

DECLARE_ARRAY_TYPE(BNFScannerNode)
DECLARE_ARRAY_ITERATOR_TYPE(BNFScannerNode)
DECLARE_ARRAY_FUNCS(BNFScannerNode)
DECLARE_ARRAY_ITERATOR_FUNCS(BNFScannerNode)

struct BNFScannerRule_struct {
    CGString* name;                 /* used for debugging, but will also be good for error detection */
    CGArray(BNFScannerNode)* nodes; /* the alternative nodes that can be applied at this point in the scanning process */
    BNFScannerNode* successNode;    /* the resulting node, (uh, where is the node text actually?) */
};

BNFScannerNode* BNFScannerNode__new(BNFScannerNodeType type, CGString* pattern, BNFScannerRule* followupRule, BNFTokenType* tokenType, bool isNoise, BNFToken* (*onAfterScanToken)(BNFToken*));
BNFScannerNode* BNFScannerNode_clone(BNFScannerNode* this);
void BNFScannerNode_delete(BNFScannerNode* this);
bool BNFScannerNode_setRegex(BNFScannerNode* this, CGString* pattern);
BNFTokenType* BNFScannerNode_getTokenType(BNFScannerNode* this);
BNFScannerRule* BNFScannerNode_getFollowupRule(BNFScannerNode* this);
void BNFScannerNode_setFollowupRule(BNFScannerNode* this, BNFScannerRule* rule);
bool BNFScannerNode_getIsNoise(BNFScannerNode* this);
BNFToken* BNFScannerNode_applyToText(BNFScannerNode* this, const CGString* text);
CGString* BNFScannerNode_createCDeclaration(BNFScannerNode* this, unsigned int index);
CGString* BNFScannerNode_createCConstructor(BNFScannerNode* this, unsigned int index);

BNFScannerRule* BNFScannerRule__new(CGString* name, CGArray(BNFScannerNode)* nodes);
BNFScannerRule* BNFScannerRule_clone(BNFScannerRule* this);
void BNFScannerRule_delete(BNFScannerRule* this);
CGString* BNFScannerRule_getName(BNFScannerRule* this);
BNFToken* BNFScannerRule_applyToText(BNFScannerRule* this, const CGString* text);
BNFScannerNode* BNFScannerRule_getSuccessNode(BNFScannerRule* this);
void BNFScannerRule_setNodes(BNFScannerRule* this, CGArray(BNFScannerNode)* nodes);
BNFToken* BNFScannerNode_runOnAfterScanToken(BNFScannerNode* this, BNFToken* token);


#endif
