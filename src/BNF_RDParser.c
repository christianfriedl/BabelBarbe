#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"BNF_RDParser.h"

DEFINE_ARRAY(BNFAst);
DEFINE_ARRAY(BNFSentence);
DEFINE_ARRAY(BNFPhrase);
DEFINE_ARRAY(BNFAlternative);
DEFINE_ARRAY(BNFTokenType);
DEFINE_ARRAY_ITERATOR(BNFToken);
DEFINE_ARRAY_ITERATOR(BNFTokenType);
DEFINE_ARRAY_ITERATOR(BNFAlternative);
DEFINE_ARRAY_ITERATOR(BNFSentence);
DEFINE_ARRAY_ITERATOR(BNFPhrase);

BNFAst* BNFSentence_parse(BNFSentence* this, CGArrayIterator(BNFToken)* tokenIterator);

CGString* BNFPhraseRepeatSwitch_toString(BNFPhraseRepeatSwitch this) {
    switch(this) {
        case BNFPhraseRepeat_zeroOrOnce:
            return CGString__new("BNFPhraseRepeat_zeroOrOnce");
        case BNFPhraseRepeat_zeroOrMore:
            return CGString__new("BNFPhraseRepeat_zeroOrMore");
        case BNFPhraseRepeat_once:
            return CGString__new("BNFPhraseRepeat_once");
        case BNFPhraseRepeat_many:
            return CGString__new("BNFPhraseRepeat_many");
        default:
            CGAppState_THROW(CGAppState__getInstance(), CGExceptionID_GeneralNonfatalException, Severity_warning, "no such BNFPhraseRepeatSwitch");
            return NULL;
    }
}


BNFPhrase* BNFPhrase__new(BNFPhraseRepeatSwitch repeatSwitch, CGArray(BNFSentence)* parts) {
    BNFPhrase* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->repeatSwitch = repeatSwitch;
        this->parts = parts;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}

BNFPhrase* BNFPhrase_clone(BNFPhrase* this) {
    return BNFPhrase__new(this->repeatSwitch, this->parts);
}

    
void BNFPhrase_delete(BNFPhrase* this) {
    /* TODO: ooops, with recursion this probably will lead to problems... solution: object-store? coughcough refcounting cough? */
    /* CGArray_deleteValues(BNFSentence, this->parts); */
    CGAppState_THROW(CGAppState__getInstance(), Severity_warning, CGExceptionID_GeneralNonfatalException, "TODO: sentences are currently not deleted - create object-store for them");
    CGArray_delete(BNFSentence, this->parts);
    free(this);
}

void BNFPhrase_setParts(BNFPhrase* this, CGArray(BNFSentence)* parts) {
    this->parts = parts;
}

void BNFPhrase_addPart(BNFPhrase* this, BNFSentence* part) {
    CGArray_add(BNFSentence, this->parts, part);
}

