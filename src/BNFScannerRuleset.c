#include"BNFScannerRuleset.h"

static BNFScannerRule* bnfScannerRuleset_instance = NULL;

static BNFScannerRule* BNFScannerRuleset__new_() {
    BNFScannerRule* startRule = BNFScannerRule__new(CGString__new("startRule"), NULL);
    BNFScannerRule* noiseRule = BNFScannerRule__new(CGString__new("noiseRule"), NULL);
    BNFScannerNode* identifierNode = BNFScannerNode__new(BNFScannerNodeType_regex, "\\w+", noiseRule, BNFTokenType_identifier, false);
    BNFScannerNode* noiseNode = BNFScannerNode__new(BNFScannerNodeType_regex, "\\s*", startRule, BNFTokenType_noise, true);
    BNFScannerNode* definitionSignNode = BNFScannerNode__new(BNFScannerNodeType_string, "::=", noiseRule, BNFTokenType_definitionSign, false);
    BNFScannerNode* semicolonNode = BNFScannerNode__new(BNFScannerNodeType_string, ";", noiseRule, BNFTokenType_semicolon, false);
    BNFScannerNode* orSignNode = BNFScannerNode__new(BNFScannerNodeType_string, "|", noiseRule, BNFTokenType_OrSign, false);
    BNFScannerNode* openParenNode = BNFScannerNode__new(BNFScannerNodeType_string, "(", noiseRule, BNFTokenType_openParen, false);
    BNFScannerNode* closeParenNode = BNFScannerNode__new(BNFScannerNodeType_string, ")", noiseRule, BNFTokenType_closeParen, false);
    BNFScannerNode* repeatZeroOrOnceNode = BNFScannerNode__new(BNFScannerNodeType_string, "?", noiseRule, BNFTokenType_repeatZeroOrOnce, false);
    BNFScannerNode* repeatZeroOrMoreNode = BNFScannerNode__new(BNFScannerNodeType_string, "*", noiseRule, BNFTokenType_repeatZeroOrMore, false);
    /* unneeded, this one is implied: BNFScannerNode* repeatOnceNode = BNFScannerNode__new(BNFScannerNodeType_string, "", noiseRule, BNFTokenType_repeatOnce, false); */
    BNFScannerNode* repeatManyNode = BNFScannerNode__new(BNFScannerNodeType_string, "+", noiseRule, BNFTokenType_repeatMany, false);
    BNFScannerNode* stringLiteralNode = BNFScannerNode__new(BNFScannerNodeType_regex, "'(\\'|[^'])*'", noiseRule, BNFTokenType_stringLiteral, false); /* TODO: escaping of '"' */
    BNFScannerNode* regexLiteralNode = BNFScannerNode__new(BNFScannerNodeType_regex, "/(\\/|[^/])*/", noiseRule, BNFTokenType_regexLiteral, false); /* TODO: escaping of '/'; possible TODO: we could conveniently return $1 from regex nodes, if there is one */
    BNFScannerRule_setNodes(startRule, CGArray__newFromInitializerList(BNFScannerNode, identifierNode, definitionSignNode, semicolonNode, orSignNode, openParenNode, closeParenNode, repeatZeroOrOnceNode, repeatZeroOrMoreNode, repeatManyNode, stringLiteralNode, regexLiteralNode, noiseNode));
    BNFScannerRule_setNodes(noiseRule, CGArray__newFromInitializerList(BNFScannerNode, identifierNode, definitionSignNode, semicolonNode, orSignNode, openParenNode, closeParenNode, repeatZeroOrOnceNode, repeatZeroOrMoreNode, repeatManyNode, stringLiteralNode, regexLiteralNode));

    return startRule;
}

BNFScannerRule* BNFScannerRuleset__getInstance() {
    if (bnfScannerRuleset_instance == NULL)
        bnfScannerRuleset_instance = BNFScannerRuleset__new_();
    return bnfScannerRuleset_instance;
}

/* TODO destructor, as useless as it is with a singleton (blargh) */
