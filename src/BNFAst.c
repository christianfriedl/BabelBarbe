#include"BNF.h"
#include"BNFAst.h"

BNFAst* BNFAst__new(BNFAst* parent, Token* token, BNFAstPosition where) {
    BNFAst* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->parent = parent;
        this->token = token;
        this->left = NULL;
        this->right = NULL;
        if (parent != NULL) {
            if (where == BNFAstPosition_left)
                parent->left = this;
            else if (where == BNFAstPosition_right)
                parent->right = this;
            else
                BNF__raiseFatalError("Parent given, but no ast_pos argument");
        }
    } else
        BNF__raiseFatalError("Unable to allocate BNFAst.");
    return this;
}
void BNFAst_delete(BNFAst* this) {
    if (this->left != NULL)
        BNFAst_delete(this->left);
    if (this->right != NULL)
        BNFAst_delete(this->right);
    Token_delete(this->token);
    free(this);
}
BNFAst* BNFAst_getParent(BNFAst* this) {
    return this->parent;
}
BNFAst* BNFAst_getLeft(BNFAst* this) {
    return this->left;
}
BNFAst* BNFAst_getRight(BNFAst* this) {
    return this->right;
}
Token* BNFAst_getToken(BNFAst* this) {
    return this->token;
}
void BNFAst_setToken(BNFAst* this, Token* token) {
    Token_delete(this->token);
    this->token = token;
}
void BNFAst_setParent(BNFAst* this, BNFAst* parent) {
    this->parent = parent;
}
void BNFAst_setLeft(BNFAst* this, BNFAst* left) {
    this->left = left;
}
void BNFAst_setRight(BNFAst* this, BNFAst* right) {
    this->right = right;
}
void BNFAst_print_(BNFAst* this, unsigned int level);
void BNFAst_print(BNFAst* this) {
    BNFAst_print_(this, 0);
}
void BNFAst_print_(BNFAst* this, unsigned int level) {
    unsigned int i;
    if (this->left != NULL)
        BNFAst_print_(this->left, level + 1);
    for (i=0; i < level; ++i)
        printf("    ");
    Token_print(this->token);
    if (this->right != NULL)
        BNFAst_print_(this->right, level + 1);
}
