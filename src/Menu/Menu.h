#pragma once
#include <memory>

namespace PVZ
{
	namespace Render
	{
		class Renderer;
	}

	class Menu
	{
	public:
		Menu(Render::Renderer* pRenderer);
		~Menu();

		void Show();

		bool GetShowMenu();
		void SetShowMenu(bool val);

		void SetOverlayClickable(bool val);

	private:
		bool m_ShowMenu = true;
		Render::Renderer* m_pRenderer = nullptr;
	};
}
