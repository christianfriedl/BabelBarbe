#include"Logger.h"
int main() {
    Logger__log(LOG_NOTICE, "a test (string '%s', int %i)", "string", 2);
    Logger__debug("a test (string '%s', int %i)", "string", 2);
    Logger__notice("a test (string '%s', int %i)", "string", 2);
    Logger__warn("a test (string '%s', int %i)", "string", 2);
    Logger__error("a test (string '%s', int %i)", "string", 2);
    return 0;
}
