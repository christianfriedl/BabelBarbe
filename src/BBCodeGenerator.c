#include"BBCodeGenerator.h"

DEFINE_ARRAY_FUNCS(CGString)

static BBSentence* BBCodeGenerator_getCurrentSentence_(BBCodeGenerator* this);
static int BBCodeGenerator__compareSentenceNames_(const BBSentence **first, const BBSentence **second);

BBCodeGenerator* BBCodeGenerator__new(BBAst* ast) {
    BBCodeGenerator* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->ast = ast;
        this->startSentence = NULL;
        this->currentSentenceStack = CGArray__new(BBSentence, 4);
        this->nonTerminalTokenType = BBTokenTypeFactory_createBBTokenType(BBTokenTypeFactory__getInstance(), CGString__new("nonTerminal"));
        this->ruleTokenTypeCount = 0;
        this->ruleSentenceCount = 0;
        this->ruleName = "start";
        this->ruleSentences = CGArray__new(BBSentence, 16);
        this->terminalSentences = CGArray__new(BBSentence, 16);
        this->tokenTypes = CGArray__new(BBTokenType, 16);
        this->scannerNodes = CGArray__new(BBScannerNode, 16);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, CGExceptionID_CannotAllocate, "unable to allocate BBCodeGenerator");
    return this;
}

void BBCodeGenerator_delete(BBCodeGenerator* this) {
    free(this);
    /* TODO ast?? */
}

BBTokenType* BBCodeGenerator_createTokenType(BBCodeGenerator* this) {
    CGString* name = NULL;
    if (CGArray_getSize(BBSentence, this->currentSentenceStack) > 0)
        name = CGString__newWithSprintf("%s%u", this->ruleName, this->ruleTokenTypeCount++);
    else
        name = "Unknown token type";

    BBTokenType* tt = BBTokenTypeFactory_createBBTokenType(BBTokenTypeFactory__getInstance(), name);
    CGArray_add(BBTokenType, this->tokenTypes, tt);
    return tt;
}

static int BBCodeGenerator__compareSentenceNames_(const BBSentence **first, const BBSentence **second) {
    BBSentence* unconstFirst = (BBSentence*) *first;
    BBSentence* unconstSecond = (BBSentence*) *second;
    return CGString__compare(BBSentence_getName(unconstFirst), BBSentence_getName(unconstSecond));
}

static BBSentence* BBCodeGenerator_getCurrentSentence_(BBCodeGenerator* this) {
    return CGArray_getValueAt(BBSentence, this->currentSentenceStack, CGArray_getSize(BBSentence, this->currentSentenceStack) - 1);
}
static int getRepeatSwitchFromRepetitionMarkerSentence_(BBAst* ast) {
    BBSentence* sentence = BBAst_getSentence(BBAst_getSubAstAt(ast, 0));
    if (sentence == repetitionMarkerZeroOrOnceSentence)
        return BBPhraseRepeat_zeroOrOnce;
    else if (sentence == repetitionMarkerZeroOrMoreSentence)
        return BBPhraseRepeat_zeroOrMore;
    else if (sentence == repetitionMarkerManySentence)
        return BBPhraseRepeat_many;
    else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_CCUnexpectedSentence, "Repetition marker expected");
    return 0;
}
/* end helper functions */

