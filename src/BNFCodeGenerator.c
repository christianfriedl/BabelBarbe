#include"BNFCodeGenerator.h"

DEFINE_ARRAY_FUNCS(CGString)

BNFCodeGenerator* BNFCodeGenerator__new(BNFAst* ast) {
    BNFCodeGenerator* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->ast = ast;
        this->startSentence = NULL;
        this->currentSentenceStack = CGArray__new(BNFSentence, 4);
        this->nextTokenType = 0;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, CGExceptionID_CannotAllocate, "unable to allocate BNFCodeGenerator");
    return this;
}

void BNFCodeGenerator_delete(BNFCodeGenerator* this) {
    free(this);
    /* TODO ast?? */
}

unsigned int BNFCodeGenerator_createTokenType(BNFCodeGenerator* this) {
    return this->nextTokenType++;
}


static BNFSentence* BNFCodeGenerator_getCurrentSentence_(BNFCodeGenerator* this) {
    return CGArray_getValueAt(BNFSentence, this->currentSentenceStack, CGArray_getSize(BNFSentence, this->currentSentenceStack) - 1);
}
static repeatSwitch getRepeatSwitchFromRepetitionMarkerSentence_(BNFAst* ast) {
    BNFSentence* sentence = BNFAst_getSentence(BNFAst_getSubAstAt(BNFAst_getSentence(ast), 0));
    switch(sentence) {
        case repetitionMarkerZeroOrOnceSentence:
            return BNFPhraseRepeat_zeroOrOnce;
        case repetitionMarkerZeroOrMoreSentence:
            return BNFPhraseRepeat_zeroOrMore;
        case repetitionMarkerManySentence:
            return BNFPhraseRepeat_many;
        default:
            CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "Repetition marker expected");
    }
}
/*
 *  phrase ::= term repetition_marker? ;
 */
static void BNFCodeGenerator_handlePhraseSentence_(BNFCodeGenerator* this, BNFAst* ast) {
    BNFPhraseRepeatSwitch repeatSwitch = BNFPhraseRepeat_once;
    BNFAst* termAst = BNFAst_getSubAstAt(ast, 0);
    if (BNFAst_getSubAstsSize(ast) > 1) /* determine repeat switch */
        repeatSwitch = getRepeatSwitchFromRepetitionMarkerSentence_(BNFAst_getSubAstAt(ast, 1));
        
    BNFPhrase* phrase = BNFPhrase__new(repeatSwitch, CGArray__new(BNFSentence, 1)); /* sentences could lead upwards! */
    CGTree(BNFAst)* astTree = NULL;
    CGArrayOfCGTreeOfBNFAstIterator* iter = BNFAst_getSubAstIterator(ast);
    while ((astTree = CGArrayIterator_fetch(CGTreeOfBNFAst, iter)) != NULL)
        if (BNFAst_getSentence(CGTree_getValue(BNFAst, astTree)) == phraseSentence)
            BNFCodeGenerator_handleTermSentence_(this, CGTree_getValue(BNFAst, astTree));
}
/*
 * alternative ::= phrase+ ;
 */
static void BNFCodeGenerator_handleAlternativeSentence_(BNFCodeGenerator* this, BNFAst* ast) {
    BNFAlternative* alternative = BNFAlternative__new(CGArray__new(BNFPhrase, 1));
    BNFSentence_addAlternative(BNFCodeGenerator_getCurrentSentence_(this), alternative);

    CGTree(BNFAst)* astTree = NULL;
    CGArrayOfCGTreeOfBNFAstIterator* iter = BNFAst_getSubAstIterator(ast);
    while ((astTree = CGArrayIterator_fetch(CGTreeOfBNFAst, iter)) != NULL)
        if (BNFAst_getSentence(CGTree_getValue(BNFAst, astTree)) == phraseSentence)
            BNFCodeGenerator_handlePhraseSentence_(this, CGTree_getValue(BNFAst, astTree));
}
static void BNFCodeGenerator_handleAlternativesSentence_(BNFCodeGenerator* this, BNFAst* ast) {
    CGTree(BNFAst)* astTree = NULL;
    CGArrayOfCGTreeOfBNFAstIterator* iter = BNFAst_getSubAstIterator(ast);

    // we don't have to amend the current sentence because it is a nonterminal and already has an alternatives array

    while ((astTree = CGArrayIterator_fetch(CGTreeOfBNFAst, iter)) != NULL)
        if (BNFAst_getSentence(CGTree_getValue(BNFAst, astTree)) == alternativeSentence) /* ignore the '|' sign - we might want the parser to do that... */
            BNFCodeGenerator_handleAlternativeSentence_(this, CGTree_getValue(BNFAst, astTree));
    CGArrayOfCGTreeOfBNFAstIterator_delete(iter);
}
static void BNFCodeGenerator_handleRuleSentence_(BNFCodeGenerator* this, BNFAst* ast) {
    BNFAst* identifierAst = BNFAst_getSubAstAt(ast, 0);
    BNFAst* alternativesAst = BNFAst_getSubAstAt(ast, 2);
    if (BNFAst_getSentence(identifierAst) == identifierSentence) {
        /* create new sentence */
        CGString* name = NULL;
        asprintf(&name, "%sSentence", BNFToken_getText(BNFAst_getToken(identifierAst)));
        BNFSentence* sentence = BNFSentence__newNonTerminalSymbol(name, 0);

        /* add new sentence to start rule */
        CGArray(BNFSentence)* parts = CGArray__newFromInitializerList(BNFSentence, sentence, NULL);
        BNFPhrase* phrase = BNFPhrase__new(BNFPhraseRepeat_once, parts);
        CGArray(BNFPhrase)* phrases = CGArray__newFromInitializerList(BNFPhrase, phrase, NULL);
        BNFAlternative* alternative = BNFAlternative__new(phrases);
        BNFSentence* currentSentence = BNFCodeGenerator_getCurrentSentence_(this);
        BNFSentence_addAlternative(currentSentence, alternative);

        CGArray_push(BNFSentence, this->currentSentenceStack, sentence); 

        BNFCodeGenerator_handleAlternativesSentence_(this, alternativesAst);

        CGArray_pop(BNFSentence, this->currentSentenceStack);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "Identifier expected");
}

static void BNFCodeGenerator_handleStartSentence_(BNFCodeGenerator* this, BNFAst* ast) {
    this->startSentence = BNFSentence__newNonTerminalSymbol("startSentence", BNFCodeGenerator_createTokenType(this)); 
    CGArray_push(BNFSentence, this->currentSentenceStack, this->startSentence); 
    
    CGTree(BNFAst)* astTree = NULL;
    CGArrayOfCGTreeOfBNFAstIterator* iter = BNFAst_getSubAstIterator(ast);

    while ((astTree = CGArrayIterator_fetch(CGTreeOfBNFAst, iter)) != NULL)
        BNFCodeGenerator_handleRuleSentence_(this, CGTree_getValue(BNFAst, astTree));
    CGArrayOfCGTreeOfBNFAstIterator_delete(iter);
}




CGString* BNFCodeGenerator_createCode(BNFCodeGenerator* this) {
    if (BNFAst_getSentence(this->ast) == startSentence)
        BNFCodeGenerator_handleStartSentence_(this, this->ast);
    else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "No start sentence found.");
    printf("----> resulting ruleset: \n");
    CGArray(BNFSentence)* seenSentences = CGArray__new(BNFSentence, 16);
    BNFSentence_print(this->startSentence, 0, seenSentences);
    return "";
}
