#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"BB_RDParser.h"

DEFINE_ARRAY(BBAst);
DEFINE_ARRAY(BBSentence);
DEFINE_ARRAY(BBPhrase);
DEFINE_ARRAY(BBAlternative);
DEFINE_ARRAY(BBTokenType);
DEFINE_ARRAY_ITERATOR(BBToken);
DEFINE_ARRAY_ITERATOR(BBTokenType);
DEFINE_ARRAY_ITERATOR(BBAlternative);
DEFINE_ARRAY_ITERATOR(BBSentence);
DEFINE_ARRAY_ITERATOR(BBPhrase);

BBAst* BBSentence_parse(BBSentence* this, CGArrayIterator(BBToken)* tokenIterator);

CGString* BBPhraseRepeatSwitch_toString(BBPhraseRepeatSwitch this) {
    switch(this) {
        case BBPhraseRepeat_zeroOrOnce:
            return CGString__new("BBPhraseRepeat_zeroOrOnce");
        case BBPhraseRepeat_zeroOrMore:
            return CGString__new("BBPhraseRepeat_zeroOrMore");
        case BBPhraseRepeat_once:
            return CGString__new("BBPhraseRepeat_once");
        case BBPhraseRepeat_many:
            return CGString__new("BBPhraseRepeat_many");
        default:
            CGAppState_THROW(CGAppState__getInstance(), CGExceptionID_GeneralNonfatalException, Severity_warning, "no such BBPhraseRepeatSwitch");
            return NULL;
    }
}


BBPhrase* BBPhrase__new(BBPhraseRepeatSwitch repeatSwitch, CGArray(BBSentence)* parts) {
    BBPhrase* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->repeatSwitch = repeatSwitch;
        this->parts = parts;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}

BBPhrase* BBPhrase_clone(BBPhrase* this) {
    return BBPhrase__new(this->repeatSwitch, this->parts);
}

    
void BBPhrase_delete(BBPhrase* this) {
    /* TODO: ooops, with recursion this probably will lead to problems... solution: object-store? coughcough refcounting cough? */
    /* CGArray_deleteValues(BBSentence, this->parts); */
    CGAppState_THROW(CGAppState__getInstance(), Severity_warning, CGExceptionID_GeneralNonfatalException, "TODO: sentences are currently not deleted - create object-store for them");
    CGArray_delete(BBSentence, this->parts);
    free(this);
}

void BBPhrase_setParts(BBPhrase* this, CGArray(BBSentence)* parts) {
    this->parts = parts;
}

void BBPhrase_addPart(BBPhrase* this, BBSentence* part) {
    CGArray_add(BBSentence, this->parts, part);
}

