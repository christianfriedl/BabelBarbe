#ifndef _BNF_SCANNER_RULE_H
#define _BNF_SCANNER_RULE_H

#include<stdio.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGArray.h>
#include<cgenerics/CGString.h>

struct BNFScannerRule_struct;
typedef struct BNFScannerRule_struct BNFScannerRule;

typedef enum { BNFScannerRuleType_string = 0, BNFScannerRuleType_regex = 1 } BNFScannerRuleType;


DECLARE_ARRAY_TYPE(BNFScannerRule)

struct BNFScannerRule_struct {
    BNFScannerRuleType type;
    CGString *pattern;

    CGArray(BNFScannerRule)* followupRules;
};
    

BNFScannerRule* BNFScannerRule_clone(CGAppState* appState, BNFScannerRule* this);
BNFScannerRule* BNFScannerRule__new(CGAppState* appState, BNFScannerRuleType type, CGString* pattern, CGArray(BNFScannerRule)* followupRules);
void BNFScannerRule_delete(CGAppState* appState, BNFScannerRule* this);

#endif
