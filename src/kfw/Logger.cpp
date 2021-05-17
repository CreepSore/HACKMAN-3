#include "k-framework.h"
#include <stdio.h>
#include <iostream>
#include <ctime>

void kfw::core::Logger::log(const std::string& msg, const std::string& context)
{
    // Next-Level hacky ISO-Date thing
    // thanks @ https://stackoverflow.com/questions/9527960/how-do-i-construct-an-iso-8601-datetime-in-c
    const time_t now = time(nullptr);
    struct tm timeinfo;
    char buf[21];
    gmtime_s(&timeinfo, &now);
    strftime(buf, sizeof buf, "%FT%TZ", &timeinfo);

    std::cout << "[" << buf << "]" << "[" << context << "] " << msg << std::endl;
}

duk_ret_t js_log(duk_context* ctx) {
    const char* msg = duk_get_string(ctx, 0);
    const char* context = "JavaScript";

    if (duk_get_top(ctx) == 2) {
        context = duk_get_string(ctx, 1);
    }

    kfw::core::Factory::getDefaultLogger()->log(msg, context);
    return 0;
}

void kfw::core::Logger::setupJsContext(duk_context* ctx) {
    duk_push_c_function(ctx, js_log, DUK_VARARGS);
    duk_put_global_string(ctx, "log");
}