CGArray(BBAst)* BBPhrase_parse(BBPhrase* this, CGArrayIterator(BBToken)* tokenIterator) {
    bool isParsed = false;
    CGArrayIterator(BBSentence)* partsIterator = CGArrayIterator__new(BBSentence, this->parts);
    BBSentence* sentence = NULL;
    CGArray(BBAst)* asts = CGArray__new(BBAst, 1);
    BBAst* ast = NULL;
    if (this->repeatSwitch == BBPhraseRepeat_zeroOrMore || this->repeatSwitch == BBPhraseRepeat_zeroOrOnce)
        isParsed = true;
    bool partIsParsed;
    do {
        partIsParsed = true;
        while ((sentence = CGArrayIterator_fetch(BBSentence, partsIterator)) != NULL) {
            //printf("phrase found sentence\n");
            if ((ast = BBSentence_parse(sentence, tokenIterator)) != NULL) {
                //printf("phrase received ast, partIsParsed is %i\n", partIsParsed);
                CGArray_add(BBAst, asts, ast);
            } else {
                partIsParsed = false;
                break;
            }
        }
        CGArrayIterator_reset(BBSentence, partsIterator);
        if (this->repeatSwitch == BBPhraseRepeat_once) {
            isParsed = partIsParsed;
            break;
        } else if (this->repeatSwitch == BBPhraseRepeat_zeroOrOnce) {
            break;
        } else if (this->repeatSwitch == BBPhraseRepeat_many)
            isParsed |= partIsParsed;
    } while (partIsParsed);
    //printf("phrase claims parsed is %i\n", isParsed);
    if (isParsed)
        return asts;
    else
        return NULL;
}
void BBPhrase_print(BBPhrase* this, unsigned int indentationLevel, CGArray(BBSentence)* seenSentences) {
    CGString* indentation = CGString__newFromLengthAndPreset(indentationLevel * BB_INDENTATION_SIZE, ' ');
    CGString* rsString = BBPhraseRepeatSwitch_toString(this->repeatSwitch);
    #ifdef DEBUG
    printf("%sBBPhrase (repeatSwitch %s)\n", indentation, rsString);
    #endif
    CGArrayIterator(BBSentence)* iter = CGArrayIterator__new(BBSentence, this->parts);
    BBSentence* part = NULL;
    while ((part = CGArrayIterator_fetch(BBSentence, iter)) != NULL)
        BBSentence_print(part, indentationLevel + 1, seenSentences);
    CGArrayIterator_delete(BBSentence, iter);
    CGString_delete(rsString);
    CGString_delete(indentation);
}

BBAlternative* BBAlternative__new(CGArray(BBPhrase)* phrases) {
    BBAlternative* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->phrases = phrases;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}
void BBAlternative_addPhrase(BBAlternative* this, BBPhrase* phrase) {
    CGArray_add(BBPhrase, this->phrases, phrase);
}
BBAlternative* BBAlternative_clone(BBAlternative* this) {
    return BBAlternative__new(this->phrases);
}
void BBAlternative_delete(BBAlternative* this) {
    CGArray_deleteValues(BBPhrase, this->phrases);
    CGArray_delete(BBPhrase, this->phrases);
    free(this);
}
CGArray(BBAst)* BBAlternative_parse(BBAlternative* this, CGArrayIterator(BBToken)* tokenIterator) {
    CGArray(BBAst)* asts = NULL;
    CGArray(BBAst)* phraseAsts = NULL;
    CGArrayIterator(BBPhrase)* iter = CGArrayIterator__new(BBPhrase, this->phrases);
    BBPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BBPhrase, iter)) != NULL) {
        if ((phraseAsts = BBPhrase_parse(phrase, tokenIterator)) != NULL) {
            if (asts == NULL)
                asts = CGArray__new(BBAst, 3);
            CGArray_append(BBAst, asts, phraseAsts);
        } else
            break;
    }
    #ifdef DEBUG
    if (asts != NULL) {
        printf("alternative - received asts from phrases:\n");
        printf("%u\n", CGArray_getSize(BBAst, asts));
    }
    #endif
    return asts;
}
void BBAlternative_print(BBAlternative* this, unsigned int indentationLevel, CGArray(BBSentence)* seenSentences) {
    CGString* indentation = CGString__newFromLengthAndPreset(indentationLevel * BB_INDENTATION_SIZE, ' '); 
    printf("%sBBAlternative (%u BBPhrases)\n", indentation, CGArray_getSize(BBPhrase, this->phrases));
    CGArrayIterator(BBPhrase)* iter = CGArrayIterator__new(BBPhrase, this->phrases);
    BBPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BBPhrase, iter)) != NULL)
        BBPhrase_print(phrase, indentationLevel + 1, seenSentences);
    CGArrayIterator_delete(BBPhrase, iter);
    CGString_delete(indentation);
}


/* if alternatives is NULL, then this is a terminal symbol! */
BBSentence* BBSentence__new(CGString* name, BBTokenType* tokenType, CGArray(BBAlternative)* alternatives) {
    BBSentence* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->name = name;
        this->tokenType = tokenType;
        this->alternatives = alternatives;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}