static void BBCodeGenerator_handleStringLiteralSentence_(BBCodeGenerator* this, BBAst* ast, BBPhrase* phrase) {
        CGString* name = NULL;
        asprintf(&name, "%sStringLiteral%u", this->ruleName, this->ruleSentenceCount++);
        BBTokenType* tokenType = BBCodeGenerator_createTokenType(this);
        BBSentence* sentence = BBSentence__newTerminalSymbol(name, tokenType);
        CGArray(BBSentence)* parts = CGArray__newFromInitializerList(BBSentence, sentence, NULL);
        BBPhrase_setParts(phrase, parts);
        CGArray_add(BBSentence, this->terminalSentences, sentence);
        BBScannerNode* node = BBScannerNode__new(BBScannerNodeType_string, BBToken_getText(BBAst_getToken(ast)), NULL, tokenType, false, NULL);
        CGArray_add(BBScannerNode, this->scannerNodes, node);
}
static void BBCodeGenerator_handleRegexLiteralSentence_(BBCodeGenerator* this, BBAst* ast, BBPhrase* phrase) {
        CGString* name = NULL;
        asprintf(&name, "%sRegexLiteral%u", this->ruleName, this->ruleSentenceCount++);
        BBTokenType* tokenType = BBCodeGenerator_createTokenType(this);
        BBSentence* sentence = BBSentence__newTerminalSymbol(name, tokenType);
        CGArray(BBSentence)* parts = CGArray__newFromInitializerList(BBSentence, sentence, NULL);
        BBPhrase_setParts(phrase, parts);
        CGArray_add(BBSentence, this->terminalSentences, sentence);
        BBScannerNode* node = BBScannerNode__new(BBScannerNodeType_regex, BBToken_getText(BBAst_getToken(ast)), NULL, tokenType, false, NULL);
        CGArray_add(BBScannerNode, this->scannerNodes, node);
}
static void BBCodeGenerator_handleTerminalSymbolSentence_(BBCodeGenerator* this, BBAst* ast, BBPhrase* phrase) {
    BBSentence* sentence = BBAst_getSentence(BBAst_getSubAstAt(ast, 0));
    if (sentence == stringLiteralSentence)
        BBCodeGenerator_handleStringLiteralSentence_(this, BBAst_getSubAstAt(ast, 0), phrase);
    else if (sentence == regexLiteralSentence)
        BBCodeGenerator_handleRegexLiteralSentence_(this, BBAst_getSubAstAt(ast, 0), phrase);
    else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_CCUnexpectedSentence, "String or Regex literal expected");
}
static void BBCodeGenerator_handleIdentifierSentence_(BBCodeGenerator* this, BBAst* ast, BBPhrase* phrase) {
        CGString* name = NULL;
        asprintf(&name, "%s", BBToken_getText(BBAst_getToken(ast)));
        BBSentence* namedSentence = BBSentence__newNonTerminalSymbol(name, this->nonTerminalTokenType);
        BBSentence* sentence = CGArray_find(BBSentence, this->ruleSentences, namedSentence, BBCodeGenerator__compareSentenceNames_);
        if (sentence == NULL) {
            CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_CCUnexpectedSentence, "Rule '%s' not found", name);
            return;
        }
        CGArray(BBSentence)* parts = CGArray__newFromInitializerList(BBSentence, sentence, NULL);
        BBPhrase_setParts(phrase, parts);
}
static void BBCodeGenerator_handleSymbolSentence_(BBCodeGenerator* this, BBAst* ast, BBPhrase* phrase) {
    BBSentence* sentence = BBAst_getSentence(BBAst_getSubAstAt(ast, 0));
    if (sentence == terminalSymbolSentence)
        BBCodeGenerator_handleTerminalSymbolSentence_(this, BBAst_getSubAstAt(ast, 0), phrase);
    else if (sentence == identifierSentence)
        BBCodeGenerator_handleIdentifierSentence_(this, BBAst_getSubAstAt(ast, 0), phrase);
    else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_CCUnexpectedSentence, "Symbol or Identifier expected");
}
static void BBCodeGenerator_handleTermSentence_(BBCodeGenerator* this, BBAst* ast, BBPhrase* phrase) {
    BBSentence* firstSentence = BBAst_getSentence(BBAst_getSubAstAt(ast, 0));
    if (firstSentence == openParenSentence) { /* '(' alternative ')' */ /* TODO create new sentence, call handleAlternative */
        // BBCodeGenerator_handleAlternativeSentence_(this, BBAst_getSubAstAt(ast, 1));
    } else /* symbol */
        BBCodeGenerator_handleSymbolSentence_(this, BBAst_getSubAstAt(ast, 0), phrase);
}
/*
 *  phrase ::= term repetition_marker? ;
 */
static void BBCodeGenerator_handlePhraseSentence_(BBCodeGenerator* this, BBAst* ast, BBAlternative* alternative) {
    BBPhraseRepeatSwitch repeatSwitch = BBPhraseRepeat_once;
    BBAst* termAst = BBAst_getSubAstAt(ast, 0);
    if (BBAst_getSubAstsSize(ast) > 1) /* determine repeat switch */
        repeatSwitch = getRepeatSwitchFromRepetitionMarkerSentence_(BBAst_getSubAstAt(ast, 1));
        
    BBPhrase* phrase = BBPhrase__new(repeatSwitch, CGArray__new(BBSentence, 1)); /* sentences could lead upwards! */
    BBAlternative_addPhrase(alternative, phrase);
    CGTree(BBAst)* astTree = NULL;
    CGArrayOfCGTreeOfBBAstIterator* iter = BBAst_getSubAstIterator(ast);
    while ((astTree = CGArrayIterator_fetch(CGTreeOfBBAst, iter)) != NULL)
        if (BBAst_getSentence(CGTree_getValue(BBAst, astTree)) == termSentence)
            BBCodeGenerator_handleTermSentence_(this, CGTree_getValue(BBAst, astTree), phrase);
}
/*
 * alternative ::= phrase+ ;
 *
 * for each alternative in a rule, add a new alternative to the current rule
 */
