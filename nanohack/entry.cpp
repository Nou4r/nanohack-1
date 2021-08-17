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
#include <random>
#include <iostream>
#include <iomanip>
#include <emmintrin.h>
#include <comdef.h>
#include <iostream>
#include <filesystem>
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
#include <shlobj.h>
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Winmm.lib")
#ifdef THEM
#include "ThemidaSDK/ThemidaSDK.h"
#else
#define VM_TIGER_WHITE_START
#define VM_TIGER_WHITE_END

#define VM_TIGER_RED_START
#define VM_TIGER_RED_END

#define VM_TIGER_BLACK_START
#define VM_TIGER_BLACK_END

#define VM_FISH_WHITE_START
#define VM_FISH_WHITE_END

#define VM_FISH_RED_START
#define VM_FISH_RED_END

#define VM_FISH_BLACK_START
#define VM_FISH_BLACK_END

#define VM_PUMA_WHITE_START
#define VM_PUMA_WHITE_END

#define VM_PUMA_RED_START
#define VM_PUMA_RED_END

#define VM_PUMA_BLACK_START
#define VM_PUMA_BLACK_END

#define VM_SHARK_WHITE_START
#define VM_SHARK_WHITE_END

#define VM_SHARK_RED_START
#define VM_SHARK_RED_END 

#define VM_SHARK_BLACK_START
#define VM_SHARK_BLACK_END

#define VM_DOLPHIN_WHITE_START
#define VM_DOLPHIN_WHITE_END

#define VM_DOLPHIN_RED_START
#define VM_DOLPHIN_RED_END

#define VM_DOLPHIN_BLACK_START
#define VM_DOLPHIN_BLACK_END

#define VM_EAGLE_WHITE_START
#define VM_EAGLE_WHITE_END

#define VM_EAGLE_RED_START
#define VM_EAGLE_RED_END

#define VM_EAGLE_BLACK_START
#define VM_EAGLE_BLACK_END

#define VM_MUTATE_ONLY_START
#define VM_MUTATE_ONLY_END
#endif
#pragma warning ( disable : 4172 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4305 )

#define authh

#include "core/sdk/utils/string.hpp"
#include "core/sdk/utils/xorstr.hpp"
#include "core/sdk/utils/xorf.hpp"
#include "utils/WinReg.hpp"
#include "utils/Fingerprint.hpp"
#include "utils/Cryptography.hpp"
#include <curl/curl.h>
#include "cpr/cpr.h"

#include "settings.hpp"
#include "core/sdk/vector.hpp"
#include "core/stdafx.hpp"
#include "core/drawing/renderer.hpp"

#define FGUI_IMPLEMENTATION
#define FGUI_USE_D2D
#include "FGUI/FGUI.hpp"
#include "core/drawing/fgui/FInput.hpp"
#include "core/drawing/fgui/FRenderer.hpp"
#include "core/drawing/ui.hpp"

#include "core/sdk/utils/hookengine.hpp"
#include "core/sdk/mem.hpp"
#include "core/sdk/utils/crc32.hpp"
#include "core/sdk/il2cpp/wrapper.hpp"
#include "core/sdk/il2cpp/dissector.hpp"
#include "core/sdk/structs.hpp"
#include "core/sdk/game.hpp"
#include "core/main/cache.hpp"
#include "core/main/other.hpp"
#include "core/sdk/utils/math.hpp"
#include "core/main/entities.hpp"
#include "core/drawing/d3d.hpp"
#include "core/main/aimutils.hpp"
#include "core/main/hooks.hpp"

class LoginResult {
public:
	static inline std::string OK = xorstr_("OK");
	static inline std::string INVALID_REQUEST = xorstr_("ERR1");
	static inline std::string INVALID_USERPASS = xorstr_("ERR2");
	static inline std::string INTERNAL_ERROR = xorstr_("ERR3");
	static inline std::string LICENSE_EXPIRED = xorstr_("ERR4");
	static inline std::string HWID_CHANGED = xorstr_("ERR5");
};

std::vector<std::string> split_string(const std::string& str,
	const std::string& delimiter)
{
	std::vector<std::string> strings;

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = str.find(delimiter, prev)) != std::string::npos)
	{
		strings.push_back(str.substr(prev, pos - prev));
		prev = pos + 1;
	}

	strings.push_back(str.substr(prev));

	return strings;
}

