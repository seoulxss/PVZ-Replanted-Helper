#include <Windows.h>
//#pragma comment (linker, "/export:GetAdaptersAddresses=Proxy.GetAdaptersAddresses,@46")
//#pragma comment (linker, "/export:GetIfEntry=Proxy.GetIfEntry,@49")
//#pragma comment (linker, "/export:GetAdaptersInfo=Proxy.GetNetworkParams,@7d")
// -> Use for DDL Sideloading

void main(HMODULE hMod)
{




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

