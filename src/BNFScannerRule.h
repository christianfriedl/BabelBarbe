#ifndef _BNF_SCANNER_RULE_H
#define _BNF_SCANNER_RULE_H

#include<stdio.h>
#include<pcre.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGArray.h>
#include<cgenerics/CGArrayIterator.h>
#include<cgenerics/CGString.h>
#include"BNFToken.h"

struct BNFScannerRule_struct;
typedef struct BNFScannerRule_struct BNFScannerRule;

typedef enum { BNFScannerNodeType_string = 0, BNFScannerNodeType_regex = 1 } BNFScannerNodeType;

typedef struct {
    BNFScannerNodeType type;
    CGString* pattern;
    pcre* regex;
    BNFScannerRule* followupRule;
    BNFTokenType tokenType;
    bool isNoise;
} BNFScannerNode;

DECLARE_ARRAY_TYPE(BNFScannerNode)
DECLARE_ARRAY_ITERATOR_TYPE(BNFScannerNode)
DECLARE_ARRAY_FUNCS(BNFScannerNode)
DECLARE_ARRAY_ITERATOR_FUNCS(BNFScannerNode)

struct BNFScannerRule_struct {
    CGString* name;
    CGArray(BNFScannerNode)* nodes;
    BNFScannerNode* successNode;
};

BNFScannerNode* BNFScannerNode__new(BNFScannerNodeType type, CGString* pattern, BNFScannerRule* followupRule, BNFTokenType tokenType, bool isNoise);
BNFScannerNode* BNFScannerNode_clone(BNFScannerNode* this);
void BNFScannerNode_delete(BNFScannerNode* this);
bool BNFScannerNode_setRegex(BNFScannerNode* this, CGString* pattern);
BNFTokenType BNFScannerNode_getTokenType(BNFScannerNode* this);
BNFScannerRule* BNFScannerNode_getFollowupRule(BNFScannerNode* this);
void BNFScannerNode_setFollowupRule(BNFScannerNode* this, BNFScannerRule* rule);
bool BNFScannerNode_getIsNoise(BNFScannerNode* this);
BNFToken* BNFScannerNode_applyToText(BNFScannerNode* this, const CGString* text);

BNFScannerRule* BNFScannerRule__new(CGString* name, CGArray(BNFScannerNode)* nodes);
BNFScannerRule* BNFScannerRule_clone(BNFScannerRule* this);
void BNFScannerRule_delete(BNFScannerRule* this);
CGString* BNFScannerRule_getName(BNFScannerRule* this);
BNFToken* BNFScannerRule_applyToText(BNFScannerRule* this, const CGString* text);
BNFScannerNode* BNFScannerRule_getSuccessNode(BNFScannerRule* this);
void BNFScannerRule_setNodes(BNFScannerRule* this, CGArray(BNFScannerNode)* nodes);


#endif
