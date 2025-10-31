#pragma once
#include "../Globals.h"

namespace PVZ::Version
{
	//! This is a better check if the version is supported
	//! @return True if the version is supported
	std::expected<bool, PVZ::STATUS> IsSupportedVersionEx();

	//! Checks the .exe Version number
	//! @return True if the version is supported
	std::expected<bool, PVZ::STATUS> IsSupportedVersionWin();
}