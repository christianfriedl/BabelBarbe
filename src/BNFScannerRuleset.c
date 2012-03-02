#include"BNFScannerRuleset.h"

static BNFScannerRule* bnfScannerRuleset_instance = NULL;

static BNFScannerRule* BNFScannerRuleset__new_() {
    BNFScannerRule* noiseRule = BNFScannerRule__new(NULL);
    BNFScannerNode* identifierNode = BNFScannerNode__new(BNFScannerNodeType_regex, "\\w+", noiseRule, BNFTokenType_identifier, false);
    BNFScannerNode* noiseNode = BNFScannerNode__new(BNFScannerNodeType_regex, "\\s*", noiseRule, BNFTokenType_noise, true);
    BNFScannerNode* definitionSignNode = BNFScannerNode__new(BNFScannerNodeType_string, "::=", noiseRule, BNFTokenType_definitionSign, false);
    BNFScannerNode* semicolonNode = BNFScannerNode__new(BNFScannerNodeType_string, ";", noiseRule, BNFTokenType_semicolon, false);
    BNFScannerNode* orSignNode = BNFScannerNode__new(BNFScannerNodeType_string, "|", noiseRule, BNFTokenType_OrSign, false);
    BNFScannerNode* openParenNode = BNFScannerNode__new(BNFScannerNodeType_string, "(", noiseRule, BNFTokenType_openParen, false);
    BNFScannerNode* closeParenNode = BNFScannerNode__new(BNFScannerNodeType_string, ")", noiseRule, BNFTokenType_closeParen, false);
    BNFScannerNode* repeatZeroOrMoreNode = BNFScannerNode__new(BNFScannerNodeType_string, "*", noiseRule, BNFTokenType_repeatZeroOrMore, false);
    /* unneeded, this one is implied: BNFScannerNode* repeatOnceNode = BNFScannerNode__new(BNFScannerNodeType_string, "", noiseRule, BNFTokenType_repeatOnce, false); */
    BNFScannerNode* repeatManyNode = BNFScannerNode__new(BNFScannerNodeType_string, "+", noiseRule, BNFTokenType_repeatMany, false);
    BNFScannerNode* stringLiteralNode = BNFScannerNode__new(BNFScannerNodeType_regex, "\"[^\"]*\"", noiseRule, BNFTokenType_stringLiteral, false); /* TODO: escaping " */
    BNFScannerNode* regexLiteralNode = BNFScannerNode__new(BNFScannerNodeType_regex, "/[^\\/]+/", noiseRule, BNFTokenType_regexLiteral, false); /* possible TODO: we could conveniently return $1 from regex nodes, if there is one */
    BNFScannerRule* startRule = BNFScannerRule__new(CGArray__newFromInitializerList(BNFScannerNode, identifierNode, noiseNode, NULL));
    BNFScannerRule_setNodes(noiseRule, CGArray__newFromInitializerList(BNFScannerNode, identifierNode, definitionSignNode, semicolonNode, orSignNode, openParenNode, closeParenNode, repeatZeroOrMoreNode, repeatManyNode, stringLiteralNode, regexLiteralNode));

    return startRule;
}

BNFScannerRule* BNFScannerRuleset__getInstance() {
    if (bnfScannerRuleset_instance == NULL)
        bnfScannerRuleset_instance = BNFScannerRuleset__new_();
    return bnfScannerRuleset_instance;
}

/* TODO destructor, as useless as it is with a singleton (blargh) */
