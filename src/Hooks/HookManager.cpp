#include "HookManager.h"
#include <ranges>
#include "../../ext/PatternScanner/PatternScanner.h"
#include "../Pattern/Patterns.h"

PVZ::Hooks::HookManager::HookManager()
{
	m_Hooks.reserve(100);

	std::uint64_t* Vtable = static_cast<std::uint64_t*>(DX::Present::GetSwapChainVTable());
	std::uint64_t PresentAddr = Vtable[8];
	std::uint64_t ResizeBuffersAddr = Vtable[13];

	m_Hooks.try_emplace(HOOK_ID::DX_PRESENT, PresentAddr, reinterpret_cast<std::uint64_t>(DX::Present::hkPresent), &DX::Present::g_oPresent);
	m_Hooks.try_emplace(HOOK_ID::DX_RESIZE_BUFFERS, ResizeBuffersAddr, reinterpret_cast<std::uint64_t>(DX::ResizeBuffers::hkResizeBuffers), &DX::ResizeBuffers::g_oResizeBuffers);
	m_Hooks.try_emplace(HOOK_ID::RELOADED_GAMEPLAY_COIN_COININITIALIZE, reinterpret_cast<uintptr_t>(Global::Funcs::g_pCoinInitialized), reinterpret_cast<std::uint64_t>(Coin::hkReloadedGameplayCoinCoinInitialize), &Coin::g_oCoinInitialize);
}

PVZ::Hooks::HookManager::~HookManager()
{
	for (auto& val : m_Hooks | std::views::values)
		val.unHook();
}

std::expected<const PLH::NatDetour*, PVZ::STATUS> PVZ::Hooks::HookManager::GetHook(HOOK_ID id) const
{
	const auto it = m_Hooks.find(id);
	if (it != m_Hooks.end())
		return &it->second;

	return std::unexpected(PVZ::STATUS::NOT_FOUND);
}

std::expected<PLH::NatDetour*, PVZ::STATUS> PVZ::Hooks::HookManager::GetHook(HOOK_ID id)
{
	const auto it = m_Hooks.find(id);
	if (it != m_Hooks.end())
		return &it->second;

	return std::unexpected(PVZ::STATUS::NOT_FOUND);
}

std::expected<bool, PVZ::STATUS> PVZ::Hooks::HookManager::IsHooked(HOOK_ID id)
{
	const auto it = m_Hooks.find(id);
	if (it != m_Hooks.end())
		return it->second.isHooked();

	return std::unexpected(PVZ::STATUS::NOT_FOUND);
}

std::expected<bool, PVZ::STATUS> PVZ::Hooks::HookManager::Hook(HOOK_ID id)
{
	const auto it = m_Hooks.find(id);
	if (it != m_Hooks.end())
	{
		if (it->second.isHooked())
			return true;

		else
		{
			it->second.hook();
			return true;
		}
	}

	return std::unexpected(PVZ::STATUS::NOT_FOUND);
}

std::expected<bool, PVZ::STATUS> PVZ::Hooks::HookManager::UnHook(HOOK_ID id)
{
	const auto it = m_Hooks.find(id);
	if (it != m_Hooks.end())
	{
		if (!it->second.isHooked())
			return true;

		else
		{
			it->second.unHook();
			return true;
		}
	}

	return std::unexpected(PVZ::STATUS::NOT_FOUND);
}

std::expected<bool, PVZ::STATUS> PVZ::Hooks::HookManager::AddHook(HOOK_ID id, std::uint64_t FuncAddr, std::uint64_t CallBack, std::uint64_t* Tramp)
{
	const auto it = m_Hooks.find(id);
	if (it == m_Hooks.end())
	{
		m_Hooks.try_emplace(id, FuncAddr, CallBack, Tramp);
		return true;
	}

	return std::unexpected(PVZ::STATUS::ALREADY_THERE);
}

std::expected<bool, PVZ::STATUS> PVZ::Hooks::HookManager::RemoveHook(HOOK_ID id)
{
	const auto it = m_Hooks.find(id);
	if (it == m_Hooks.end())
		return std::unexpected(PVZ::STATUS::NOT_FOUND);


	it->second.unHook();
	m_Hooks.erase(it);
	return true;
}

void PVZ::Hooks::HookManager::HookAll()
{
	for (auto& val : m_Hooks | std::views::values)
	{
		if (!val.isHooked())
			val.hook();
	}
}

void PVZ::Hooks::HookManager::UnHookAll()
{
	for (auto& val : m_Hooks | std::views::values)
	{
		if (val.isHooked())
			val.unHook();
	}
}

