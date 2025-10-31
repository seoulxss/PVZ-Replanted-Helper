#include "VersionChecker.h"
#include <Windows.h>
#include "../Blake3/Blake3Wrapper.h"

#include <Psapi.h>

#include "../Hooks/HookManager.h"
#pragma comment(lib, "psapi.lib")

std::expected<bool, PVZ::STATUS> PVZ::Version::IsSupportedVersionEx()
{
	auto winVer = IsSupportedVersionWin();
	if (winVer.has_value())
	{
		if (winVer.value() == true)
		{
			auto mod = GetModuleHandleW(L"GameAssembly.dll");
			if (!mod)
				return std::unexpected(PVZ::STATUS::NOT_FOUND);

			MODULEINFO info;
			if (K32GetModuleInformation(GetCurrentProcess(), mod, &info, sizeof(info)) == FALSE)
				return std::unexpected(PVZ::STATUS::INVALID_VALUE);

			//Check hash
			PVZ::Hasher::Blake3Wrapper hasher;
			std::vector<std::uint8_t> bytes(info.SizeOfImage, 0);
			std::memcpy(bytes.data(), info.lpBaseOfDll, info.SizeOfImage);

			auto Hash = hasher.GetHashValueW(bytes);
			if (!Hash.has_value())
				return std::unexpected(Hash.error());

			for (const auto& hashes : PVZ::Global::g_SupportedGameVersionHash)
			{
				if (Hash == hashes)
					return true;
			}

			return false;
		}
	}

	else if (!winVer.has_value())
		return std::unexpected(winVer.error());

	else
		return false;
}

std::expected<bool, PVZ::STATUS> PVZ::Version::IsSupportedVersionWin()
{
	HMODULE mod = GetModuleHandleW(L"GameAssembly.dll");
	if (!mod)
		return std::unexpected(PVZ::STATUS::NOT_FOUND);
	

	std::wstring buffer(MAX_PATH, L'\0');
	const auto len = K32GetModuleFileNameExW(GetCurrentProcess(), mod, buffer.data(), static_cast<DWORD>(buffer.size()));
	if (len == 0) //Len = 0
		return std::unexpected(PVZ::STATUS::INVALID_VALUE);

	buffer.resize(len);

	DWORD size = GetFileVersionInfoSizeW(buffer.c_str(), nullptr);
	if (size == 0)
		return std::unexpected(PVZ::STATUS::INVALID_VALUE);

	std::vector<BYTE> data(size);
	if (!GetFileVersionInfoW(buffer.c_str(), 0, size, data.data()))
		return std::unexpected(PVZ::STATUS::FILE_READ_ERROR);

	VS_FIXEDFILEINFO* pInfo = nullptr;
	UINT lenQuery;
	if (!VerQueryValueW(data.data(), L"\\", reinterpret_cast<LPVOID*>(&pInfo), &lenQuery))
		return std::unexpected(PVZ::STATUS::INVALID_VERSION_DATA);


	std::wstring versionStr =
		std::to_wstring(HIWORD(pInfo->dwFileVersionMS)) + L"." +
		std::to_wstring(LOWORD(pInfo->dwFileVersionMS)) + L"." +
		std::to_wstring(HIWORD(pInfo->dwFileVersionLS)) + L"." +
		std::to_wstring(LOWORD(pInfo->dwFileVersionLS));

	for (const auto& Versions : PVZ::Global::g_SupportedGameVersion)
	{
		if (std::wcscmp(Versions, versionStr.c_str()) == 0)
			return true;
	}

	return false;
}