BBSentence* BBSentence__newTerminalSymbol(CGString* name, BBTokenType* tokenType) {
    return BBSentence__new(name, tokenType, NULL);
}
BBSentence* BBSentence__newNonTerminalSymbol(CGString* name, BBTokenType* tokenType) {
    return BBSentence__new(name, tokenType, CGArray__new(BBAlternative, 1));
}
BBSentence* BBSentence__newFromBBAst(BBAst* ast, BBTokenType* tokenType) {
    char *sentenceName = NULL, *tokenText = NULL;
    char *name = NULL;

    sentenceName = BBSentence_getName(BBAst_getSentence(ast));
    asprintf(&name, "%sSentence", tokenText); 
    /* token type is, of course, NOT one of "our" BBParser token types, but a newly generated one */
    return BBSentence__new(name, tokenType, NULL);
}
BBSentence* BBSentence_clone(BBSentence* this) {
    return BBSentence__new(this->name, this->tokenType, this->alternatives);
}
CGString* BBSentence_getName(BBSentence* this) {
    return this->name;
}
BBTokenType* BBSentence_getTokenType(BBSentence* this) {
    return this->tokenType;
}
void BBSentence_delete(BBSentence* this) {
    CGString_delete(this->name);
    if (this->alternatives != NULL) {
        CGArray_deleteValues(BBAlternative, this->alternatives);
        CGArray_delete(BBAlternative, this->alternatives);
    }
    free(this);
}

BBAst* BBSentence_parse(BBSentence* this, CGArrayIterator(BBToken)* tokenIterator) {
    if (this->alternatives == NULL) { /* this is a terminal symbol */
        BBToken* token = CGArrayIterator_fetch(BBToken, tokenIterator);
        #ifdef DEBUG
        printf("terminal symbol sentence for token %s %s\n", token != NULL ? BBToken_getTypeName(token): "(NULL)" , token != NULL ? BBToken_getText(token) : "(NULL)");
        #endif
        if (token == NULL) {
            return NULL;
        } else if (BBTokenType_isEqual(BBToken_getType(token), this->tokenType))
            return BBAst__new(NULL, token, this);
        else {
            CGArrayIterator_unFetch(BBToken, tokenIterator);
            return NULL;
        }
    } else {
        BBAst* ast = NULL;
        CGArrayIterator(BBAlternative)* alternativesIterator = CGArrayIterator__new(BBAlternative, this->alternatives);
        BBAlternative* alternative = NULL;
        while ((alternative = CGArrayIterator_fetch(BBAlternative, alternativesIterator)) != NULL) {
            //printf("trying alternative @%ld\n", alternative);
            CGArray(BBAst)* altAsts = BBAlternative_parse(alternative, tokenIterator);
            if (altAsts != NULL) {
                ast = BBAst__new(NULL, BBToken__new(this->tokenType, CGString__new("")), this);
                //printf("created ast @%ld with tokenType %s\n", ast, BBTokenType_toString(this->tokenType));
                BBAst_setSubAsts(ast, altAsts);
                break;
            } else if (CGAppState_isExceptionRaisedWithID(CGAppState__getInstance(), BBExceptionID_ScannerError))
                return NULL;
            /* Possible TODO: warn if there are multiple applicable alternatives */
        }
        return ast;
    }
}

int BBSentence_compare(const BBSentence** first, const BBSentence** second) {
    //printf("BBSentence_compare comparing %ld and %ld, returning %ld\n", *second, *first, (*second-*first));
    return (*second - *first);
}
void BBSentence_setAlternatives(BBSentence* this, CGArray(BBAlternative)* alternatives) {
    this->alternatives = alternatives;
}

void BBSentence_addAlternative(BBSentence* this, BBAlternative* alternative) {
    CGArray_add(BBAlternative, this->alternatives, alternative);
}

