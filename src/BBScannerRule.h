#ifndef _BB_SCANNER_RULE_H
#define _BB_SCANNER_RULE_H

#include<stdio.h>
#include<pcre.h>
#include<cgenerics/CGAppState.h>
#include<cgenerics/CGArray.h>
#include<cgenerics/CGArrayIterator.h>
#include<cgenerics/CGString.h>
#include"BBToken.h"

#define BBScannerNode__PCRE_OVECTOR_COUNT (30)

struct BBScannerRule_struct;
typedef struct BBScannerRule_struct BBScannerRule;

typedef enum { BBScannerNodeType_string = 0, BBScannerNodeType_regex = 1 } BBScannerNodeType;

typedef struct {
    BBScannerNodeType type;        /* this scanner node can represent a static string literal, or regular expression */
    CGString* pattern;              /* the pattern, represented as a string - either literal, or regex */
    BBScannerRule* followupRule;   /* if this node matches, then the scanner should move to this rule */
    BBTokenType* tokenType;         /* if this node matches, then this is its resulting token type */
    bool isNoise;                   /* true if this node represents "noise", i.e. whitespace */
    BBToken* (*onAfterScanToken)(BBToken*);    /* callback function to do postprocessing on the token; can be NULL
                                                        NOTE: this HAS to create a new token from the old one - the old one gets delete'd in the scanner
                                                  */
    pcre* regex;                    /* the compiled regex, used internally */
} BBScannerNode;

DECLARE_ARRAY_TYPE(BBScannerNode)
DECLARE_ARRAY_ITERATOR_TYPE(BBScannerNode)
DECLARE_ARRAY_FUNCS(BBScannerNode)
DECLARE_ARRAY_ITERATOR_FUNCS(BBScannerNode)

struct BBScannerRule_struct {
    CGString* name;                 /* used for debugging, but will also be good for error detection */
    CGArray(BBScannerNode)* nodes; /* the alternative nodes that can be applied at this point in the scanning process */
    BBScannerNode* successNode;    /* the resulting node, (uh, where is the node text actually?) */
};

BBScannerNode* BBScannerNode__new(BBScannerNodeType type, CGString* pattern, BBScannerRule* followupRule, BBTokenType* tokenType, bool isNoise, BBToken* (*onAfterScanToken)(BBToken*));
BBScannerNode* BBScannerNode_clone(BBScannerNode* this);
void BBScannerNode_delete(BBScannerNode* this);
bool BBScannerNode_setRegex(BBScannerNode* this, CGString* pattern);
BBTokenType* BBScannerNode_getTokenType(BBScannerNode* this);
BBScannerRule* BBScannerNode_getFollowupRule(BBScannerNode* this);
void BBScannerNode_setFollowupRule(BBScannerNode* this, BBScannerRule* rule);
bool BBScannerNode_getIsNoise(BBScannerNode* this);
BBToken* BBScannerNode_applyToText(BBScannerNode* this, const CGString* text);
CGString* BBScannerNode_createCDeclaration(BBScannerNode* this, unsigned int index);
CGString* BBScannerNode_createCConstructor(BBScannerNode* this, unsigned int index);

BBScannerRule* BBScannerRule__new(CGString* name, CGArray(BBScannerNode)* nodes);
BBScannerRule* BBScannerRule_clone(BBScannerRule* this);
void BBScannerRule_delete(BBScannerRule* this);
CGString* BBScannerRule_getName(BBScannerRule* this);
BBToken* BBScannerRule_applyToText(BBScannerRule* this, const CGString* text);
BBScannerNode* BBScannerRule_getSuccessNode(BBScannerRule* this);
void BBScannerRule_setNodes(BBScannerRule* this, CGArray(BBScannerNode)* nodes);
BBToken* BBScannerNode_runOnAfterScanToken(BBScannerNode* this, BBToken* token);


#endif
