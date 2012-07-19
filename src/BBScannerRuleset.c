#include"BBScannerRuleset.h"

static BBScannerRule* bnfScannerRuleset_instance = NULL;

static BBScannerRule* BBScannerRuleset__new_() {
	BBTokenTypeFactory *ttf = BBTokenTypeFactory__getInstance();
	BBTokenType_nonTerminal = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_nonTerminal");
	BBTokenType_noise = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_noise");
	BBTokenType_identifier = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_identifier");
	BBTokenType_definitionSign = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_definitionSign");
	BBTokenType_semicolon = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_semicolon");

	BBTokenType_OrSign = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_OrSign");
	BBTokenType_openParen = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_openParen");
	BBTokenType_closeParen = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_closeParen");
	BBTokenType_repeatZeroOrOnce = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_repeatZeroOrOnce");
	BBTokenType_repeatZeroOrMore = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_repeatZeroOrMore");

	BBTokenType_repeatMany = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_repeatMany");
	BBTokenType_stringLiteral = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_stringLiteral");
	BBTokenType_regexLiteral = BBTokenTypeFactory_createBBTokenType(ttf, "BBTokenType_regexLiteral");

    BBScannerRule* startRule = BBScannerRule__new(CGString__new("startRule"), NULL);
    BBScannerRule* noiseRule = BBScannerRule__new(CGString__new("noiseRule"), NULL);
    BBScannerNode* identifierNode = BBScannerNode__new(BBScannerNodeType_regex, "\\w+", noiseRule, BBTokenType_identifier, false, NULL);
    BBScannerNode* noiseNode = BBScannerNode__new(BBScannerNodeType_regex, "\\s*", startRule, BBTokenType_noise, true, NULL);
    BBScannerNode* definitionSignNode = BBScannerNode__new(BBScannerNodeType_string, "::=", noiseRule, BBTokenType_definitionSign, false, NULL);
    BBScannerNode* semicolonNode = BBScannerNode__new(BBScannerNodeType_string, ";", noiseRule, BBTokenType_semicolon, false, NULL);
    BBScannerNode* orSignNode = BBScannerNode__new(BBScannerNodeType_string, "|", noiseRule, BBTokenType_OrSign, false, NULL);
    BBScannerNode* openParenNode = BBScannerNode__new(BBScannerNodeType_string, "(", noiseRule, BBTokenType_openParen, false, NULL);
    BBScannerNode* closeParenNode = BBScannerNode__new(BBScannerNodeType_string, ")", noiseRule, BBTokenType_closeParen, false, NULL);
    BBScannerNode* repeatZeroOrOnceNode = BBScannerNode__new(BBScannerNodeType_string, "?", noiseRule, BBTokenType_repeatZeroOrOnce, false, NULL);
    BBScannerNode* repeatZeroOrMoreNode = BBScannerNode__new(BBScannerNodeType_string, "*", noiseRule, BBTokenType_repeatZeroOrMore, false, NULL);
    BBScannerNode* repeatManyNode = BBScannerNode__new(BBScannerNodeType_string, "+", noiseRule, BBTokenType_repeatMany, false, NULL);
    BBScannerNode* stringLiteralNode = BBScannerNode__new(BBScannerNodeType_regex, "'(\\\\'|[^'])*'", noiseRule, BBTokenType_stringLiteral, false, BBToken_removeQuotationMarks); 
    BBScannerNode* regexLiteralNode = BBScannerNode__new(BBScannerNodeType_regex, "/(\\\\/|[^/])*/", noiseRule, BBTokenType_regexLiteral, false, BBToken_removeRegexSlashes); 
    BBScannerRule_setNodes(startRule, CGArray__newFromInitializerList(BBScannerNode, identifierNode, definitionSignNode, 
                semicolonNode, orSignNode, openParenNode, closeParenNode, repeatZeroOrOnceNode, repeatZeroOrMoreNode, 
                repeatManyNode, stringLiteralNode, regexLiteralNode, noiseNode, NULL));
    BBScannerRule_setNodes(noiseRule, CGArray__newFromInitializerList(BBScannerNode, noiseNode, NULL));

    return startRule;
}

BBScannerRule* BBScannerRuleset__getInstance() {
    if (bnfScannerRuleset_instance == NULL)
        bnfScannerRuleset_instance = BBScannerRuleset__new_();
    return bnfScannerRuleset_instance;
}

/* TODO destructor, as useless as it is with a singleton (blargh) */
