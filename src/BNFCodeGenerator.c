#include"BNFCodeGenerator.h"

DEFINE_ARRAY_FUNCS(CGString)

static BNFSentence* BNFCodeGenerator_getCurrentSentence_(BNFCodeGenerator* this);
static int BNFCodeGenerator__compareSentenceNames_(const BNFSentence **first, const BNFSentence **second);

BNFCodeGenerator* BNFCodeGenerator__new(BNFAst* ast) {
    BNFCodeGenerator* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->ast = ast;
        this->startSentence = NULL;
        this->currentSentenceStack = CGArray__new(BNFSentence, 4);
        this->nonTerminalTokenType = BNFTokenTypeFactory_createBNFTokenType(BNFTokenTypeFactory__getInstance(), CGString__new("nonTerminal"));
        this->ruleTokenTypeCount = 0;
        this->ruleSentenceCount = 0;
        this->ruleName = "start";
        this->ruleSentences = CGArray__new(BNFSentence, 16);
        this->terminalSentences = CGArray__new(BNFSentence, 16);
        this->tokenTypes = CGArray__new(BNFTokenType, 16);
        this->scannerNodes = CGArray__new(BNFScannerNode, 16);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, CGExceptionID_CannotAllocate, "unable to allocate BNFCodeGenerator");
    return this;
}

void BNFCodeGenerator_delete(BNFCodeGenerator* this) {
    free(this);
    /* TODO ast?? */
}

BNFTokenType* BNFCodeGenerator_createTokenType(BNFCodeGenerator* this) {
    CGString* name = NULL;
    if (CGArray_getSize(BNFSentence, this->currentSentenceStack) > 0)
        name = CGString__newWithSprintf("%s%u", this->ruleName, this->ruleTokenTypeCount++);
    else
        name = "Unknown token type";
    printf("--> createing token '%s' in rule %s\n", name, this->ruleName);

    BNFTokenType* tt = BNFTokenTypeFactory_createBNFTokenType(BNFTokenTypeFactory__getInstance(), name);
    CGArray_add(BNFTokenType, this->tokenTypes, tt);
    return tt;
}

static int BNFCodeGenerator__compareSentenceNames_(const BNFSentence **first, const BNFSentence **second) {
    BNFSentence* unconstFirst = (BNFSentence*) *first;
    BNFSentence* unconstSecond = (BNFSentence*) *second;
    return CGString__compare(BNFSentence_getName(unconstFirst), BNFSentence_getName(unconstSecond));
}

static BNFSentence* BNFCodeGenerator_getCurrentSentence_(BNFCodeGenerator* this) {
    return CGArray_getValueAt(BNFSentence, this->currentSentenceStack, CGArray_getSize(BNFSentence, this->currentSentenceStack) - 1);
}
static int getRepeatSwitchFromRepetitionMarkerSentence_(BNFAst* ast) {
    BNFSentence* sentence = BNFAst_getSentence(BNFAst_getSubAstAt(ast, 0));
    if (sentence == repetitionMarkerZeroOrOnceSentence)
        return BNFPhraseRepeat_zeroOrOnce;
    else if (sentence == repetitionMarkerZeroOrMoreSentence)
        return BNFPhraseRepeat_zeroOrMore;
    else if (sentence == repetitionMarkerManySentence)
        return BNFPhraseRepeat_many;
    else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "Repetition marker expected");
    return 0;
}
/* end helper functions */

