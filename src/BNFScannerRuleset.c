#include"BNFScannerRuleset.h"

static BNFScannerRule* bnfScannerRuleset_instance = NULL;

static BNFScannerRule* BNFScannerRuleset__new_() {
    BNFScannerRule* identifierRule = NULL;
    BNFScannerNode* identifierNode = BNFScannerNode__new(BNFScannerNodeType_regex, "\\w+", identifierRule, BNFTokenType_identifier, false);
    BNFScannerRule* startRule = BNFScannerRule__new(CGArray__newFromInitializerList(BNFScannerNode, identifierNode, NULL));

    return startRule;
}

BNFScannerRule* BNFScannerRuleset__getInstance() {
    if (bnfScannerRuleset_instance != NULL)
        bnfScannerRuleset_instance = BNFScannerRuleset__new_();
        return bnfScannerRuleset_instance;
}
