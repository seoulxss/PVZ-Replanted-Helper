#include "HookDef.h"
#include <d3d11.h>
#include "HookManager.h"
#include "../Globals.h"
#include "../Logger/Logging.h"

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

		return p_vtable;
	}

	return nullptr;
}

HRESULT PVZ::Hooks::DX::Present::hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	PVZ::LogDebug(L"Present!");




	return reinterpret_cast<tPresent*>(g_oPresent)(pSwapChain, SyncInterval, Flags);
}

HRESULT PVZ::Hooks::DX::ResizeBuffers::hkResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat,
	UINT SwapChainFlag)
{

	PVZ::LogDebug(L"ResizeBuffers!");

	return reinterpret_cast<tResizeBuffers*>(g_oResizeBuffers)(BufferCount, Width, Height, NewFormat, SwapChainFlag);
}
