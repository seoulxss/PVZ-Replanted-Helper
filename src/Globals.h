#pragma once
#include <array>
#include <expected>
#include <memory>
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


		FILE_READ_ERROR,
		INVALID_VERSION_DATA, 
	};
}

namespace PVZ::Global
{
	inline std::array<const wchar_t*, 1> g_SupportedGameVersion
	{
		L"1.2.1205.0"
	};

	inline std::array<const wchar_t*, 1> g_SupportedGameVersionHash
	{
		L"75EA1F58F4BEE50C2E2ED300014E4D51A7E851EEDF50C73DC5F049324B13FC0A"
	};
}