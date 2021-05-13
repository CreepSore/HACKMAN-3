#include <Windows.h>
#include "k-framework.h"

using namespace kfw::core;

PatchInfo* noRecoil = new PatchInfo("hitman3.exe", "\x74\x00\x48\x8d\x15\x00\x00\x00\x00\x48\x8d\x4c\x24\x00\xe8\x00\x00\x00\x00\x48\x8b\x4c\x24", "x?xxx????xxxx?x????xxxx", "\x74\x4B", "No Recoil");
PatchInfo* noSpread = new PatchInfo("hitman3.exe", "\xf3\x44\x0f\x11\x87\x00\x00\x00\x00\x76", "xxxxx????x", "\x90\x90\x90\x90\x90\x90\x90\x90\x90", "No Spread");
PatchInfo* infiniteAmmo = new PatchInfo("hitman3.exe", "\xe8\x00\x00\x00\x00\x83\xbf\x00\x00\x00\x00\x00\x76\x00\xc7\x87", "x????xx?????x?xx", "\x90\x90\x90\x90\x90", "Infinite Ammo");

void loadHack() {
    Utils::setupConsole();
    Logger* logger = Factory::getDefaultLogger();
    logger->log("Loading HACKMAN 3 ...", "HackThread");
    logger->log("Patching hacks ...", "HackThread");

    noRecoil->patch();
    noSpread->patch();
    infiniteAmmo->patch();

    Factory::getDefaultHookManager()->hookAll();
}

void unloadHack() {
    Logger* logger = Factory::getDefaultLogger();
    logger->log("Unloading HACKMAN 3 ...", "HackThread");
    logger->log("Unpatching hacks ...", "HackThread");
    noRecoil->unpatch();
    noSpread->unpatch();
    infiniteAmmo->unpatch();
    Factory::cleanup();
    delete noRecoil;
    delete infiniteAmmo;
}

void __stdcall HackThread(void* hModule) {
    Logger* logger = Factory::getDefaultLogger();
    
    loadHack();

    logger->log("Loaded HACKMAN 3", "HackThread");
    while (!GetAsyncKeyState(VK_NUMPAD0)) {
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