static void BNFCodeGenerator_handleStringLiteralSentence_(BNFCodeGenerator* this, BNFAst* ast, BNFPhrase* phrase) {
        CGString* name = NULL;
        asprintf(&name, "%sStringLiteral%u", this->ruleName, this->ruleSentenceCount++);
        BNFTokenType* tokenType = BNFCodeGenerator_createTokenType(this);
        BNFSentence* sentence = BNFSentence__newTerminalSymbol(name, tokenType);
        CGArray(BNFSentence)* parts = CGArray__newFromInitializerList(BNFSentence, sentence, NULL);
        BNFPhrase_setParts(phrase, parts);
        CGArray_add(BNFSentence, this->terminalSentences, sentence);
        BNFScannerNode* node = BNFScannerNode__new(BNFScannerNodeType_string, BNFToken_getText(BNFAst_getToken(ast)), NULL, tokenType, false, NULL);
        CGArray_add(BNFScannerNode, this->scannerNodes, node);
}
static void BNFCodeGenerator_handleRegexLiteralSentence_(BNFCodeGenerator* this, BNFAst* ast, BNFPhrase* phrase) {
        CGString* name = NULL;
        asprintf(&name, "%sRegexLiteral%u", this->ruleName, this->ruleSentenceCount++);
        BNFTokenType* tokenType = BNFCodeGenerator_createTokenType(this);
        BNFSentence* sentence = BNFSentence__newTerminalSymbol(name, tokenType);
        CGArray(BNFSentence)* parts = CGArray__newFromInitializerList(BNFSentence, sentence, NULL);
        BNFPhrase_setParts(phrase, parts);
        CGArray_add(BNFSentence, this->terminalSentences, sentence);
        BNFScannerNode* node = BNFScannerNode__new(BNFScannerNodeType_regex, BNFToken_getText(BNFAst_getToken(ast)), NULL, tokenType, false, NULL);
        CGArray_add(BNFScannerNode, this->scannerNodes, node);
}
static void BNFCodeGenerator_handleTerminalSymbolSentence_(BNFCodeGenerator* this, BNFAst* ast, BNFPhrase* phrase) {
    BNFSentence* sentence = BNFAst_getSentence(BNFAst_getSubAstAt(ast, 0));
    if (sentence == stringLiteralSentence)
        BNFCodeGenerator_handleStringLiteralSentence_(this, BNFAst_getSubAstAt(ast, 0), phrase);
    else if (sentence == regexLiteralSentence)
        BNFCodeGenerator_handleRegexLiteralSentence_(this, BNFAst_getSubAstAt(ast, 0), phrase);
    else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "String or Regex literal expected");
}
static void BNFCodeGenerator_handleIdentifierSentence_(BNFCodeGenerator* this, BNFAst* ast, BNFPhrase* phrase) {
        CGString* name = NULL;
        asprintf(&name, "%s", BNFToken_getText(BNFAst_getToken(ast)));
        BNFSentence* namedSentence = BNFSentence__newNonTerminalSymbol(name, this->nonTerminalTokenType);
        BNFSentence* sentence = CGArray_find(BNFSentence, this->ruleSentences, namedSentence, BNFCodeGenerator__compareSentenceNames_);
        if (sentence == NULL) {
            CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "Rule '%s' not found", name);
            return;
        }
        CGArray(BNFSentence)* parts = CGArray__newFromInitializerList(BNFSentence, sentence, NULL);
        BNFPhrase_setParts(phrase, parts);
}
static void BNFCodeGenerator_handleSymbolSentence_(BNFCodeGenerator* this, BNFAst* ast, BNFPhrase* phrase) {
    BNFSentence* sentence = BNFAst_getSentence(BNFAst_getSubAstAt(ast, 0));
    if (sentence == terminalSymbolSentence)
        BNFCodeGenerator_handleTerminalSymbolSentence_(this, BNFAst_getSubAstAt(ast, 0), phrase);
    else if (sentence == identifierSentence)
        BNFCodeGenerator_handleIdentifierSentence_(this, BNFAst_getSubAstAt(ast, 0), phrase);
    else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "Symbol or Identifier expected");
}
static void BNFCodeGenerator_handleTermSentence_(BNFCodeGenerator* this, BNFAst* ast, BNFPhrase* phrase) {
    BNFSentence* firstSentence = BNFAst_getSentence(BNFAst_getSubAstAt(ast, 0));
    if (firstSentence == openParenSentence) { /* '(' alternative ')' */ /* TODO create new sentence, call handleAlternative */
        // BNFCodeGenerator_handleAlternativeSentence_(this, BNFAst_getSubAstAt(ast, 1));
    } else /* symbol */
        BNFCodeGenerator_handleSymbolSentence_(this, BNFAst_getSubAstAt(ast, 0), phrase);
}
/*
 *  phrase ::= term repetition_marker? ;
 */
