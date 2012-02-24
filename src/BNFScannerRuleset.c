#include"BNFScannerRuleset.h"

static BNFScannerRule* bnfScannerRuleset_instance = NULL;

static BNFScannerRule* BNFScannerRuleset__new_(CGAppState* appState) {
    BNFScannerRule* identifierRule = NULL;
    BNFScannerNode* identifierNode = BNFScannerNode__new(appState, BNFScannerNodeType_regex, "\\w+", identifierRule, BNFTokenType_identifier, false);
    BNFScannerRule* startRule = BNFScannerRule__new(appState, CGArray__newFromInitializerList(appState, BNFScannerNode, identifierNode, NULL));

    return startRule;
}

BNFScannerRule* BNFScannerRuleset__getInstance(CGAppState* appState) {
    if (bnfScannerRuleset_instance != NULL)
        bnfScannerRuleset_instance = BNFScannerRuleset__new_(appState);
        return bnfScannerRuleset_instance;
}