static void BBCodeGenerator_handleAlternativeSentence_(BBCodeGenerator* this, BBAst* ast) {
    BBAlternative* alternative = BBAlternative__new(CGArray__new(BBPhrase, 1));
    BBSentence_addAlternative(BBCodeGenerator_getCurrentSentence_(this), alternative);

    CGTree(BBAst)* astTree = NULL;
    CGArrayOfCGTreeOfBBAstIterator* iter = BBAst_getSubAstIterator(ast);
    while ((astTree = CGArrayIterator_fetch(CGTreeOfBBAst, iter)) != NULL)
        if (BBAst_getSentence(CGTree_getValue(BBAst, astTree)) == phraseSentence)
            BBCodeGenerator_handlePhraseSentence_(this, CGTree_getValue(BBAst, astTree), alternative);
}
static void BBCodeGenerator_handleAlternativesSentence_(BBCodeGenerator* this, BBAst* ast) {
    CGTree(BBAst)* astTree = NULL;
    CGArrayOfCGTreeOfBBAstIterator* iter = BBAst_getSubAstIterator(ast);

    // we don't have to amend the current sentence because it is a nonterminal and already has an alternatives array

    while ((astTree = CGArrayIterator_fetch(CGTreeOfBBAst, iter)) != NULL)
        if (BBAst_getSentence(CGTree_getValue(BBAst, astTree)) == alternativeSentence) /* ignore the '|' sign - we might want the parser to do that... */
            BBCodeGenerator_handleAlternativeSentence_(this, CGTree_getValue(BBAst, astTree));
    CGArrayOfCGTreeOfBBAstIterator_delete(iter);
}
static void BBCodeGenerator_createRuleSentence_(BBCodeGenerator* this, BBAst* ast) {
    BBAst* identifierAst = BBAst_getSubAstAt(ast, 0);
    BBAst* alternativesAst = BBAst_getSubAstAt(ast, 2);
    this->ruleTokenTypeCount = 0;
    this->ruleSentenceCount = 0;
    if (BBAst_getSentence(identifierAst) == identifierSentence) {
        /* create new sentence */
        CGString* name = NULL;
        asprintf(&name, "%s", BBToken_getText(BBAst_getToken(identifierAst)));
        BBSentence* sentence = BBSentence__newNonTerminalSymbol(name, this->nonTerminalTokenType);

        /* add new sentence to start rule */
        CGArray(BBSentence)* parts = CGArray__newFromInitializerList(BBSentence, sentence, NULL);
        BBPhrase* phrase = BBPhrase__new(BBPhraseRepeat_once, parts);
        CGArray(BBPhrase)* phrases = CGArray__newFromInitializerList(BBPhrase, phrase, NULL);
        BBAlternative* alternative = BBAlternative__new(phrases);
        BBSentence* currentSentence = BBCodeGenerator_getCurrentSentence_(this);
        BBSentence_addAlternative(currentSentence, alternative);

        /* lower handlers will work on our new sentence */
        CGArray_add(BBSentence, this->ruleSentences, sentence);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_CCUnexpectedSentence, "Identifier expected");
}
static void BBCodeGenerator_handleRuleSentence_(BBCodeGenerator* this, BBAst* ast) {
    BBAst* identifierAst = BBAst_getSubAstAt(ast, 0);
    BBAst* alternativesAst = BBAst_getSubAstAt(ast, 2);
    if (BBAst_getSentence(identifierAst) == identifierSentence) {
        CGString* name = NULL;
        asprintf(&name, "%s", BBToken_getText(BBAst_getToken(identifierAst)));
        this->ruleName = name;
        this->ruleSentenceCount = 0;
        this->ruleTokenTypeCount = 0;
        BBSentence* namedSentence = BBSentence__newNonTerminalSymbol(name, this->nonTerminalTokenType);
        BBSentence* sentence = CGArray_find(BBSentence, this->ruleSentences, namedSentence, BBCodeGenerator__compareSentenceNames_);
        /* TODO: sideeffects??? BBSentence_delete(namedSentence); */
        if (sentence != NULL) {
            /* lower handlers will work on our new sentence */
            CGArray_push(BBSentence, this->currentSentenceStack, sentence); 
            BBCodeGenerator_handleAlternativesSentence_(this, alternativesAst);
        } else {
            CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_CCUnexpectedSentence, "Rule '%s' not found.", name);
            return;
        }

        CGArray_pop(BBSentence, this->currentSentenceStack);
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_CCUnexpectedSentence, "Identifier expected");
}
static void BBCodeGenerator_handleStartSentence_(BBCodeGenerator* this, BBAst* ast) {
    this->startSentence = BBSentence__newNonTerminalSymbol("start", this->nonTerminalTokenType);
    CGArray_push(BBSentence, this->currentSentenceStack, this->startSentence); 
    
    CGTree(BBAst)* astTree = NULL;
    CGArrayOfCGTreeOfBBAstIterator* iter = BBAst_getSubAstIterator(ast);

    while ((astTree = CGArrayIterator_fetch(CGTreeOfBBAst, iter)) != NULL)
        BBCodeGenerator_createRuleSentence_(this, CGTree_getValue(BBAst, astTree));
    CGArrayIterator_reset(CGTreeOfBBAst, iter);
    while ((astTree = CGArrayIterator_fetch(CGTreeOfBBAst, iter)) != NULL)
        BBCodeGenerator_handleRuleSentence_(this, CGTree_getValue(BBAst, astTree));
    CGArrayOfCGTreeOfBBAstIterator_delete(iter);
}

