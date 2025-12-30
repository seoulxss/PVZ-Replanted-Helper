#include "VersionChecker.h"
#include <Windows.h>
#include "../Blake3/Blake3Wrapper.h"

#include <Psapi.h>

#include "../Hooks/HookManager.h"
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "version.lib")