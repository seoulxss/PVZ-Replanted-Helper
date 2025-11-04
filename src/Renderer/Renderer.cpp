#include "Renderer.h"

#include "../../ext/ImGui/imgui.h"
#include "../../ext/ImGui/imgui_impl_dx11.h"
#include "../../ext/ImGui/imgui_impl_win32.h"

std::uint8_t PVZ::Render::RendererCounter::m_Counter = 0;

PVZ::Render::Renderer::Renderer(const DXData& Data) : m_DxData(Data)
{
	m_IsInit = true;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	if (!Data.pSwap or !Data.pDevice or !Data.pDeviceCon)
	{
		m_IsInit = false;
	}

	if (!ImGui_ImplWin32_Init(m_DxData.hWnd))
	{
		m_IsInit = false;
	}

	if (!ImGui_ImplDX11_Init(m_DxData.pDevice, m_DxData.pDeviceCon))
	{
		m_IsInit = false;
	}
}

PVZ::Render::Renderer::~Renderer()
{
	if (m_IsInit)
	{
		m_Menus.clear();

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}

void PVZ::Render::Renderer::Render()
{
	if (m_Counter > 0 && m_IsInit)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		for (auto& menus : m_Menus)
		{
			if (menus.GetShowMenu())
				menus.Show();
		}


		ImGui::EndFrame();
		ImGui::Render();
		constexpr ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);
		constexpr float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		m_DxData.pDeviceCon->OMSetRenderTargets(1, &m_DxData.pRenderTargetView, nullptr);
		//m_DxData.pDeviceCon->ClearRenderTargetView(m_DxData.pRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

void PVZ::Render::Renderer::PushCounter()
{
	m_Counter++;
}

void PVZ::Render::Renderer::PopCounter()
{
	m_Counter--;
}

std::uint8_t PVZ::Render::Renderer::GetCounter() const
{
	return m_Counter;
}

const PVZ::Render::DXData& PVZ::Render::Renderer::GetDXData() const
{
	return m_DxData;
}

PVZ::Render::DXData& PVZ::Render::Renderer::GetDXData()
{
	return m_DxData;
}

std::list<PVZ::Menu>& PVZ::Render::Renderer::GetMenus()
{
	return m_Menus;
}
