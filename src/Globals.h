#pragma once
#include <Windows.h>
#include <array>
#include <expected>
#include <memory>
#include <Psapi.h>
#include <vector>
#include <string>

namespace PVZ
{
	enum class STATUS : uint8_t
	{
		NOT_FOUND,
		ALREADY_THERE,
		INVALID_VALUE,
		VALUE_WAS_NULLPTR,


		FILE_NOT_FOUND,
		FILE_WRITE_ERROR,
		FILE_READ_ERROR,
		INVALID_VERSION_DATA, 
	};
}

namespace PVZ::Global
{

	inline MODULEINFO g_ModInfo = {};
	inline HMODULE g_hAssembly = nullptr;

	inline std::array<const wchar_t*, 1> g_SupportedGameVersion
	{
		L"1.3.1217.0"
	};

	inline std::array<const wchar_t*, 1> g_SupportedGameVersionHash
	{
		L"4707F22E7C7C49B2E1E8F5AF530CF1E5BF4DCC1838324D89588DEA483D010188"
	};
}