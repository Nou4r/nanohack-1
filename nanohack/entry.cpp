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
//#include "ThemidaSDK/ThemidaSDK.h"
#define VM_EAGLE_BLACK_START
#define VM_EAGLE_BLACK_END
#pragma warning ( disable : 4172 )

#include "core/sdk/utils/string.hpp"
#include "core/sdk/utils/xorstr.hpp"
#include "core/sdk/utils/xorf.hpp"
#include "auth/WinReg.hpp"
#include "auth/Fingerprint.hpp"
#include "auth/api.hpp"

Authentication::api api(xorstr_("plusminus"), xorstr_("92GlzUUazj"), xorstr_("a1510cb312c56fc3ecb23b398988cf2c4e0aefd5b96b2718eee5a559a6635eed"), xorstr_("1.5"));

#include "settings.hpp"
#include "core/sdk/vector.hpp"
#include "core/stdafx.hpp"
#include "core/drawing/renderer.hpp"
#include "core/sdk/utils/hookengine.hpp"
#include "core/sdk/mem.hpp"
#include "core/sdk/utils/crc32.hpp"
#include "core/sdk/il2cpp/wrapper.hpp"
#include "core/sdk/il2cpp/dissector.hpp"
#include "core/sdk/structs.hpp"
#include "core/sdk/game.hpp"
#include "core/core.hpp"
#include "core/main/cache.hpp"
#include "core/main/other.hpp"
#include "core/sdk/utils/math.hpp"
#include "core/main/entities.hpp"
#include  "core/main/config.hpp"
#include "core/drawing/framework.hpp"
#include "core/drawing/d3d.hpp"
#include "core/main/aimutils.hpp"
#include "core/main/hooks.hpp"

// #define authh

void entry_thread( ) {
#ifdef authh

	std::string username = xorstr_("");
	std::string password = xorstr_("");

	std::ifstream save_file(xorstr_("C:\\pml.dat"));
	if (save_file.is_open( )) {
		std::string buf;
		std::getline(save_file, buf);

		std::vector<std::string> splitdat = split(buf, xorstr_(":"));
		if (splitdat.size( ) >= 2) {
			username = splitdat.at(0);
			password = splitdat.at(1);
		}
	}
	save_file.close( );

	api.init( );

	api.login(username, password);

	Sleep(1000);

	settings::auth::days_left = api.days_left;
	settings::auth::username = StringConverter::ToUnicode(username);
#endif


	d3d::init( );
	config::init( );

	/*AllocConsole( );
	SetConsoleTitleA(xorstr_("dbg"));
	freopen_s(reinterpret_cast<FILE**>(stdin), xorstr_("CONIN$"), xorstr_("r"), stdin);
	freopen_s(reinterpret_cast<FILE**>(stdout), xorstr_("CONOUT$"), xorstr_("w"), stdout);*/

	initialize_cheat();
	do_hooks();
}

#ifdef auth
extern "C" __declspec(dllexport) int Chad(DWORD Magic, const char* mutex)
{
	auto RMul = 0x7123A781 * GetCurrentProcessId();
	if (!(RMul % 2)) RMul++;
	Magic *= RMul;
	if ((Magic & 0xFFFF) != (GetCurrentProcessId() & 0xFFFF))
		exit(0);


	HANDLE hMutex = OpenMutexA(SYNCHRONIZE, FALSE, xorstr_("Mf0fbSaxIFaCU8Nsa1ET9bPEd8YR6GnGz0f8i99K"));
	if (!hMutex)
	{
		exit(0);
		return false;
	}

	const auto handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry_thread), 0, 0, nullptr);
	if (handle != NULL)
		CloseHandle(handle);

	return 42069;
}
#endif

bool DllMain(HMODULE hMod, uint32_t call_reason, LPVOID reserved) {
#ifndef auth
	if (call_reason == DLL_PROCESS_ATTACH)
	{
		const auto handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry_thread), 0, 0, nullptr);
		if (handle != NULL)
			CloseHandle(handle);
	}
#endif
	return true;
}