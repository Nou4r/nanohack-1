#define NOMINMAX
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdint.h>
#include <Windows.h>
#include <psapi.h>
#include <d3d11.h>
#include <string>
#include <codecvt>
#include <locale>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <emmintrin.h>
#include <comdef.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <lmcons.h>
#include <thread>
#include <map>

#pragma warning ( disable : 4172 )

#include "core/sdk/utils/string.hpp"
#include "core/sdk/utils/xorstr.hpp"
#include "settings.hpp"
#include "core/sdk/vector.hpp"
#include "core/stdafx.hpp"
#include "core/drawing/renderer.hpp"
#include "core/sdk/utils/hookengine.hpp"
#include "core/sdk/mem.hpp"
#include "core/drawing/menu.hpp"
#include "core/sdk/utils/crc32.hpp"
#include "core/sdk/il2cpp/wrapper.hpp"
#include "core/sdk/il2cpp/dissector.hpp"
#include "core/sdk/structs.hpp"
#include "core/sdk/game.hpp"
#include "core/main/bonecache.hpp"
#include "core/main/other.hpp"
#include "core/sdk/utils/math.hpp"
#include "core/main/players.hpp"
#include "core/drawing/d3d.hpp"
#include "core/main/aimutils.hpp"
#include "core/main/hooks.hpp"

// #define auth

void entry_thread( ) {

	d3d::init( );

	AllocConsole( );
	SetConsoleTitleA(xorstr_("dbg"));
	freopen_s(reinterpret_cast<FILE**>(stdin), xorstr_("CONIN$"), xorstr_("r"), stdin);
	freopen_s(reinterpret_cast<FILE**>(stdout), xorstr_("CONOUT$"), xorstr_("w"), stdout);

	initialize_cheat( );
	do_hooks( );
}

bool DllMain(HMODULE hMod, uint32_t call_reason, void*) {
	if (call_reason != DLL_PROCESS_ATTACH)
		return false;

	const auto handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry_thread), 0, 0, nullptr);
	if (handle != NULL)
		CloseHandle(handle);

	return true;
}