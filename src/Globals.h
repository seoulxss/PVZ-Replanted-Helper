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

namespace Settings
{
	inline bool g_CollectSun = false;
	inline bool g_CollectMoney = false;


}

namespace PVZ::Global
{

	inline MODULEINFO g_ModInfo = {};
	inline HMODULE g_hAssembly = nullptr;


	namespace Funcs
	{
		inline void* g_pCoinInitialized = nullptr;
		inline void* g_pCollect = nullptr;
		inline void* g_pIsMoney = nullptr;
		inline void* g_pIsSun = nullptr;

	}
}