CGArray(BNFAst)* BNFPhrase_parse(BNFPhrase* this, CGArrayIterator(BNFToken)* tokenIterator) {
    bool isParsed = false;
    CGArrayIterator(BNFSentence)* partsIterator = CGArrayIterator__new(BNFSentence, this->parts);
    BNFSentence* sentence = NULL;
    CGArray(BNFAst)* asts = CGArray__new(BNFAst, 1);
    BNFAst* ast = NULL;
    if (this->repeatSwitch == BNFPhraseRepeat_zeroOrMore || this->repeatSwitch == BNFPhraseRepeat_zeroOrOnce)
        isParsed = true;
    bool partIsParsed;
    do {
        partIsParsed = true;
        while ((sentence = CGArrayIterator_fetch(BNFSentence, partsIterator)) != NULL) {
            //printf("phrase found sentence\n");
            if ((ast = BNFSentence_parse(sentence, tokenIterator)) != NULL) {
                //printf("phrase received ast, partIsParsed is %i\n", partIsParsed);
                CGArray_add(BNFAst, asts, ast);
            } else {
                partIsParsed = false;
                break;
            }
        }
        CGArrayIterator_reset(BNFSentence, partsIterator);
        if (this->repeatSwitch == BNFPhraseRepeat_once) {
            isParsed = partIsParsed;
            break;
        } else if (this->repeatSwitch == BNFPhraseRepeat_zeroOrOnce) {
            break;
        } else if (this->repeatSwitch == BNFPhraseRepeat_many)
            isParsed |= partIsParsed;
    } while (partIsParsed);
    //printf("phrase claims parsed is %i\n", isParsed);
    if (isParsed)
        return asts;
    else
        return NULL;
}
void BNFPhrase_print(BNFPhrase* this, unsigned int indentationLevel, CGArray(BNFSentence)* seenSentences) {
    CGString* indentation = CGString__newFromLengthAndPreset(indentationLevel * BNF_INDENTATION_SIZE, ' ');
    CGString* rsString = BNFPhraseRepeatSwitch_toString(this->repeatSwitch);
    #ifdef DEBUG
    printf("%sBNFPhrase (repeatSwitch %s)\n", indentation, rsString);
    #endif
    CGArrayIterator(BNFSentence)* iter = CGArrayIterator__new(BNFSentence, this->parts);
    BNFSentence* part = NULL;
    while ((part = CGArrayIterator_fetch(BNFSentence, iter)) != NULL)
        BNFSentence_print(part, indentationLevel + 1, seenSentences);
    CGArrayIterator_delete(BNFSentence, iter);
    CGString_delete(rsString);
    CGString_delete(indentation);
}

BNFAlternative* BNFAlternative__new(CGArray(BNFPhrase)* phrases) {
    BNFAlternative* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->phrases = phrases;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}
void BNFAlternative_addPhrase(BNFAlternative* this, BNFPhrase* phrase) {
    CGArray_add(BNFPhrase, this->phrases, phrase);
}
BNFAlternative* BNFAlternative_clone(BNFAlternative* this) {
    return BNFAlternative__new(this->phrases);
}
void BNFAlternative_delete(BNFAlternative* this) {
    CGArray_deleteValues(BNFPhrase, this->phrases);
    CGArray_delete(BNFPhrase, this->phrases);
    free(this);
}
CGArray(BNFAst)* BNFAlternative_parse(BNFAlternative* this, CGArrayIterator(BNFToken)* tokenIterator) {
    CGArray(BNFAst)* asts = NULL;
    CGArray(BNFAst)* phraseAsts = NULL;
    CGArrayIterator(BNFPhrase)* iter = CGArrayIterator__new(BNFPhrase, this->phrases);
    BNFPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BNFPhrase, iter)) != NULL) {
        if ((phraseAsts = BNFPhrase_parse(phrase, tokenIterator)) != NULL) {
            if (asts == NULL)
                asts = CGArray__new(BNFAst, 3);
            CGArray_append(BNFAst, asts, phraseAsts);
        } else
            break;
    }
    #ifdef DEBUG
    if (asts != NULL) {
        printf("alternative - received asts from phrases:\n");
        printf("%u\n", CGArray_getSize(BNFAst, asts));
    }
    #endif
    return asts;
}
void BNFAlternative_print(BNFAlternative* this, unsigned int indentationLevel, CGArray(BNFSentence)* seenSentences) {
    CGString* indentation = CGString__newFromLengthAndPreset(indentationLevel * BNF_INDENTATION_SIZE, ' '); 
    printf("%sBNFAlternative (%u BNFPhrases)\n", indentation, CGArray_getSize(BNFPhrase, this->phrases));
    CGArrayIterator(BNFPhrase)* iter = CGArrayIterator__new(BNFPhrase, this->phrases);
    BNFPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BNFPhrase, iter)) != NULL)
        BNFPhrase_print(phrase, indentationLevel + 1, seenSentences);
    CGArrayIterator_delete(BNFPhrase, iter);
    CGString_delete(indentation);
}