void BBSentence_print(BBSentence* this, unsigned int indentationLevel, CGArray(BBSentence)* seenSentences) {
    if (CGArray_find(BBSentence, seenSentences, this, BBSentence_compare) == NULL) {
        CGArray_add(BBSentence, seenSentences, this);
        CGString* indentation = CGString__newFromLengthAndPreset(indentationLevel * BB_INDENTATION_SIZE, ' ');
        CGString* tokenTypeString = BBTokenType_toString(this->tokenType);
        printf("%sSentence '%s' (TokenType '%s', %u alternatives)\n", indentation, this->name, tokenTypeString, (this->alternatives != NULL ? CGArray_getSize(BBAlternative, this->alternatives) : 0));
        if (this->alternatives != NULL) {
            CGArrayIterator(BBAlternative)* iter = CGArrayIterator__new(BBAlternative, this->alternatives);
            BBAlternative* alternative = NULL;
            while ((alternative = CGArrayIterator_fetch(BBAlternative, iter)) != NULL)
                BBAlternative_print(alternative, indentationLevel + 1, seenSentences);
            CGArrayIterator_delete(BBAlternative, iter);
        }
        CGString_delete(tokenTypeString);
        CGString_delete(indentation);
        CGArray_removeValueAt(BBSentence, seenSentences, CGArray_getSize(BBSentence, seenSentences) - 1);
    }
}

BB_RDParser* BB_RDParser__new(BBSentence* startSentence) {
    BB_RDParser* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->startSentence = startSentence;
        this->tokenListIterator = NULL;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, BBExceptionID_ScannerError, "unable to allocate in %s", __func__);
    return this;
}
void BB_RDParser_delete(BB_RDParser* this) {
    BBSentence_delete(this->startSentence);
    if (this->tokenListIterator != NULL)
        CGArrayIterator_delete(BBToken, this->tokenListIterator);
    free(this);
}
BBAst* BB_RDParser_parse(BB_RDParser* this, CGArray(BBToken)* tokenList) {
    this->tokenListIterator = CGArrayIterator__new(BBToken, tokenList);
    BBAst* ast = BBSentence_parse(this->startSentence, this->tokenListIterator);
    BBToken* leftoverToken;
    if ((leftoverToken = CGArrayIterator_fetch(BBToken, this->tokenListIterator)) == NULL)
        return ast;
    else {
        CGAppState_THROW(CGAppState__getInstance(), Severity_error, BBExceptionID_ScannerError, "leftover text after parse, '%s...'", BBToken_getText(leftoverToken));
        return NULL;
    }
}

void BB_RDParser_print(BB_RDParser* this) {
    printf("Parser:\n"); 
    CGArray(BBSentence)* seenSentences = CGArray__new(BBSentence, 10);
    BBSentence_print(this->startSentence, 1, seenSentences);
    CGArray_delete(BBSentence, seenSentences);
}

void BB_RDParser__printTokenList(CGArray(BBToken)* tokenList) {
    CGArrayIterator(BBToken)* iter = CGArrayIterator__new(BBToken, tokenList);
    BBToken* token = NULL;
    printf("Token List:\n");
    while ((token = CGArrayIterator_fetch(BBToken, iter)) != NULL) {
        BBToken_print(token);
        printf("\n");
    }
}

/* c dumping */
CGString* BBSentence_createCDeclaration(BBSentence* this) {
    CGString *text = CGString__newWithSprintf("BBSentence* %sSentence = NULL;\n", this->name);
    return text;
}
CGString* BBSentence_createCConstructor(BBSentence* this) {
    CGString *text = NULL;
    if (this->alternatives == NULL)
        text = CGString__newWithSprintf("%sSentence = BBSentence__newTerminalSymbol(CGString__new(\"%s\"), BBTokenType_%s);\n", this->name, this->name, BBTokenType_getName(this->tokenType));
    else 
        text = CGString__newWithSprintf("%sSentence = BBSentence__new(CGString__new(\"%s\"), BBTokenType_%s, CGArray__new(BBAlternative, 1));\n", this->name, this->name, BBTokenType_getName(this->tokenType));
    return text;
}
CGString* BBSentence_createCAddAlternatives(BBSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBAlternative)* iter = CGArrayIterator__new(BBAlternative, this->alternatives);
    unsigned i = 0;
    BBAlternative* alternative;
    while ((alternative = CGArrayIterator_fetch(BBAlternative, iter)) != NULL) {
        text = CGString_append(text, CGString__newWithSprintf("    BBSentence_addAlternative(%sSentence, %sSentenceAlternative%u);\n", this->name, this->name, i));
        i++;
    }
    return text;
}

