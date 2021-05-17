#include "k-framework.h"
#include "duktape.h"

kfw::core::HackManager* kfw::core::Factory::hackManager = nullptr;
kfw::core::HookManager* kfw::core::Factory::hookManager = nullptr;
kfw::core::Logger* kfw::core::Factory::logger = nullptr;
duk_context* kfw::core::Factory::jsContext = nullptr;

kfw::core::HookManager * kfw::core::Factory::getDefaultHookManager()
{
    if (hookManager == nullptr) hookManager = new HookManager();
    return hookManager;
}

kfw::core::HackManager* kfw::core::Factory::getDefaultHackManager()
{
    if (hackManager == nullptr) hackManager = new HackManager();
    return hackManager;
}

kfw::core::Logger* kfw::core::Factory::getDefaultLogger()
{
    if (logger == nullptr) logger = new Logger();
    return logger;
}

duk_context* kfw::core::Factory::getDefaultJsContext()
{
    if (jsContext == nullptr) jsContext = duk_create_heap_default();
    return jsContext;
}

void kfw::core::Factory::cleanup()
{
    delete hackManager;
    delete hookManager;
    delete logger;
    duk_destroy_heap(jsContext);
}
