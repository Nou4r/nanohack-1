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

	/*AllocConsole( );
	SetConsoleTitleA(xorstr_("dbg"));
	freopen_s(reinterpret_cast<FILE**>(stdin), xorstr_("CONIN$"), xorstr_("r"), stdin);
	freopen_s(reinterpret_cast<FILE**>(stdout), xorstr_("CONOUT$"), xorstr_("w"), stdout);*/


	initialize_cheat( );
	do_hooks( );
	while (true) {
		if (settings::chams) {
			auto playerList = BasePlayer::visiblePlayerList( );
			if (playerList) {
				for (int i = 0; i < playerList->vals->size; i++) {
					auto player = *reinterpret_cast<BasePlayer**>(std::uint64_t(playerList->vals->buffer) + (0x20 + (sizeof(void*) * i)));

					if (!player) continue;
					if (!player->IsValid( )) continue;
					if (!player->isCached( )) continue;
					if (player->health( ) <= 0.0f) continue;
					if (player->HasPlayerFlag(PlayerFlags::Sleeping)) continue;
					if (player->playerModel( )->isNpc( ) && !settings::npcs) continue;
					if (player->userID( ) == LocalPlayer::Entity( )->userID( )) continue;

					//assets/shaders/chams.shader
					//assets/shaders/chamslit.shader

					//static auto	bundle = AssetBundle::LoadFromFile(xorstr_("C:/Users/yty/Desktop/EscapeFromTarkov-Trainer-master/EscapeFromTarkov-Trainer-master/Files/outline"));
					//static auto shader_from_bundle = (Shader*)bundle->LoadAsset(xorstr_("assets/outline.shader"), Type::Shader( ));
					//if (shader_from_bundle && bundle) {
					auto renderer_list = player->playerModel( )->_multiMesh( )->Renderers( );
					if (renderer_list) {
						for (int j = 0; j < renderer_list->size; j++) {
							auto renderer = (Renderer_*)renderer_list->get(j);
							if (!renderer)
								continue;

							auto material = renderer->material( );
							if (!material)
								continue;

							auto shader = material->shader( );
							if (!shader)
								continue;

							if (shader == nullptr)
								continue;

							material->set_shader(nullptr);

							//material->SetColor(xorstr_("_FirstOutlineColor"), Color::yellow( ));
							//material->SetFloat(xorstr_("_FirstOutlineWidth"), 0.02f + settings::test1);
							//material->SetColor(xorstr_("_SecondOutlineColor"), Color::red( ));
							//material->SetFloat(xorstr_("_SecondOutlineWidth"), 0.0025f + settings::test2);
						}
					}
					//}
				}
			}
		}
	}
}

bool DllMain(HMODULE hMod, uint32_t call_reason, void*) {
	if (call_reason != DLL_PROCESS_ATTACH)
		return false;

	const auto handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry_thread), 0, 0, nullptr);
	if (handle != NULL)
		CloseHandle(handle);

	return true;
}