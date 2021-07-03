#define NOMINMAX

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
#include <thread>
#include <map>

#pragma warning ( disable : 4172 )

#include "xorstr.hpp"
#include "settings.hpp"
#include "vector.hpp"
#include "stdafx.hpp"
#include "renderer.hpp"
#include "hookengine.hpp"
#include "mem.hpp"
#include "menu.hpp"
#include "crc32.hpp"
#include "wrapper.hpp"
#include "dissector.hpp"
#include "structs.hpp"
#include "game.hpp"
#include "bonecache.hpp"
#include "other.hpp"
#include "math.hpp"
#include "players.hpp"
#include "d3d.hpp"
#include "aimutils.hpp"
#include "hooks.hpp"

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