#include"BNFTokenTypeFactory.h"

static BNFTokenTypeFactory* BNFTokenTypeFactory__instance_ = NULL;

static BNFTokenTypeFactory* BNFTokenTypeFactory__new_() {
    BNFTokenTypeFactory* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->nextId = 0;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, CGExceptionID_CannotAllocate, "Cannot allocate BNFTokenTypeFactory");
    return this;
}
BNFTokenTypeFactory* BNFTokenTypeFactory_delete(BNFTokenTypeFactory* this) {
    free(this);
}
BNFTokenTypeFactory* BNFTokenTypeFactory__getInstance() {
    if (BNFTokenTypeFactory__instance_ == NULL)
        BNFTokenTypeFactory__instance_ = BNFTokenTypeFactory__new_();
    return BNFTokenTypeFactory__instance_;
}

BNFTokenType* BNFTokenTypeFactory_createBNFTokenType(BNFTokenTypeFactory* this, CGString* name) {
    return BNFTokenType__new(this->nextId++, name);
}