static void BNFCodeGenerator_handlePhraseSentence_(BNFCodeGenerator* this, BNFAst* ast, BNFAlternative* alternative) {
    BNFPhraseRepeatSwitch repeatSwitch = BNFPhraseRepeat_once;
    BNFAst* termAst = BNFAst_getSubAstAt(ast, 0);
    if (BNFAst_getSubAstsSize(ast) > 1) /* determine repeat switch */
        repeatSwitch = getRepeatSwitchFromRepetitionMarkerSentence_(BNFAst_getSubAstAt(ast, 1));
        
    BNFPhrase* phrase = BNFPhrase__new(repeatSwitch, CGArray__new(BNFSentence, 1)); /* sentences could lead upwards! */
    BNFAlternative_addPhrase(alternative, phrase);
    CGTree(BNFAst)* astTree = NULL;
    CGArrayOfCGTreeOfBNFAstIterator* iter = BNFAst_getSubAstIterator(ast);
    while ((astTree = CGArrayIterator_fetch(CGTreeOfBNFAst, iter)) != NULL)
        if (BNFAst_getSentence(CGTree_getValue(BNFAst, astTree)) == termSentence)
            BNFCodeGenerator_handleTermSentence_(this, CGTree_getValue(BNFAst, astTree), phrase);
}
/*
 * alternative ::= phrase+ ;
 *
 * for each alternative in a rule, add a new alternative to the current rule
 */
static void BNFCodeGenerator_handleAlternativeSentence_(BNFCodeGenerator* this, BNFAst* ast) {
    BNFAlternative* alternative = BNFAlternative__new(CGArray__new(BNFPhrase, 1));
    BNFSentence_addAlternative(BNFCodeGenerator_getCurrentSentence_(this), alternative);

    CGTree(BNFAst)* astTree = NULL;
    CGArrayOfCGTreeOfBNFAstIterator* iter = BNFAst_getSubAstIterator(ast);
    while ((astTree = CGArrayIterator_fetch(CGTreeOfBNFAst, iter)) != NULL)
        if (BNFAst_getSentence(CGTree_getValue(BNFAst, astTree)) == phraseSentence)
            BNFCodeGenerator_handlePhraseSentence_(this, CGTree_getValue(BNFAst, astTree), alternative);
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
static void BNFCodeGenerator_createRuleSentence_(BNFCodeGenerator* this, BNFAst* ast) {
    BNFAst* identifierAst = BNFAst_getSubAstAt(ast, 0);
    BNFAst* alternativesAst = BNFAst_getSubAstAt(ast, 2);
    this->ruleTokenTypeCount = 0;
    this->ruleSentenceCount = 0;
    if (BNFAst_getSentence(identifierAst) == identifierSentence) {
        /* create new sentence */
        CGString* name = NULL;
        asprintf(&name, "%s", BNFToken_getText(BNFAst_getToken(identifierAst)));
        BNFSentence* sentence = BNFSentence__newNonTerminalSymbol(name, this->nonTerminalTokenType);

        /* add new sentence to start rule */
        CGArray(BNFSentence)* parts = CGArray__newFromInitializerList(BNFSentence, sentence, NULL);
        BNFPhrase* phrase = BNFPhrase__new(BNFPhraseRepeat_once, parts);
        CGArray(BNFPhrase)* phrases = CGArray__newFromInitializerList(BNFPhrase, phrase, NULL);
        BNFAlternative* alternative = BNFAlternative__new(phrases);
        BNFSentence* currentSentence = BNFCodeGenerator_getCurrentSentence_(this);
        BNFSentence_addAlternative(currentSentence, alternative);

        /* lower handlers will work on our new sentence */
        CGArray_add(BNFSentence, this->ruleSentences, sentence);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "Identifier expected");
}
static void BNFCodeGenerator_handleRuleSentence_(BNFCodeGenerator* this, BNFAst* ast) {
    BNFAst* identifierAst = BNFAst_getSubAstAt(ast, 0);
    BNFAst* alternativesAst = BNFAst_getSubAstAt(ast, 2);
    if (BNFAst_getSentence(identifierAst) == identifierSentence) {
        CGString* name = NULL;
        asprintf(&name, "%s", BNFToken_getText(BNFAst_getToken(identifierAst)));
        this->ruleName = name;
        this->ruleSentenceCount = 0;
        this->ruleTokenTypeCount = 0;
        printf("--> in rule %s\n", this->ruleName);
        printf("--> handle rule %s\n", name);
        BNFSentence* namedSentence = BNFSentence__newNonTerminalSymbol(name, this->nonTerminalTokenType);
        BNFSentence* sentence = CGArray_find(BNFSentence, this->ruleSentences, namedSentence, BNFCodeGenerator__compareSentenceNames_);
        /* TODO: sideeffects??? BNFSentence_delete(namedSentence); */
        if (sentence != NULL) {
            /* lower handlers will work on our new sentence */
            CGArray_push(BNFSentence, this->currentSentenceStack, sentence); 
            BNFCodeGenerator_handleAlternativesSentence_(this, alternativesAst);
        } else {
            CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "Rule '%s' not found.", name);
            return;
        }

        CGArray_pop(BNFSentence, this->currentSentenceStack);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "Identifier expected");
}
static void BNFCodeGenerator_handleStartSentence_(BNFCodeGenerator* this, BNFAst* ast) {
    this->startSentence = BNFSentence__newNonTerminalSymbol("start", this->nonTerminalTokenType);
    CGArray_push(BNFSentence, this->currentSentenceStack, this->startSentence); 
    
    CGTree(BNFAst)* astTree = NULL;
    CGArrayOfCGTreeOfBNFAstIterator* iter = BNFAst_getSubAstIterator(ast);

    while ((astTree = CGArrayIterator_fetch(CGTreeOfBNFAst, iter)) != NULL)
        BNFCodeGenerator_createRuleSentence_(this, CGTree_getValue(BNFAst, astTree));
    CGArrayIterator_reset(CGTreeOfBNFAst, iter);
    while ((astTree = CGArrayIterator_fetch(CGTreeOfBNFAst, iter)) != NULL)
        BNFCodeGenerator_handleRuleSentence_(this, CGTree_getValue(BNFAst, astTree));
    CGArrayOfCGTreeOfBNFAstIterator_delete(iter);
}

