#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
bool menu_init = false;

void undo_hooks( );

namespace d3d {

	HRESULT present_hook(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
		static Vector2 text_size = Vector2(0, 0);
		if (!device) {
			swapChain->GetDevice(__uuidof(device), reinterpret_cast<PVOID*>(&device));
			device->GetImmediateContext(&immediate_context);

			ID3D11Texture2D* renderTarget = nullptr;
			swapChain->GetBuffer(0, __uuidof(renderTarget), reinterpret_cast<PVOID*>(&renderTarget));
			device->CreateRenderTargetView(renderTarget, nullptr, &render_target_view);
			renderTarget->Release( );

			Renderer::Init(swapChain);
			FGUI_INPUT_WIN32::OnEntryPoint();
			FGUI_D3D11::OnEntryPoint();

		}
		immediate_context->OMSetRenderTargets(1, &render_target_view, nullptr);
		immediate_context->RSGetViewports(&vps, &viewport);
		screen_size = { viewport.Width, viewport.Height };
		screen_center = { viewport.Width / 2.0f, viewport.Height / 2.0f };

		//if (GetAsyncKeyState(VK_INSERT) & 1)
		//	settings::menu = !settings::menu;

		if (!settings::panic) {
			if (Renderer::new_frame(swapChain)) {
				Renderer::text(Vector2(5, 4), Color3(255, 255, 255), 14.f, false, true, wxorstr_(L"plusminus | %s | expires: %s"), settings::auth::username.c_str(), settings::auth::days_left.c_str());

				if (!plusminus::ui::init)
					plusminus::ui::OnSetupDevice();

				plusminus::ui::vars::Container->Render();

				if (settings::cheat_init)
					entities::loop( );

				if (plusminus::ui::get_bool(xorstr_("draw targeting fov")))
					Renderer::circle(screen_center, plusminus::ui::get_color(xorstr_("targeting fov color")), plusminus::ui::get_float(xorstr_("targeting fov")), 1.f);

				Renderer::end_frame( );
			}
		}
		else {
			static bool once = false;
			if (!once) {
				if(LocalPlayer::Entity())
					LocalPlayer::Entity()->playerFlags() |= ~PlayerFlags::IsAdmin;

				undo_hooks( );
				
				once = true;
			}
		}

		return present_original(swapChain, syncInterval, flags);
	}

	HRESULT resize_hook(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
		Renderer::reset_canvas( );

		if (render_target_view)
			render_target_view->Release( );

		if (immediate_context)
			immediate_context->Release( );

		if (device)
			device->Release( );

		device = nullptr;

		return resize_original(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
	}

	bool init( ) {
		unity_player = (uintptr_t)GetModuleHandleA(xorstr_("UnityPlayer.dll"));
		game_assembly = (uintptr_t)GetModuleHandleA(xorstr_("GameAssembly.dll"));

		auto addr = mem::find_pattern(unity_player, (PBYTE)"\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x48\x8B\x80\xA0\x03\x00\x00", xorstr_("xxxxx????xxxxxxx"));

		if (!addr)
			return false;

		auto swapchain = reinterpret_cast<IDXGISwapChain * (__fastcall*)()>(addr)();

		if (swapchain) {
			auto table = *reinterpret_cast<PVOID**>(swapchain);
			present_original = reinterpret_cast<HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT)>(table[ 8 ]);
			resize_original = reinterpret_cast<HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT)>(table[ 13 ]);

			hookengine::hook(present_original, present_hook);
			hookengine::hook(resize_original, resize_hook);

			return true;
		}
		return false;
	}
}