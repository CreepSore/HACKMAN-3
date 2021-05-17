#include <Windows.h>
#include <iostream>
#include <fstream>
#include "k-framework.h"


using namespace kfw::core;

PatchInfo* noRecoil = new PatchInfo("hitman3.exe", "\x74\x00\x48\x8d\x15\x00\x00\x00\x00\x48\x8d\x4c\x24\x00\xe8\x00\x00\x00\x00\x48\x8b\x4c\x24", "x?xxx????xxxx?x????xxxx", "\x74\x4B", "No Recoil");
PatchInfo* noSpread = new PatchInfo("hitman3.exe", "\xf3\x44\x0f\x11\x87\x00\x00\x00\x00\x76", "xxxxx????x", "\x90\x90\x90\x90\x90\x90\x90\x90\x90", "No Spread");
PatchInfo* infiniteAmmo = new PatchInfo("hitman3.exe", "\xe8\x00\x00\x00\x00\x83\xbf\x00\x00\x00\x00\x00\x76\x00\xc7\x87", "x????xx?????x?xx", "\x90\x90\x90\x90\x90", "Infinite Ammo"); // 0x140fc1c6
PatchInfo* pistolsMakeNoSound = new PatchInfo("hitman3.exe", "\xff\x90\x00\x00\x00\x00\x48\x8b\x06\x48\x8b\xce\xff\x90\x00\x00\x00\x00\x0f\x28\xbc\x24", "xx????xxxxxxxx????xxxx", "\x90\x90\x90\x90\x90\x90", "No Pistol sound");

bool jsInitialized = false;
duk_idx_t scriptIdx = 0;

void initializeJs() {
    std::ifstream sScript("kfw.js");
    std::string readScript((std::istreambuf_iterator<char>(sScript)),
        std::istreambuf_iterator<char>());
    sScript.close();

    duk_context* jCtx = Factory::getDefaultJsContext();
    Utils::setupJsContext(jCtx);
    Logger::setupJsContext(jCtx);
    if (duk_peval_string(jCtx, readScript.c_str()) != 0) {
        std::cout << "eval failed: " << duk_safe_to_string(jCtx, -1) << std::endl;
    }
    else {
        duk_push_string(jCtx, "init");
        if (duk_pcall_prop(jCtx, scriptIdx, 0) != 0) {
            std::cout << "Failed to initialize JavaScript:" << duk_safe_to_string(jCtx, -1) << std::endl;
        }
        duk_pop(jCtx);
        jsInitialized = true;
    }
}

void loadHack() {
    Utils::setupConsole();
    Logger* logger = Factory::getDefaultLogger();
    logger->log("Loading HACKMAN 3 ...", "loadHack");
    logger->log("Patching hacks ...", "loadHack");


    noRecoil->patch();
    //noSpread->patch();
    infiniteAmmo->patch();

    Factory::getDefaultHookManager()->hookAll();
    try {
        initializeJs();
    }
    catch (const std::exception& ex) {
        logger->log("Failed to initialize JavaScript", "loadHack");
    }
}

void unloadHack() {
    Logger* logger = Factory::getDefaultLogger();
    logger->log("Unloading HACKMAN 3 ...", "unloadHack");
    logger->log("Unpatching hacks ...", "unloadHack");
    noRecoil->unpatch();
    //noSpread->unpatch();
    infiniteAmmo->unpatch();
    Factory::cleanup();
    delete noRecoil;
    delete infiniteAmmo;
}

void __stdcall HackThread(void* hModule) {
    Logger* logger = Factory::getDefaultLogger();
    loadHack();
    duk_context* jCtx = Factory::getDefaultJsContext();
    logger->log("Loaded HACKMAN 3", "HackThread");
    while (!GetAsyncKeyState(VK_NUMPAD0)) {
        if (jsInitialized) {
            duk_push_string(jCtx, "loop");
            if (duk_pcall_prop(jCtx, scriptIdx, 0) != 0) {
                std::cout << "Failed to run JavaScript loop:" << duk_safe_to_string(jCtx, -1) << std::endl;
                jsInitialized = false;
            }
            duk_pop(jCtx);
        }
        Sleep(1);
    }
    
    unloadHack();

    Beep(1000, 1000);
    FreeLibraryAndExitThread((HMODULE)hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, (void*)hModule, 0, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