CGString* BBAlternative_createCDeclaration(BBAlternative* this, CGString* sentenceName, unsigned index) {
    CGString* text = CGString__newWithSprintf("BBAlternative* %sSentenceAlternative%u = NULL;\n", sentenceName, index);
    return text;
}
CGString* BBAlternative_createCConstructor(BBAlternative* this, CGString* sentenceName, unsigned index) {
    CGString* text = CGString__newWithSprintf("    %sSentenceAlternative%u = BBAlternative__new(CGArray__new(BBPhrase, 1));\n", sentenceName, index);
    return text;
}

CGString* BBSentence_createCAlternativesDeclarations(BBSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBAlternative)* iter = CGArrayIterator__new(BBAlternative, this->alternatives);
    unsigned i = 0;
    BBAlternative* alternative;
    while ((alternative = CGArrayIterator_fetch(BBAlternative, iter)) != NULL) {
        CGString* ctext = BBAlternative_createCDeclaration(alternative, this->name, i);
        CGString* text2 = CGString_append(text, ctext);
        CGString_delete(text);
        CGString_delete(ctext);
        text = text2;
        i++;
    }
    return text;
}
CGString* BBSentence_createCAlternativesConstructors(BBSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBAlternative)* iter = CGArrayIterator__new(BBAlternative, this->alternatives);
    unsigned i = 0;
    BBAlternative* alternative;
    while ((alternative = CGArrayIterator_fetch(BBAlternative, iter)) != NULL) {
        CGString* ctext = BBAlternative_createCConstructor(alternative, this->name, i);
        CGString* text2 = CGString_append(text, ctext);
        CGString_delete(text);
        CGString_delete(ctext);
        text = text2;
        i++;
    }
    return text;
}
CGString* BBPhrase_createCDeclaration(BBPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex) {
    CGString* text = CGString__newWithSprintf("BBPhrase* %sSentenceAlternative%uPhrase%u = NULL;\n", sentenceName, alternativeIndex, phraseIndex);
    return text;
}
CGString* BBPhrase_createCConstructor(BBPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex) {
    CGString* repeatSwitchString = BBPhraseRepeatSwitch_toString(this->repeatSwitch);
    CGString* text = CGString__newWithSprintf("    %sSentenceAlternative%uPhrase%u = BBPhrase__new(%s, CGArray__new(BBSentence, 1));\n", 
            sentenceName, alternativeIndex, phraseIndex, repeatSwitchString);
    CGString_delete(repeatSwitchString);
    return text;
}
CGString* BBPhrase_createCAddParts(BBPhrase* this, CGString* sentenceName, unsigned alternativeIndex, unsigned phraseIndex) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBSentence)* iter = CGArrayIterator__new(BBSentence, this->parts);
    unsigned i = 0;
    BBSentence* part;
    while ((part = CGArrayIterator_fetch(BBSentence, iter)) != NULL) {
        CGString* appendText = CGString__newWithSprintf("    BBPhrase_addPart(%sSentenceAlternative%uPhrase%u, %sSentence);\n", 
            sentenceName, alternativeIndex, phraseIndex, BBSentence_getName(part));
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BBAlternative_createCAddPhrases(BBAlternative* this, CGString* sentenceName, unsigned int alternativeIndex) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBPhrase)* iter = CGArrayIterator__new(BBPhrase, this->phrases);
    unsigned i = 0;
    BBPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BBPhrase, iter)) != NULL) {
        CGString* appendText = CGString__newWithSprintf("    BBAlternative_addPhrase(%sSentenceAlternative%u, %sSentenceAlternative%uPhrase%u);\n", 
            sentenceName, alternativeIndex, sentenceName, alternativeIndex, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BBAlternative_createCPhrasesAddParts(BBAlternative* this, CGString* sentenceName, unsigned int alternativeIndex) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBPhrase)* iter = CGArrayIterator__new(BBPhrase, this->phrases);
    unsigned i = 0;
    BBPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BBPhrase, iter)) != NULL) {
        CGString* appendText = BBPhrase_createCAddParts(phrase, sentenceName, alternativeIndex, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BBAlternative_createCPhrasesDeclarations(BBAlternative* this, CGString* sentenceName, unsigned int alternativeIndex) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBPhrase)* iter = CGArrayIterator__new(BBPhrase, this->phrases);
    unsigned i = 0;
    BBPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BBPhrase, iter)) != NULL) {
        CGString* appendText = BBPhrase_createCDeclaration(phrase, sentenceName, alternativeIndex, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BBAlternative_createCPhrasesConstructors(BBAlternative* this, CGString* sentenceName, unsigned int alternativeIndex) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBPhrase)* iter = CGArrayIterator__new(BBPhrase, this->phrases);
    unsigned i = 0;
    BBPhrase* phrase = NULL;
    while ((phrase = CGArrayIterator_fetch(BBPhrase, iter)) != NULL) {
        CGString* appendText = BBPhrase_createCConstructor(phrase, sentenceName, alternativeIndex, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BBSentence_createCAlternativesPhrasesAddParts(BBSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBAlternative)* iter = CGArrayIterator__new(BBAlternative, this->alternatives);
    unsigned i = 0;
    BBAlternative* alternative = NULL;
    while ((alternative = CGArrayIterator_fetch(BBAlternative, iter)) != NULL) {
        CGString* appendText = BBAlternative_createCPhrasesAddParts(alternative, this->name, i);
        text = CGString_appendWithSprintf_I(text, "%s", appendText);
        CGString_delete(appendText);
        i++;
    }
    return text;
}
CGString* BBSentence_createCAlternativesPhrasesDeclarations(BBSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBAlternative)* iter = CGArrayIterator__new(BBAlternative, this->alternatives);
    unsigned i = 0;
    BBAlternative* alternative = NULL;
    while ((alternative = CGArrayIterator_fetch(BBAlternative, iter)) != NULL) {
        CGString* appendText = BBAlternative_createCPhrasesDeclarations(alternative, this->name, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BBSentence_createCAlternativesPhrasesConstructors(BBSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBAlternative)* iter = CGArrayIterator__new(BBAlternative, this->alternatives);
    unsigned i = 0;
    BBAlternative* alternative = NULL;
    while ((alternative = CGArrayIterator_fetch(BBAlternative, iter)) != NULL) {
        CGString* appendText = BBAlternative_createCPhrasesConstructors(alternative, this->name, i);
        CGString* text2 = CGString_append(text, appendText);
        CGString_delete(text);
        CGString_delete(appendText);
        text = text2;
        i++;
    }
    return text;
}
CGString* BBSentence_createCAlternativesAddPhrases(BBSentence* this) {
    CGString *text = CGString__new("");
    CGArrayIterator(BBAlternative)* iter = CGArrayIterator__new(BBAlternative, this->alternatives);
    unsigned i = 0;
    BBAlternative* alternative;
    while ((alternative = CGArrayIterator_fetch(BBAlternative, iter)) != NULL) {
        CGString* ctext = BBAlternative_createCAddPhrases(alternative, this->name, i);
        CGString* text2 = CGString_append(text, ctext);
        CGString_delete(text);
        CGString_delete(ctext);
        text = text2;
        i++;
    }
    return text;
}
