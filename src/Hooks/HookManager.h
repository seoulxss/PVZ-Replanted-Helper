#pragma once
#include "../Globals.h"
#include <polyhook2/Detour/NatDetour.hpp>
#include <unordered_map>
#include "HookDef.h"

namespace PVZ::Hooks
{
	enum class HOOK_ID : std::uint8_t
	{
		DX_PRESENT,



	};

	class HookManager final
	{
	public:
		HookManager();
		~HookManager();

		[[nodiscard]] std::expected<const PLH::NatDetour*, PVZ::STATUS> GetHook(HOOK_ID id) const;
		std::expected<PLH::NatDetour*, PVZ::STATUS> GetHook(HOOK_ID id);

		[[nodiscard]] std::expected<bool, PVZ::STATUS> IsHooked(HOOK_ID id);
		std::expected<bool, PVZ::STATUS> Hook(HOOK_ID id);
		std::expected<bool, PVZ::STATUS> UnHook(HOOK_ID id);

		std::expected<bool, PVZ::STATUS> AddHook(HOOK_ID id, std::uint64_t FuncAddr, std::uint64_t CallBack, std::uint64_t* Tramp);
		std::expected<bool, PVZ::STATUS> RemoveHook(HOOK_ID id);

	private:
		std::unordered_map<HOOK_ID, PLH::NatDetour> m_Hooks;
	};

}

namespace PVZ::Global
{
	inline std::shared_ptr<Hooks::HookManager> g_pHookManager = nullptr;
}