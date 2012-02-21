#ifndef _BNF_SCANNER_RULE_H
#define _BNF_SCANNER_RULE_H

#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGArray.h>
#include<cgenerics/CGString.h>
#include"BNFToken.h"

struct BNFScannerRule_struct;
typedef struct BNFScannerRule_struct BNFScannerRule;

typedef enum { BNFScannerNodeType_string = 0, BNFScannerNodeType_regex = 1 } BNFScannerNodeType;

typedef struct {
    BNFScannerNodeType type;
    CGString* pattern;
    BNFScannerRule* followupRule;
    BNFToken* token;
} BNFScannerNode;

DECLARE_ARRAY_TYPE(BNFScannerNode)

struct BNFScannerRule_struct {

    CGArray(BNFScannerNode)* nodes;
};
    

BNFScannerNode* BNFScannerNode__new(CGAppState* appState, BNFScannerNodeType type, CGString* pattern, BNFScannerRule* followupRule, BNFToken* token);
BNFScannerNode* BNFScannerNode_clone(CGAppState* appState, BNFScannerNode* this);
void BNFScannerNode_delete(CGAppState* appState, BNFScannerNode* this);

BNFScannerRule* BNFScannerRule__new(CGAppState* appState, CGArray(BNFScannerNode)* nodes);
BNFScannerRule* BNFScannerRule_clone(CGAppState* appState, BNFScannerRule* this);
void BNFScannerRule_delete(CGAppState* appState, BNFScannerRule* this);

#endif
