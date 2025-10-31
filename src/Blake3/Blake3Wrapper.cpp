#include "Blake3Wrapper.h"

#include <format>       // C++20
#include <sstream>
#include <iomanip>

std::expected<std::wstring, PVZ::STATUS> PVZ::Hasher::Blake3Wrapper::GetHashValueW(std::vector<std::uint8_t>& DataToHash)
{
	std::array<std::uint8_t, 32> Bytes = {0};

	if (DataToHash.empty())
		return std::unexpected(PVZ::STATUS::INVALID_VALUE);

	if (!m_CachedHash.empty())
		return m_CachedHash;

	if (!m_Hasher)
		m_Hasher = std::make_unique<blake3_hasher>();

	if (!m_Hasher)
		return std::unexpected(PVZ::STATUS::VALUE_WAS_NULLPTR);

	blake3_hasher_init(m_Hasher.get());
	blake3_hasher_update(m_Hasher.get(), DataToHash.data(), DataToHash.size());
	blake3_hasher_finalize(m_Hasher.get(), Bytes.data(), Bytes.size());

    std::wstring hex;
    hex.reserve(64);  // 32 * 2
    for (std::uint8_t b : Bytes)
        hex += std::format(L"{:02X}", b);
    m_CachedHash = hex;

	//Reset hasher
	m_Hasher.reset();

    return m_CachedHash;
}