/* if alternatives is NULL, then this is a terminal symbol! */
BNFSentence* BNFSentence__new(CGString* name, BNFTokenType* tokenType, CGArray(BNFAlternative)* alternatives) {
    BNFSentence* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->name = name;
        this->tokenType = tokenType;
        this->alternatives = alternatives;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}
BNFSentence* BNFSentence__newTerminalSymbol(CGString* name, BNFTokenType* tokenType) {
    return BNFSentence__new(name, tokenType, NULL);
}
BNFSentence* BNFSentence__newNonTerminalSymbol(CGString* name, BNFTokenType* tokenType) {
    return BNFSentence__new(name, tokenType, CGArray__new(BNFAlternative, 1));
}
BNFSentence* BNFSentence__newFromBNFAst(BNFAst* ast, BNFTokenType* tokenType) {
    char *sentenceName = NULL, *tokenText = NULL;
    char *name = NULL;

    sentenceName = BNFSentence_getName(BNFAst_getSentence(ast));
    asprintf(&name, "%sSentence", tokenText); 
    /* token type is, of course, NOT one of "our" BNFParser token types, but a newly generated one */
    return BNFSentence__new(name, tokenType, NULL);
}
BNFSentence* BNFSentence_clone(BNFSentence* this) {
    return BNFSentence__new(this->name, this->tokenType, this->alternatives);
}
CGString* BNFSentence_getName(BNFSentence* this) {
    return this->name;
}
BNFTokenType* BNFSentence_getTokenType(BNFSentence* this) {
    return this->tokenType;
}
void BNFSentence_delete(BNFSentence* this) {
    CGString_delete(this->name);
    if (this->alternatives != NULL) {
        CGArray_deleteValues(BNFAlternative, this->alternatives);
        CGArray_delete(BNFAlternative, this->alternatives);
    }
    free(this);
}

BNFAst* BNFSentence_parse(BNFSentence* this, CGArrayIterator(BNFToken)* tokenIterator) {
    if (this->alternatives == NULL) { /* this is a terminal symbol */
        BNFToken* token = CGArrayIterator_fetch(BNFToken, tokenIterator);
        #ifdef DEBUG
        printf("terminal symbol sentence for token %s %s\n", token != NULL ? BNFToken_getTypeName(token): "(NULL)" , token != NULL ? BNFToken_getText(token) : "(NULL)");
        #endif
        if (token == NULL) {
            return NULL;
        } else if (BNFTokenType_isEqual(BNFToken_getType(token), this->tokenType))
            return BNFAst__new(NULL, token, this);
        else {
            CGArrayIterator_unFetch(BNFToken, tokenIterator);
            return NULL;
        }
    } else {
        BNFAst* ast = NULL;
        CGArrayIterator(BNFAlternative)* alternativesIterator = CGArrayIterator__new(BNFAlternative, this->alternatives);
        BNFAlternative* alternative = NULL;
        while ((alternative = CGArrayIterator_fetch(BNFAlternative, alternativesIterator)) != NULL) {
            //printf("trying alternative @%ld\n", alternative);
            CGArray(BNFAst)* altAsts = BNFAlternative_parse(alternative, tokenIterator);
            if (altAsts != NULL) {
                ast = BNFAst__new(NULL, BNFToken__new(this->tokenType, CGString__new("")), this);
                //printf("created ast @%ld with tokenType %s\n", ast, BNFTokenType_toString(this->tokenType));
                BNFAst_setSubAsts(ast, altAsts);
                break;
            } else if (CGAppState_isExceptionRaisedWithID(CGAppState__getInstance(), BNFExceptionID_ScannerError))
                return NULL;
            /* Possible TODO: warn if there are multiple applicable alternatives */
        }
        return ast;
    }
}

int BNFSentence_compare(const BNFSentence** first, const BNFSentence** second) {
    //printf("BNFSentence_compare comparing %ld and %ld, returning %ld\n", *second, *first, (*second-*first));
    return (*second - *first);
}
void BNFSentence_setAlternatives(BNFSentence* this, CGArray(BNFAlternative)* alternatives) {
    this->alternatives = alternatives;
}