CGString* BNFCodeGenerator_createCode(BNFCodeGenerator* this) {
    BNFScannerRuleset__getInstance();
    if (BNFAst_getSentence(this->ast) == startSentence)
        BNFCodeGenerator_handleStartSentence_(this, this->ast);
    else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_CCUnexpectedSentence, "No start sentence found.");
    printf("----> resulting ruleset: \n");
    CGArray(BNFSentence)* seenSentences = CGArray__new(BNFSentence, 16);
    BNFSentence_print(this->startSentence, 0, seenSentences);
    printf("----> dumping code: \n");
    BNFSentence* sentence;
    CGString* text;
    CGString* resultText = CGString__new("");

    text = BNFTokenType_createCDeclaration(this->nonTerminalTokenType);
    resultText = CGString_append_I(resultText, text);

    CGArrayIterator(BNFTokenType)* tokenTypeIter = CGArrayIterator__new(BNFTokenType, this->tokenTypes);
    BNFTokenType *tt = NULL;
    while ((tt = CGArrayIterator_fetch(BNFTokenType, tokenTypeIter)) != NULL) {
        text = BNFTokenType_createCDeclaration(tt);
        resultText = CGString_append_I(resultText, text);
    }
    CGArrayIterator(BNFScannerNode)* scannerNodeIter = CGArrayIterator__new(BNFScannerNode, this->scannerNodes);
    BNFScannerNode* node = NULL;
    unsigned int i = 0;
    while ((node = CGArrayIterator_fetch(BNFScannerNode, scannerNodeIter)) != NULL) {
        text = BNFScannerNode_createCDeclaration(node, i);
        resultText = CGString_append_I(resultText, text);
        i++;
    }
    printf("BNFScannerRule* startRule = NULL;\n");
    CGArrayIterator(BNFSentence)* terminalSentenceIter = CGArrayIterator__new(BNFSentence, this->terminalSentences);
    while ((sentence = CGArrayIterator_fetch(BNFSentence, terminalSentenceIter)) != NULL) {
        text = BNFSentence_createCDeclaration(sentence);
        resultText = CGString_append_I(resultText, text);
    }
    text = BNFSentence_createCDeclaration(this->startSentence);
    resultText = CGString_append_I(resultText, text);
    CGArrayIterator(BNFSentence)* ruleSentenceIter = ruleSentenceIter = CGArrayIterator__new(BNFSentence, this->ruleSentences);
    while ((sentence = CGArrayIterator_fetch(BNFSentence, ruleSentenceIter)) != NULL) {
        text = BNFSentence_createCDeclaration(sentence);
        resultText = CGString_append_I(resultText, text);
    }
    CGArrayIterator_reset(BNFSentence, ruleSentenceIter);
    while ((sentence = CGArrayIterator_fetch(BNFSentence, ruleSentenceIter)) != NULL) {
        text = BNFSentence_createCAlternativesDeclarations(sentence);
        resultText = CGString_append_I(resultText, text);
    }
    CGArrayIterator_reset(BNFSentence, ruleSentenceIter);
    while ((sentence = CGArrayIterator_fetch(BNFSentence, ruleSentenceIter)) != NULL) {
        text = BNFSentence_createCAlternativesPhrasesDeclarations(sentence);
        resultText = CGString_append_I(resultText, text);
    }

    resultText = CGString_append_I(resultText, "BNFScannerRule* createScannerRuleset() {\n");
    text = BNFTokenType_createCConstructor(this->nonTerminalTokenType);
    resultText = CGString_appendWithSprintf_I(resultText, "    %s", text);

    CGArrayIterator_reset(BNFTokenType, tokenTypeIter);
    while ((tt = CGArrayIterator_fetch(BNFTokenType, tokenTypeIter)) != NULL) {
        text = BNFTokenType_createCConstructor(tt);
        resultText = CGString_appendWithSprintf_I(resultText, "    %s", text);
    }
    CGArrayIterator_reset(BNFScannerNode, scannerNodeIter);
    i = 0;
    while ((node = CGArrayIterator_fetch(BNFScannerNode, scannerNodeIter)) != NULL) {
        text = BNFScannerNode_createCConstructor(node, i);
        resultText = CGString_appendWithSprintf_I(resultText, "    %s", text);
        i++;
    }
    resultText = CGString_append_I(resultText, "    BNFScannerRule* scannerRuleStart = BNFScannerRule__new(CGString__new(\"start\"), NULL);\n");
    resultText = CGString_append_I(resultText, "    BNFScannerRule* scannerRuleNoise = BNFScannerRule__new(CGString__new(\"noise\"), NULL);\n");
    resultText = CGString_append_I(resultText, "    BNFScannerRule_setNodes(scannerRuleStart, CGArray__newFromInitializerList(BNFScannerNode, ");
    CGArrayIterator_reset(BNFScannerNode, scannerNodeIter);
    i = 0;
    while ((node = CGArrayIterator_fetch(BNFScannerNode, scannerNodeIter)) != NULL) {
        if (i>0)
            resultText = CGString_append_I(resultText, ", ");
        resultText = CGString_appendWithSprintf_I(resultText, "scannerNode%u", i);
        i++;
    }
    resultText = CGString_append_I(resultText, "));\n");
    resultText = CGString_append_I(resultText, "    BNFScannerRule_setNodes(scannerRuleNoise, CGArray__newFromInitializerList(BNFScannerNode, ");
    CGArrayIterator_reset(BNFScannerNode, scannerNodeIter);
    i = 0;
    while ((node = CGArrayIterator_fetch(BNFScannerNode, scannerNodeIter)) != NULL) {
        if (i>0)
            resultText = CGString_append_I(resultText, ", ");
        resultText = CGString_appendWithSprintf_I(resultText, "scannerNode%u", i);
        i++;
    }
    resultText = CGString_append_I(resultText, "));\n");
    resultText = CGString_append_I(resultText, "    return scannerRuleStart;\n}\n");

    resultText = CGString_append_I(resultText, "BNFSentence* createParserRuleset() {\n");
    text = BNFSentence_createCConstructor(this->startSentence);
    resultText = CGString_appendWithSprintf_I(resultText, "    %s", text);
    CGArrayIterator_reset(BNFSentence, terminalSentenceIter);
    while ((sentence = CGArrayIterator_fetch(BNFSentence, terminalSentenceIter)) != NULL) {
        text = BNFSentence_createCConstructor(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "    %s", text);
    }
    CGArrayIterator_reset(BNFSentence, ruleSentenceIter);
    while ((sentence = CGArrayIterator_fetch(BNFSentence, ruleSentenceIter)) != NULL) {
        text = BNFSentence_createCConstructor(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "    %s", text);
        text = BNFSentence_createCAlternativesConstructors(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "%s", text);
        text = BNFSentence_createCAlternativesPhrasesConstructors(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "%s", text);
    }
    CGArrayIterator_reset(BNFSentence, ruleSentenceIter);
    while ((sentence = CGArrayIterator_fetch(BNFSentence, ruleSentenceIter)) != NULL) {
        text = BNFSentence_createCAddAlternatives(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "%s", text);
        text = BNFSentence_createCAlternativesPhrasesAddParts(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "%s", text);
    }
    resultText = CGString_append_I(resultText, "    return startSentence;\n}\n");
    printf("%s", resultText);
    CGArrayIterator_delete(BNFSentence, terminalSentenceIter);
    CGArrayIterator_delete(BNFSentence, ruleSentenceIter);
    CGArrayIterator_delete(BNFScannerNode, scannerNodeIter);
    CGArrayIterator_delete(BNFTokenType, tokenTypeIter);
    printf("----> end dumping code\n");
    printf("----> result textg:\n%s", resultText);
    return "";
}
