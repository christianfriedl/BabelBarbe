#include"BBTokenTypeFactory.h"

static BBTokenTypeFactory* BBTokenTypeFactory__instance_ = NULL;

static BBTokenTypeFactory* BBTokenTypeFactory__new_() {
    BBTokenTypeFactory* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->nextId = 0;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, CGExceptionID_CannotAllocate, "Cannot allocate BBTokenTypeFactory");
    return this;
}
BBTokenTypeFactory* BBTokenTypeFactory_delete(BBTokenTypeFactory* this) {
    free(this);
}
BBTokenTypeFactory* BBTokenTypeFactory__getInstance() {
    if (BBTokenTypeFactory__instance_ == NULL)
        BBTokenTypeFactory__instance_ = BBTokenTypeFactory__new_();
    return BBTokenTypeFactory__instance_;
}

BBTokenType* BBTokenTypeFactory_createBBTokenType(BBTokenTypeFactory* this, CGString* name) {
    return BBTokenType__new(this->nextId++, name);
}
