#include "Menu.h"

#include "../../ext/ImGui/imgui.h"
#include "../Hooks/HookManager.h"
#include "../Renderer/Renderer.h"

PVZ::Menu::Menu(std::shared_ptr<Render::Renderer> pRenderer) : m_pRenderer(pRenderer)
{
}

void PVZ::Menu::Show()
{
	if (m_ShowMenu)
	{
		ImGui::Begin("main");

		ImGui::Text("Test");

		ImGui::End();
	}
}

bool PVZ::Menu::GetShowMenu()
{
	return m_ShowMenu;
}

void PVZ::Menu::SetHowMenu(bool val)
{
	m_ShowMenu = val;
}

void PVZ::Menu::SetOverlayClickable(bool val)
{
	if (Hooks::Windows::WndProc::g_oWndProc == 0)
		Hooks::Windows::WndProc::g_oWndProc = GetWindowLongPtrW(m_pRenderer->GetDXData().hWnd, GWLP_WNDPROC);

	if (val)
		SetWindowLongPtrW(m_pRenderer->GetDXData().hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Hooks::Windows::WndProc::hkWndProc));

	else if (!val)
		SetWindowLongPtrW(m_pRenderer->GetDXData().hWnd, GWLP_WNDPROC, static_cast<LONG_PTR>(Hooks::Windows::WndProc::g_oWndProc));
}
