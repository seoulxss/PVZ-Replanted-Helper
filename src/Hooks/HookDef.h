#pragma once
#include <cstdint>
#include <dxgi.h>
#include <memory>
#include <Windows.h>

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






}