void BNFSentence_addAlternative(BNFSentence* this, BNFAlternative* alternative) {
    CGArray_add(BNFAlternative, this->alternatives, alternative);
}

void BNFSentence_print(BNFSentence* this, unsigned int indentationLevel, CGArray(BNFSentence)* seenSentences) {
    if (CGArray_find(BNFSentence, seenSentences, this, BNFSentence_compare) == NULL) {
        CGArray_add(BNFSentence, seenSentences, this);
        CGString* indentation = CGString__newFromLengthAndPreset(indentationLevel * BNF_INDENTATION_SIZE, ' ');
        CGString* tokenTypeString = BNFTokenType_toString(this->tokenType);
        printf("%sSentence '%s' (TokenType '%s', %u alternatives)\n", indentation, this->name, tokenTypeString, (this->alternatives != NULL ? CGArray_getSize(BNFAlternative, this->alternatives) : 0));
        if (this->alternatives != NULL) {
            CGArrayIterator(BNFAlternative)* iter = CGArrayIterator__new(BNFAlternative, this->alternatives);
            BNFAlternative* alternative = NULL;
            while ((alternative = CGArrayIterator_fetch(BNFAlternative, iter)) != NULL)
                BNFAlternative_print(alternative, indentationLevel + 1, seenSentences);
            CGArrayIterator_delete(BNFAlternative, iter);
        }
        CGString_delete(tokenTypeString);
        CGString_delete(indentation);
        CGArray_removeValueAt(BNFSentence, seenSentences, CGArray_getSize(BNFSentence, seenSentences) - 1);
    }
}

BNF_RDParser* BNF_RDParser__new(BNFSentence* startSentence) {
    BNF_RDParser* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->startSentence = startSentence;
        this->tokenListIterator = NULL;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BNFExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}
void BNF_RDParser_delete(BNF_RDParser* this) {
    BNFSentence_delete(this->startSentence);
    if (this->tokenListIterator != NULL)
        CGArrayIterator_delete(BNFToken, this->tokenListIterator);
    free(this);
}
BNFAst* BNF_RDParser_parse(BNF_RDParser* this, CGArray(BNFToken)* tokenList) {
    this->tokenListIterator = CGArrayIterator__new(BNFToken, tokenList);
    BNFAst* ast = BNFSentence_parse(this->startSentence, this->tokenListIterator);
    BNFToken* leftoverToken;
    if ((leftoverToken = CGArrayIterator_fetch(BNFToken, this->tokenListIterator)) == NULL)
        return ast;
    else {
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, BNFExceptionID_ScannerError, "leftover text after parse, '%s...'", BNFToken_getText(leftoverToken));
        return NULL;
    }
}

void BNF_RDParser_print(BNF_RDParser* this) {
    printf("Parser:\n"); 
    CGArray(BNFSentence)* seenSentences = CGArray__new(BNFSentence, 10);
    BNFSentence_print(this->startSentence, 1, seenSentences);
    CGArray_delete(BNFSentence, seenSentences);
}

void BNF_RDParser__printTokenList(CGArray(BNFToken)* tokenList) {
    CGArrayIterator(BNFToken)* iter = CGArrayIterator__new(BNFToken, tokenList);
    BNFToken* token = NULL;
    printf("Token List:\n");
    while ((token = CGArrayIterator_fetch(BNFToken, iter)) != NULL) {
        BNFToken_print(token);
        printf("\n");
    }
}

