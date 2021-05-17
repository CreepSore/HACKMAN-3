#include <iostream>
#include "k-framework.h"

using namespace kfw::core;

duk_ret_t js_findPattern(duk_context* ctx) {
    const char* mod = duk_get_string(ctx, 0);
    const char* mask = duk_get_string(ctx, 2);
 
    size_t maskLength = strlen(mask);
    char* pattern = new char[maskLength];
    
    int i = 0;
    duk_enum(ctx, 1, DUK_ENUM_ARRAY_INDICES_ONLY);
    while (duk_next(ctx, -1, 1)) {
        *(pattern + i++) = duk_get_number(ctx, -1);
        duk_pop_2(ctx);
    }
    duk_pop(ctx);


    DWORD64 result = Utils::findPattern(mod, pattern, mask);
    delete pattern;
    duk_push_number(ctx, result);
    return 1;
}

duk_ret_t js_read_uint64(duk_context* ctx) {
    duk_double_t address = duk_get_number(ctx, 0);
    uint64_t result = *(uint64_t*)(DWORD64)address;
    duk_push_number(ctx, result);
    return 1;
}

duk_ret_t js_read_uint32(duk_context* ctx) {
    duk_double_t address = duk_get_number(ctx, 0);
    uint32_t result = *(uint32_t*)(DWORD64)address;
    duk_push_number(ctx, result);
    return 1;
}

duk_ret_t js_read_uint8(duk_context* ctx) {
    duk_double_t address = duk_get_number(ctx, 0);
    uint8_t result = *(uint8_t*)(DWORD64)address;
    duk_push_number(ctx, result);
    return 1;
}

duk_ret_t js_read(duk_context* ctx) {
    DWORD64 address = duk_get_number(ctx, 0);
    DWORD64 size = duk_get_number(ctx, 1);
    DWORD64 num = *(DWORD64*)address;

    DWORD64 bitshift = 0;
    for (DWORD64 i = 0; i < size; i++) {
        bitshift += 0xFF << i*8;
    }

    duk_push_number(ctx, num & bitshift);
    return 1;
}

duk_ret_t js_readBytes(duk_context* ctx) {
    DWORD64 address = (DWORD64)duk_get_number(ctx, 0);
    DWORD64 count = (DWORD64)duk_get_number(ctx, 1);

    duk_idx_t arr = duk_push_array(ctx);
    for (int i = 0; i < count; i++) {
        const char toSet = *(unsigned char*)(address + i);
        duk_push_uint(ctx, toSet & 0xFF);
        duk_put_prop_index(ctx, arr, i);
    }
    
    return 1;
}

duk_ret_t js_read_string(duk_context* ctx) {
    duk_double_t address = duk_get_number(ctx, 0);
    const char* result = (char*)(DWORD64)address;
    duk_push_string(ctx, result);
    return 1;
}

duk_ret_t js_write(duk_context* ctx) {
    DWORD64 address = duk_get_number(ctx, 0);
    DWORD64 value = duk_get_number(ctx, 1);
    DWORD64 size = duk_get_number(ctx, 2);

    memcpy((void*)address, (void*)value, size);
    return 0;
}

duk_ret_t js_getTickCount(duk_context* ctx) {
    duk_push_number(ctx, GetTickCount64());
    return 1;
}

void Utils::setupJsContext(duk_context* ctx) {
    duk_push_c_function(ctx, js_findPattern, 3);
    duk_put_global_string(ctx, "util_findPattern");

    duk_push_c_function(ctx, js_read_string, 1);
    duk_put_global_string(ctx, "readString");

    duk_push_c_function(ctx, js_read_uint64, 1);
    duk_put_global_string(ctx, "readUInt64");

    duk_push_c_function(ctx, js_read_uint32, 1);
    duk_put_global_string(ctx, "readUInt32");

    duk_push_c_function(ctx, js_read_uint8, 1);
    duk_put_global_string(ctx, "readUInt8");

    duk_push_c_function(ctx, js_readBytes, 2);
    duk_put_global_string(ctx, "readBytes");

    duk_push_c_function(ctx, js_read, 2);
    duk_put_global_string(ctx, "read");

    duk_push_c_function(ctx, js_write, 3);
    duk_put_global_string(ctx, "write");

    duk_push_c_function(ctx, js_getTickCount, 2);
    duk_put_global_string(ctx, "getTickCount");
}