CGString* BBCodeGenerator_createCode(BBCodeGenerator* this) {
    BBScannerRuleset__getInstance();
    if (BBAst_getSentence(this->ast) == startSentence)
        BBCodeGenerator_handleStartSentence_(this, this->ast);
    else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_CCUnexpectedSentence, "No start sentence found.");
    BBSentence* sentence;
    CGString* text;
    CGString* resultText = CGString__new("#include\"/opt/local/include/pcre.h\"\n"
                                "#include<bbarbe/BBTokenType.h>\n"
                                "#include<bbarbe/BBToken.h>\n"
                                "#include<bbarbe/BBScannerRule.h>\n"
                                "#include<bbarbe/BB_RDParser.h>\n\n");


    text = BBTokenType_createCDeclaration(this->nonTerminalTokenType);
    resultText = CGString_append_I(resultText, text);

    CGArrayIterator(BBTokenType)* tokenTypeIter = CGArrayIterator__new(BBTokenType, this->tokenTypes);
    BBTokenType *tt = NULL;
    while ((tt = CGArrayIterator_fetch(BBTokenType, tokenTypeIter)) != NULL) {
        text = BBTokenType_createCDeclaration(tt);
        resultText = CGString_append_I(resultText, text);
    }
    CGArrayIterator(BBScannerNode)* scannerNodeIter = CGArrayIterator__new(BBScannerNode, this->scannerNodes);
    BBScannerNode* node = NULL;
    unsigned int i = 0;
    while ((node = CGArrayIterator_fetch(BBScannerNode, scannerNodeIter)) != NULL) {
        text = BBScannerNode_createCDeclaration(node, i);
        resultText = CGString_append_I(resultText, text);
        i++;
    }
    resultText = CGString_append_I(resultText, "BBScannerRule* scannerRuleStart = NULL;\n");
    resultText = CGString_append_I(resultText, "BBScannerRule* scannerRuleNoise = NULL;\n");
    CGArrayIterator(BBSentence)* terminalSentenceIter = CGArrayIterator__new(BBSentence, this->terminalSentences);
    while ((sentence = CGArrayIterator_fetch(BBSentence, terminalSentenceIter)) != NULL) {
        text = BBSentence_createCDeclaration(sentence);
        resultText = CGString_append_I(resultText, text);
    }
    CGArrayIterator(BBSentence)* ruleSentenceIter = ruleSentenceIter = CGArrayIterator__new(BBSentence, this->ruleSentences);
    while ((sentence = CGArrayIterator_fetch(BBSentence, ruleSentenceIter)) != NULL) {
        text = BBSentence_createCDeclaration(sentence);
        resultText = CGString_append_I(resultText, text);
    }
    CGArrayIterator_reset(BBSentence, ruleSentenceIter);
    while ((sentence = CGArrayIterator_fetch(BBSentence, ruleSentenceIter)) != NULL) {
        text = BBSentence_createCAlternativesDeclarations(sentence);
        resultText = CGString_append_I(resultText, text);
    }
    CGArrayIterator_reset(BBSentence, ruleSentenceIter);
    while ((sentence = CGArrayIterator_fetch(BBSentence, ruleSentenceIter)) != NULL) {
        text = BBSentence_createCAlternativesPhrasesDeclarations(sentence);
        resultText = CGString_append_I(resultText, text);
    }

    resultText = CGString_append_I(resultText, "\n");
    resultText = CGString_append_I(resultText, "BBScannerRule* createScannerRuleset() {\n");
    text = BBTokenType_createCConstructor(this->nonTerminalTokenType);
    resultText = CGString_appendWithSprintf_I(resultText, "    %s", text);

    CGArrayIterator_reset(BBTokenType, tokenTypeIter);
    while ((tt = CGArrayIterator_fetch(BBTokenType, tokenTypeIter)) != NULL) {
        text = BBTokenType_createCConstructor(tt);
        resultText = CGString_appendWithSprintf_I(resultText, "    %s", text);
    }
    resultText = CGString_append_I(resultText, "    scannerRuleStart = BBScannerRule__new(CGString__new(\"start\"), NULL);\n");
    resultText = CGString_append_I(resultText, "    scannerRuleNoise = BBScannerRule__new(CGString__new(\"noise\"), NULL);\n");
    CGArrayIterator_reset(BBScannerNode, scannerNodeIter);
    i = 0;
    while ((node = CGArrayIterator_fetch(BBScannerNode, scannerNodeIter)) != NULL) {
        text = BBScannerNode_createCConstructor(node, i);
        resultText = CGString_appendWithSprintf_I(resultText, "    %s", text);
        i++;
    }
    resultText = CGString_append_I(resultText, "    BBScannerRule_setNodes(scannerRuleStart, CGArray__newFromInitializerList(BBScannerNode, ");
    CGArrayIterator_reset(BBScannerNode, scannerNodeIter);
    i = 0;
    while ((node = CGArrayIterator_fetch(BBScannerNode, scannerNodeIter)) != NULL) {
        if (i>0)
            resultText = CGString_append_I(resultText, ", ");
        resultText = CGString_appendWithSprintf_I(resultText, "scannerNode%u", i);
        i++;
    }
    resultText = CGString_append_I(resultText, ", NULL));\n");
    resultText = CGString_append_I(resultText, "    BBScannerRule_setNodes(scannerRuleNoise, CGArray__newFromInitializerList(BBScannerNode, ");
    CGArrayIterator_reset(BBScannerNode, scannerNodeIter);
    i = 0;
    while ((node = CGArrayIterator_fetch(BBScannerNode, scannerNodeIter)) != NULL) {
        if (i>0)
            resultText = CGString_append_I(resultText, ", ");
        resultText = CGString_appendWithSprintf_I(resultText, "scannerNode%u", i);
        i++;
    }
    resultText = CGString_append_I(resultText, ", NULL));\n");
    resultText = CGString_append_I(resultText, "    return scannerRuleStart;\n}\n");

    resultText = CGString_append_I(resultText, "\n");
    resultText = CGString_append_I(resultText, "BBSentence* createParserRuleset() {\n");
    CGArrayIterator_reset(BBSentence, terminalSentenceIter);
    while ((sentence = CGArrayIterator_fetch(BBSentence, terminalSentenceIter)) != NULL) {
        text = BBSentence_createCConstructor(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "    %s", text);
    }
    CGArrayIterator_reset(BBSentence, ruleSentenceIter);
    while ((sentence = CGArrayIterator_fetch(BBSentence, ruleSentenceIter)) != NULL) {
        text = BBSentence_createCConstructor(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "    %s", text);
        text = BBSentence_createCAlternativesConstructors(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "%s", text);
        text = BBSentence_createCAlternativesPhrasesConstructors(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "%s", text);
        text = BBSentence_createCAlternativesAddPhrases(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "%s", text);
    }
    CGArrayIterator_reset(BBSentence, ruleSentenceIter);
    while ((sentence = CGArrayIterator_fetch(BBSentence, ruleSentenceIter)) != NULL) {
        text = BBSentence_createCAddAlternatives(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "%s", text);
        text = BBSentence_createCAlternativesPhrasesAddParts(sentence);
        resultText = CGString_appendWithSprintf_I(resultText, "%s", text);
    }
    resultText = CGString_appendWithSprintf_I(resultText, "    return %sSentence;\n}\n", BBSentence_getName(CGArray_getValueAt(BBSentence, this->ruleSentences, 0)));
    CGArrayIterator_delete(BBSentence, terminalSentenceIter);
    CGArrayIterator_delete(BBSentence, ruleSentenceIter);
    CGArrayIterator_delete(BBScannerNode, scannerNodeIter);
    CGArrayIterator_delete(BBTokenType, tokenTypeIter);
    return resultText;
}
