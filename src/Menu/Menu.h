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
		Menu(std::shared_ptr<Render::Renderer> pRenderer);

		void Show();

		bool GetShowMenu();
		void SetHowMenu(bool val);

		void SetOverlayClickable(bool val);

	private:
		bool m_ShowMenu = true;
		std::shared_ptr<Render::Renderer> m_pRenderer = nullptr;
	};
}
