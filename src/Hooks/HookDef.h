#pragma once
#include <cstdint>
#include <dxgi.h>
#include <memory>
#include <Windows.h>
#include "../../ext/ImGui/imgui.h"
#include "../IL2CPP/il2cpp.h"

namespace PVZ::Defs
{
	using tReloaded_Gameplay_Coin__Collect = void(Reloaded_Gameplay_Coin_o* pThis, int32_t playerIndex, bool spawnCoins, const MethodInfo* method);
}

namespace PVZ::Hooks
{
	namespace DX
	{
		namespace Present
		{
			void* GetSwapChainVTable();

			//Present idx: 8
			using tPresent = HRESULT(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
			HRESULT hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
			inline std::uint64_t g_oPresent = 0;
		}

		namespace ResizeBuffers
		{
			//ResizeBuffers idx: 13
			using tResizeBuffers = HRESULT(UINT BufferCount,UINT Width,UINT Height,DXGI_FORMAT NewFormat,UINT SwapChainFlag);
			HRESULT hkResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlag);
			inline std::uint64_t g_oResizeBuffers = 0;
		}

	}

	namespace Windows
	{
		namespace WndProc
		{
			LRESULT hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
			inline std::uint64_t g_oWndProc = 0;
		}

	}

	namespace Coin
	{
		using tReloaded_Gameplay_Coin__CoinInitialize = void(Reloaded_Gameplay_Coin_o* pThis, float theX, float theY, int32_t theCoinType, int32_t theCoinMotion, const MethodInfo* method);
		void __fastcall hkReloadedGameplayCoinCoinInitialize(Reloaded_Gameplay_Coin_o* pThis, float theX, float theY, int32_t theCoinType, int32_t theCoinMotion, const MethodInfo* method);
		inline std::uint64_t g_oCoinInitialize = 0;
	}


}