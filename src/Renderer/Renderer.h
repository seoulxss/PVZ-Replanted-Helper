#pragma once
#include <cstdint>
#include <d3d11.h>
#include <memory>
#include "../Logger/Logging.h"
#include "../Menu/Menu.h"
#pragma comment(lib, "d3d11.lib")

namespace PVZ::Render
{
	struct DXData
	{
		DXData()
		{
		}

		IDXGISwapChain* pSwap = nullptr;
		ID3D11DeviceContext* pDeviceCon = nullptr;
		ID3D11Device* pDevice = nullptr;
		ID3D11RenderTargetView* pRenderTargetView = nullptr;
		HWND hWnd = nullptr;
	};

	class RendererCounter
	{
	public:
		RendererCounter()
		{
			m_Counter++;

			if (m_Counter > 0)
			{
				PVZ::LogCritical(L"Multiple renderers created!");
			}
		}

	private:
		static std::uint8_t m_Counter;
	};

	class Renderer final : public RendererCounter
	{
	public:
		Renderer(const DXData& Data);
		~Renderer();

		void Render();

		void PushCounter();
		void PopCounter();
		[[nodiscard]] std::uint8_t GetCounter() const;

		[[nodiscard]] const DXData& GetDXData() const;
		DXData& GetDXData();

		std::list<Menu>& GetMenus();
	private:
		std::list<Menu> m_Menus;
		bool m_IsInit = false;
		std::uint8_t m_Counter = 0;
		DXData m_DxData;
	};
}

namespace PVZ::Global
{
	inline std::unique_ptr<PVZ::Render::Renderer> g_pRenderer = nullptr;
}

