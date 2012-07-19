#include"BBTokenType.h"

BBTokenType* BBTokenType__new(unsigned int id, CGString* name) {
    BBTokenType* this = malloc(sizeof(*this));
    if (this != NULL) {
        this->id = id;
        this->name = name;
    } else
        CGAppState_THROW(CGAppState__getInstance(), Severity_fatal, CGExceptionID_CannotAllocate, "Cannot allocate BBTokenType");
    return this;
}
void BBTokenType_delete(BBTokenType* this) {
    CGString_delete(this->name);
    free(this);
}
BBTokenType* BBTokenType_clone(const BBTokenType* this) {
    return BBTokenType__new(this->id, this->name);
}
CGString* BBTokenType_getName(BBTokenType* this) {
    return this->name;
}

CGString* BBTokenType_toString(BBTokenType* this) {
    return CGString__newWithSprintf("BBTokenType (name: \"%s\", id: %u)", this->name, this->id);
}

bool BBTokenType_isEqual(BBTokenType* this, BBTokenType* that) {
    return (this->id == that->id ? true : false);
}

CGString* BBTokenType_createCDeclaration(BBTokenType* this) {
    CGString* text = CGString__newWithSprintf("BBTokenType* BBTokenType_%s = NULL;\n", this->name);
    return text;
}
CGString* BBTokenType_createCConstructor(BBTokenType* this) {
    CGString* text = CGString__newWithSprintf("BBTokenType_%s = BBTokenType__new(%u, \"%s\");\n", this->name, this->id, this->name);
    return text;
}
