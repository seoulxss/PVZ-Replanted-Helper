#include "VersionChecker.h"
#include <Windows.h>
#include "../Blake3/Blake3Wrapper.h"

#include <Psapi.h>

#include "../Hooks/HookManager.h"
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "version.lib")

inline std::wstring GetModulePath(HMODULE mod)
{
	std::wstring path(MAX_PATH, L'\0');
	DWORD len = K32GetModuleFileNameExW(GetCurrentProcess(), mod, path.data(), MAX_PATH);
	if (len == 0 || len >= MAX_PATH) return {};
	path.resize(len);
	return path;
}

inline std::expected<std::vector<std::uint8_t>, PVZ::STATUS> ReadFileBytes(const std::wstring& path)
{
	HANDLE hFile = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return std::unexpected(PVZ::STATUS::FILE_NOT_FOUND);

	LARGE_INTEGER size;
	if (!GetFileSizeEx(hFile, &size) || size.QuadPart > 200'000'000) // >200 MB?
	{
		CloseHandle(hFile);
		return std::unexpected(PVZ::STATUS::INVALID_VALUE);
	}

	std::vector<std::uint8_t> buffer(size.QuadPart);
	DWORD read = 0;
	if (!ReadFile(hFile, buffer.data(), size.QuadPart, &read, nullptr) || read != size.QuadPart)
	{
		CloseHandle(hFile);
		return std::unexpected(PVZ::STATUS::FILE_READ_ERROR);
	}

	CloseHandle(hFile);
	return buffer;
}

std::expected<bool, PVZ::STATUS> PVZ::Version::IsSupportedVersionEx()
{
	auto winVer = IsSupportedVersionWin();
	if (!winVer.has_value() || !winVer.value())
		return winVer;

	HMODULE mod = GetModuleHandleW(L"GameAssembly.dll");
	if (!mod)
		return std::unexpected(PVZ::STATUS::NOT_FOUND);

	std::wstring path = GetModulePath(mod);
	if (path.empty())
		return std::unexpected(PVZ::STATUS::INVALID_VALUE);

	auto fileData = ReadFileBytes(path);
	if (!fileData.has_value())
		return std::unexpected(fileData.error());

	PVZ::Hasher::Blake3Wrapper hasher;
	auto hash = hasher.GetHashValueW(fileData.value());
	if (!hash.has_value())
		return std::unexpected(hash.error());

	for (const auto& known : PVZ::Global::g_SupportedGameVersionHash)
		if (hash.value() == known)
			return true;

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
