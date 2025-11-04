#include <Windows.h>

#include "Hooks/HookManager.h"
#include "Logger/Logging.h"
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
    PVZ::LogDebug(L"Initialised Logger!");
#endif

    auto ver = PVZ::Version::IsSupportedVersionEx();
	if (ver.has_value())
	{
        if (ver.value() == false)
        {
            MessageBeep(MB_ICONERROR);
            MessageBoxW(nullptr, L"Looks like this version is not supported (yet)", L"Error!", MB_OK);
            Sleep(1000);
#ifdef _DEBUG
            spdlog::default_logger().reset();
            FreeConsole();
            fclose(f);
#endif
            FreeLibraryAndExitThread(hMod, 0);
        }
        
        PVZ::Global::g_pHookManager = std::make_shared<PVZ::Hooks::HookManager>();
        PVZ::Global::g_pHookManager->HookAll();

        while (!GetAsyncKeyState(VK_F1))
            Sleep(20);

	}

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

