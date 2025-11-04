#include "HookDef.h"
#include <d3d11.h>
#include "HookManager.h"
#include "../Globals.h"
#include "../Logger/Logging.h"
#include "../Renderer/Renderer.h"
#include "../../ext/PatternScanner/PatternScanner.h"
#include "../Pattern/Patterns.h"


void* PVZ::Hooks::DX::Present::GetSwapChainVTable()
{
	//18 entries
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = FindWindowW(nullptr, L"PvZ Replanted");
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGISwapChain* swap_chain = nullptr;
	ID3D11Device* device = nullptr;

	constexpr D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		feature_levels,
		2,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		nullptr,
		nullptr) == S_OK)
	{
		void** p_vtable = *reinterpret_cast<void***>(swap_chain);
		swap_chain->Release();
		device->Release();
		//context->Release();

		return static_cast<void*>(p_vtable);
	}

	return nullptr;
}

HRESULT PVZ::Hooks::DX::Present::hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	PVZ::LogDebug(L"Present!");
	//Init Renderer
	if (!PVZ::Global::g_pRenderer)
	{
		Render::DXData data{};

		DXGI_SWAP_CHAIN_DESC desc;
		pSwapChain->GetDesc(&desc);

		pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&data.pDevice));
		data.pDevice->GetImmediateContext(&data.pDeviceCon);
		data.pSwap = pSwapChain;
		data.hWnd = desc.OutputWindow;

		//Create Renderer
		Global::g_pRenderer = std::make_unique<Render::Renderer>(data);
		ID3D11Texture2D* pBackBuffer = nullptr;
		Global::g_pRenderer->GetDXData().pSwap->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		Global::g_pRenderer->GetDXData().pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &Global::g_pRenderer->GetDXData().pRenderTargetView);
		pBackBuffer->Release();

		//Add menu
		Global::g_pRenderer->GetMenus().emplace_front(Global::g_pRenderer.get());
	}

	if (PVZ::Global::g_pRenderer)
	{
		static bool show = false;
		static bool lastState = false;
		const bool isPressed = (GetAsyncKeyState(VK_RSHIFT) & 0x8000) != 0;

		if (isPressed && !lastState)
		{
			show = !show;
			if (show)
				Global::g_pRenderer->PushCounter();

			else
				Global::g_pRenderer->PopCounter();

			Global::g_pRenderer->GetMenus().front().SetOverlayClickable(show);
		}

		lastState = isPressed;
		Global::g_pRenderer->Render();
	}


	return reinterpret_cast<tPresent*>(g_oPresent)(pSwapChain, SyncInterval, Flags);
}

HRESULT PVZ::Hooks::DX::ResizeBuffers::hkResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat,
	UINT SwapChainFlag)
{

	PVZ::LogDebug(L"ResizeBuffers!");

	return reinterpret_cast<tResizeBuffers*>(g_oResizeBuffers)(BufferCount, Width, Height, NewFormat, SwapChainFlag);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT PVZ::Hooks::Windows::WndProc::hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	if (PVZ::Global::g_pRenderer)
	{
		if (Global::g_pRenderer->GetMenus().front().GetShowMenu())
			return true;
	}

	return CallWindowProcA(reinterpret_cast<WNDPROC>(g_oWndProc), hWnd, msg, wParam, lParam);
}

void __fastcall PVZ::Hooks::Coin::hkReloadedGameplayCoinCoinInitialize(Reloaded_Gameplay_Coin_o* pThis, float theX, float theY,
	int32_t theCoinType, int32_t theCoinMotion, const MethodInfo* method)
{
	reinterpret_cast<tReloaded_Gameplay_Coin__CoinInitialize*>(g_oCoinInitialize)(pThis, theX, theY, theCoinType, theCoinMotion, method);

	//Collect it instantly
	static std::uintptr_t* collect = (std::uintptr_t*)Sig::find(Global::g_hAssembly, Global::g_ModInfo.SizeOfImage, Pattern::g_pReloadedGameplayCoinCollect);
	reinterpret_cast<Defs::tReloaded_Gameplay_Coin__Collect*>(collect)(pThis, 0, true, nullptr);

}