/* c dumping */
CGString* BNFSentence_createCDeclaration(BNFSentence* this) {
    CGString *text = CGString__newWithSprintf("BNFSentence* %sSentence = NULL;\n", this->name);
    return text;
}
CGString* BNFSentence_createCConstructor(BNFSentence* this) {
    CGString *text = NULL;
    if (this->alternatives == NULL)
        text = CGString__newWithSprintf("%sSentence = BNFSentence__newTerminalSymbol(CGString__new(\"%s\"), BNFTokenType_%s);\n", this->name, this->name, BNFTokenType_getName(this->tokenType));
    else 
        text = CGString__newWithSprintf("%sSentence = BNFSentence__new(CGString__new(\"%s\"), BNFTokenType_%s, CGArray__new(BNFAlternative, 1));\n", this->name, this->name, BNFTokenType_getName(this->tokenType));
    return text;
}
CGString* BNFSentence_createCAddAlternatives(BNFSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BNFAlternative)* iter = CGArrayIterator__new(BNFAlternative, this->alternatives);
    unsigned i = 0;
    BNFAlternative* alternative;
    while ((alternative = CGArrayIterator_fetch(BNFAlternative, iter)) != NULL) {
        CGString_append(text, CGString__newWithSprintf("    BNFSentence_addAlternative(%sSentence, %sSentenceAlternative%u);\n", this->name, this->name, i));
        i++;
    }
    return text;
}

CGString* BNFAlternative_createCDeclaration(BNFAlternative* this, CGString* sentenceName, unsigned index) {
    CGString* text = CGString__newWithSprintf("BNFAlternative* %sSentenceAlternative%u = NULL;\n", sentenceName, index);
    return text;
}
CGString* BNFAlternative_createCConstructor(BNFAlternative* this, CGString* sentenceName, unsigned index) {
    CGString* text = CGString__newWithSprintf("    %sSentenceAlternative%u = BNFAlternative__new(CGArray__new(BNFPhrase, 1));\n", sentenceName, index);
    return text;
}

