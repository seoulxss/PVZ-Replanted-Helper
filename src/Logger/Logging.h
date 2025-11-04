#pragma once
#include <spdlog/spdlog.h>

namespace PVZ
{
#ifdef _DEBUG
	inline void LogDebug(const wchar_t* Msg)
	{
		spdlog::default_logger()->log(spdlog::level::debug, Msg);
	}

	inline void LogCritical(const wchar_t* Msg)
	{
		spdlog::default_logger()->log(spdlog::level::critical, Msg);
	}
#else
	inline void LogDebug(const wchar_t* Msg)
	{
	}

	inline void LogCritical(const wchar_t* Msg)
	{
	}
#endif


}


