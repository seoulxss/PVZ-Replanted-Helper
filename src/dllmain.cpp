#include <Windows.h>
#include "../ext/PatternScanner/PatternScanner.h"
#include "Hooks/HookManager.h"
#include "Logger/Logging.h"
#include "Pattern/Patterns.h"
#include "Version/VersionChecker.h"
//#pragma comment (linker, "/export:GetAdaptersAddresses=Proxy.GetAdaptersAddresses,@46")
//#pragma comment (linker, "/export:GetIfEntry=Proxy.GetIfEntry,@49")
//#pragma comment (linker, "/export:GetAdaptersInfo=Proxy.GetNetworkParams,@7d")
// -> Use for DDL Sideloading

void main(HMODULE hMod)
{
#ifdef _DEBUG
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);
    spdlog::set_default_logger(spdlog::default_logger());
    spdlog::set_level(spdlog::level::debug);
    PVZ::LogDebug(L"Initialized Logger!");
#endif

    //Init!
	//Get Module Info
    HMODULE AssemblyModule = GetModuleHandleW(L"GameAssembly.dll");
    MODULEINFO info;
    K32GetModuleInformation(GetCurrentProcess(), AssemblyModule, &info, sizeof(info));

    PVZ::Global::g_ModInfo = info;
    PVZ::Global::g_hAssembly = AssemblyModule;
    PVZ::Global::Funcs::g_pCoinInitialized = const_cast<void*>(Sig::find(AssemblyModule, info.SizeOfImage, PVZ::Pattern::g_pReloadedGameplayCoinCoinInitialize));
    PVZ::Global::Funcs::g_pIsMoney = const_cast<void*>(Sig::find(AssemblyModule, info.SizeOfImage, PVZ::Pattern::g_pReloadedGameplayCoinIsMoney));
    PVZ::Global::Funcs::g_pIsSun = const_cast<void*>(Sig::find(AssemblyModule, info.SizeOfImage, PVZ::Pattern::g_pReloadedGameplayCoinIsSun));
    PVZ::Global::Funcs::g_pCollect = const_cast<void*>(Sig::find(AssemblyModule, info.SizeOfImage, PVZ::Pattern::g_pReloadedGameplayCoinCollect));

    PVZ::Global::g_pHookManager = std::make_shared<PVZ::Hooks::HookManager>();
    if (!PVZ::Global::g_pHookManager)
        return;

    auto val = PVZ::Global::g_pHookManager->Hook(PVZ::Hooks::HOOK_ID::DX_PRESENT);
    if (!val.has_value())
        return;

    val = PVZ::Global::g_pHookManager->Hook(PVZ::Hooks::HOOK_ID::DX_RESIZE_BUFFERS);
    if (!val.has_value())
        return;

    val = PVZ::Global::g_pHookManager->Hook(PVZ::Hooks::HOOK_ID::RELOADED_GAMEPLAY_COIN_COININITIALIZE);
    if (!val.has_value())
        return;


    while (!GetAsyncKeyState(VK_F1))
        Sleep(20);

    PVZ::Global::g_pHookManager->UnHookAll();

#ifdef _DEBUG
    spdlog::default_logger().reset();
    FreeConsole();
    fclose(f);
#endif
    FreeLibraryAndExitThread(hMod, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    DisableThreadLibraryCalls(hModule);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(main), hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