void entry_thread() {
	PWSTR szPath = NULL;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &szPath)))
	{
		std::filesystem::create_directories(StringConverter::ToASCII(std::wstring(szPath) + wxorstr_(L"\\plusminus")));

#ifdef authh
		std::string username = xorstr_("");
		std::string password = xorstr_("");

		std::filesystem::create_directories(StringConverter::ToASCII(std::wstring(szPath) + wxorstr_(L"\\plusminus")));

		std::ifstream authFile(StringConverter::ToASCII(std::wstring(szPath) + wxorstr_(L"\\plusminus\\auth.bin")).c_str());


		if (authFile.fail())
		{
			for (;;)
			{
				exit(0);
				quick_exit(0);
				ExitProcess(0);
				_Exit(0);
				_exit(0);
			}
			return;
		}
		else
		{
			std::string fileContent((std::istreambuf_iterator<char>(authFile)), (std::istreambuf_iterator<char>()));
			fileContent = Cryptography::DecryptData(fileContent, Cryptography::SHA512(xorstr_("SgZ7QlO6dQfyJwU76A4xTSvJunLmUWk0")).substr(0, 32), xorstr_("198r8ZoZ0EGtM29y"));
			auto data = split_string(fileContent, xorstr_("\n"));
			username = Cryptography::Base64Decode(data.at(0));
			password = Cryptography::Base64Decode(data.at(1));
		}

		SystemFingerprint* Fingerprint = SystemFingerprint::CreateUniqueFingerprint();

		cpr::Response encResponse = cpr::Post(cpr::Url{ xorstr_("https://auth.plusminus.vip/whitelist/login/") },
			cpr::Header
			{
				{xorstr_("S"), Cryptography::SHA512(xorstr_("O1KTlL9VfxmRnYxr") + Fingerprint->ToString() + Cryptography::Base64Encode(username) + Cryptography::Base64Encode(password)).c_str() }
			},
			cpr::Payload
			{
				{xorstr_("1"), Cryptography::Base64Encode(username) },
				{xorstr_("2"), Cryptography::Base64Encode(password) },
				{xorstr_("3"), Fingerprint->ToString() }
			},
			cpr::Ssl(
				cpr::ssl::PinnedPublicKey{ xorstr_("sha256//E1R0iKEP0iGOxvYAmnMcF7KS0+74zLzU5wymBEfoy/Y=") },
				cpr::ssl::VerifyHost(true),
				cpr::ssl::VerifyPeer(true)
			)
		);

		std::string decryptedResponse = Cryptography::DecryptData(encResponse.text, Cryptography::SHA512(xorstr_("SgZ7QlO6dQfyJwU76A4xTSvJunLmUWk0")).substr(0, 32), xorstr_("198r8ZoZ0EGtM29y"));
		if (strcmp(decryptedResponse.c_str(), LoginResult::OK.c_str()) != 0)
		{
			exit(0);
			return;
		}

		settings::auth::username = StringConverter::ToUnicode(username);

		encResponse = cpr::Post(cpr::Url{ xorstr_("https://auth.plusminus.vip/whitelist/expiry/") },
			cpr::Header
			{
				{xorstr_("S"), Cryptography::SHA512(xorstr_("O1KTlL9VfxmRnYxr") + Fingerprint->ToString() + Cryptography::Base64Encode(username) + Cryptography::Base64Encode(password)).c_str() }
			},
			cpr::Payload
			{
				{xorstr_("1"), Cryptography::Base64Encode(username) },
				{xorstr_("2"), Cryptography::Base64Encode(password) },
				{xorstr_("3"), Fingerprint->ToString() }
			},
			cpr::Ssl(
				cpr::ssl::PinnedPublicKey{ xorstr_("sha256//E1R0iKEP0iGOxvYAmnMcF7KS0+74zLzU5wymBEfoy/Y=") },
				cpr::ssl::VerifyHost(true),
				cpr::ssl::VerifyPeer(true)
			)
		);

		settings::auth::days_left = StringConverter::ToUnicode(Cryptography::DecryptData(encResponse.text, Cryptography::SHA512(xorstr_("SgZ7QlO6dQfyJwU76A4xTSvJunLmUWk0")).substr(0, 32), xorstr_("198r8ZoZ0EGtM29y")));

#endif
	}
	d3d::init( );

	
	/*AllocConsole( );
	SetConsoleTitleA(xorstr_("dbg"));
	freopen_s(reinterpret_cast<FILE**>(stdin), xorstr_("CONIN$"), xorstr_("r"), stdin);
	freopen_s(reinterpret_cast<FILE**>(stdout), xorstr_("CONOUT$"), xorstr_("w"), stdout);*/
	

	initialize_cheat();
	do_hooks();
}

extern "C" __declspec(dllexport) int Gamer()
{
	MessageBoxA(0, "Success", "gamer", 0);
	return 1337;
}

bool DllMain(HMODULE hMod, uint32_t call_reason, LPVOID reserved) {
	if (call_reason == DLL_PROCESS_ATTACH)
	{
		const auto handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry_thread), 0, 0, nullptr);
		if (handle != NULL)
			CloseHandle(handle);
	}
	return true;
}