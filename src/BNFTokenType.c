#include"BNFTokenType.h"

BNFTokenType* BNFTokenType__new(unsigned int id, CGString* name) {
    BNFTokenType* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->id = id;
        this->name = name;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, CGExceptionID_CannotAllocate, "Cannot allocate BNFTokenType");
    return this;
}
void BNFTokenType_delete(BNFTokenType* this) {
    CGString_delete(this->name);
    free(this);
}
BNFTokenType* BNFTokenType_clone(BNFTokenType* this) {
    return BNFTokenType__new(this->id, this->name);
}
CGString* BNFTokenType_getName(BNFTokenType* this) {
    return this->name;
}

CGString* BNFTokenType_toString(BNFTokenType* this) {
    return CGString__newWithSprintf("BNFTokenType (name: \"%s\", id: %u)", this->name, this->id);
}

bool BNFTokenType_isEqual(BNFTokenType* this, BNFTokenType* that) {
    return (this->id == that->id ? true : false);
}
