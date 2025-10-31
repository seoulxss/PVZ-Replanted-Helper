#pragma once
#include "../Globals.h"
#include <blake3.h>

namespace PVZ::Hasher
{
	class Blake3Wrapper
	{
	public:
		Blake3Wrapper() = default;

		std::expected<std::wstring, PVZ::STATUS> GetHashValueW(std::vector<std::uint8_t>& DataToHash);


	private:
		std::wstring m_CachedHash;
		std::unique_ptr<blake3_hasher> m_Hasher = nullptr;
	};
}
