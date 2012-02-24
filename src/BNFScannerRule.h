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
} BNFScannerNode;

DECLARE_ARRAY_TYPE(BNFScannerNode)
DECLARE_ARRAY_ITERATOR_TYPE(BNFScannerNode)
DECLARE_ARRAY_FUNCS(BNFScannerNode)
DECLARE_ARRAY_ITERATOR_FUNCS(BNFScannerNode)

struct BNFScannerRule_struct {
    CGArray(BNFScannerNode)* nodes;
};

BNFScannerNode* BNFScannerNode__new(CGAppState* appState, BNFScannerNodeType type, CGString* pattern, BNFScannerRule* followupRule, BNFTokenType tokenType);
BNFScannerNode* BNFScannerNode_clone(CGAppState* appState, BNFScannerNode* this);
void BNFScannerNode_delete(CGAppState* appState, BNFScannerNode* this);
bool BNFScannerNode_setRegex(CGAppState* appState, BNFScannerNode* this, CGString* pattern);
BNFTokenType BNFScannerNode_getTokenType(CGAppState* appState, BNFScannerNode* this);
BNFToken* BNFScannerNode_applyToText(CGAppState* appState, BNFScannerNode* this, const CGString* text);

BNFScannerRule* BNFScannerRule__new(CGAppState* appState, CGArray(BNFScannerNode)* nodes);
BNFScannerRule* BNFScannerRule_clone(CGAppState* appState, BNFScannerRule* this);
void BNFScannerRule_delete(CGAppState* appState, BNFScannerRule* this);
BNFToken* BNFScannerRule_applyToText(CGAppState* appState, BNFScannerRule* this, const CGString* text);


#endif