CGString* BNFSentence_createCAlternativesDeclarations(BNFSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BNFAlternative)* iter = CGArrayIterator__new(BNFAlternative, this->alternatives);
    unsigned i = 0;
    BNFAlternative* alternative;
    while ((alternative = CGArrayIterator_fetch(BNFAlternative, iter)) != NULL) {
        CGString* ctext = BNFAlternative_createCDeclaration(alternative, this->name, i);
        CGString* text2 = CGString_append(text, ctext);
        CGString_delete(text);
        CGString_delete(ctext);
        text = text2;
        i++;
    }
    return text;
}
CGString* BNFSentence_createCAlternativesConstructors(BNFSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BNFAlternative)* iter = CGArrayIterator__new(BNFAlternative, this->alternatives);
    unsigned i = 0;
    BNFAlternative* alternative;
    while ((alternative = CGArrayIterator_fetch(BNFAlternative, iter)) != NULL) {
        CGString* ctext = BNFAlternative_createCConstructor(alternative, this->name, i);
        CGString* text2 = CGString_append(text, ctext);
        CGString_delete(text);
        CGString_delete(ctext);
        text = text2;
        i++;
    }
    return text;
}
CGString* BNFPhrase_createCDeclaration(BNFPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex) {
    CGString* text = CGString__newWithSprintf("BNFPhrase* %sSentenceAlternative%uPhrase%u = NULL;\n", sentenceName, alternativeIndex, phraseIndex);
    return text;
}
CGString* BNFPhrase_createCConstructor(BNFPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex) {
    CGString* repeatSwitchString = BNFPhraseRepeatSwitch_toString(this->repeatSwitch);
    CGString* text = CGString__newWithSprintf("    %sSentenceAlternative%uPhrase%u = BNFPhrase__new(%s, CGArray__new(BNFSentence, 1));\n", 
            sentenceName, alternativeIndex, phraseIndex, repeatSwitchString);
    CGString_delete(repeatSwitchString);
    return text;
}
CGString* BNFPhrase_createCAddParts(BNFPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex) {
    CGString *text = CGString__new("");
    CGArrayIterator(BNFSentence)* iter = CGArrayIterator__new(BNFSentence, this->parts);
    unsigned i = 0;
    BNFSentence* part;
    while ((part = CGArrayIterator_fetch(BNFSentence, iter)) != NULL) {
        CGString* appendText = CGString__newWithSprintf("    BNFPhrase_addPart(%sSentenceAlternative%uPhrase%u, %sSentence);\n", 
            sentenceName, alternativeIndex, phraseIndex, BNFSentence_getName(part));
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BNFAlternative_createCAddPhrases(BNFAlternative* this, CGString* sentenceName, unsigned int alternativeIndex) {
    CGString *text = CGString__new("");
    CGArrayIterator(BNFPhrase)* iter = CGArrayIterator__new(BNFPhrase, this->phrases);
    unsigned i = 0;
    BNFPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BNFPhrase, iter)) != NULL) {
        CGString* appendText = CGString__newWithSprintf("    BNFAlternative_addPhrase(%sSentenceAlternative%u, %sSentenceAlternative%uPhrase%u);\n", 
            sentenceName, alternativeIndex, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BNFAlternative_createCPhrasesAddParts(BNFAlternative* this, CGString* sentenceName, unsigned int alternativeIndex) {
    CGString *text = CGString__new("");
    CGArrayIterator(BNFPhrase)* iter = CGArrayIterator__new(BNFPhrase, this->phrases);
    unsigned i = 0;
    BNFPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BNFPhrase, iter)) != NULL) {
        CGString* appendText = BNFPhrase_createCAddParts(phrase, sentenceName, alternativeIndex, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BNFAlternative_createCPhrasesDeclarations(BNFAlternative* this, CGString* sentenceName, unsigned int alternativeIndex) {
    CGString *text = CGString__new("");
    CGArrayIterator(BNFPhrase)* iter = CGArrayIterator__new(BNFPhrase, this->phrases);
    unsigned i = 0;
    BNFPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BNFPhrase, iter)) != NULL) {
        CGString* appendText = BNFPhrase_createCDeclaration(phrase, sentenceName, alternativeIndex, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BNFAlternative_createCPhrasesConstructors(BNFAlternative* this, CGString* sentenceName, unsigned int alternativeIndex) {
    CGString *text = CGString__new("");
    CGArrayIterator(BNFPhrase)* iter = CGArrayIterator__new(BNFPhrase, this->phrases);
    unsigned i = 0;
    BNFPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BNFPhrase, iter)) != NULL) {
        CGString* appendText = BNFPhrase_createCConstructor(phrase, sentenceName, alternativeIndex, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BNFSentence_createCAlternativesPhrasesAddParts(BNFSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BNFAlternative)* iter = CGArrayIterator__new(BNFAlternative, this->alternatives);
    unsigned i = 0;
    BNFAlternative* alternative = NULL;
    while ((alternative = CGArrayIterator_fetch(BNFAlternative, iter)) != NULL) {
        CGString* appendText = BNFAlternative_createCPhrasesAddParts(alternative, this->name, i);
        text = CGString_appendWithSprintf_I(text, "%s", appendText);
        CGString_delete(appendText);
        i++;
    }
    return text;
}
CGString* BNFSentence_createCAlternativesPhrasesDeclarations(BNFSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BNFAlternative)* iter = CGArrayIterator__new(BNFAlternative, this->alternatives);
    unsigned i = 0;
    BNFAlternative* alternative = NULL;
    while ((alternative = CGArrayIterator_fetch(BNFAlternative, iter)) != NULL) {
        CGString* appendText = BNFAlternative_createCPhrasesDeclarations(alternative, this->name, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BNFSentence_createCAlternativesPhrasesConstructors(BNFSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BNFAlternative)* iter = CGArrayIterator__new(BNFAlternative, this->alternatives);
    unsigned i = 0;
    BNFAlternative* alternative = NULL;
    while ((alternative = CGArrayIterator_fetch(BNFAlternative, iter)) != NULL) {
        CGString* appendText = BNFAlternative_createCPhrasesConstructors(alternative, this->name, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
