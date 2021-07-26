POINT cursor;
POINT cursor_corrected;

std::string_view keys_list[ ]{
				"Error", "Left Mouse", "Right Mouse", "Break", "Middle Mouse", "Mouse 4", "Mouse 5",
				"Error", "Backspace", "TAB", "Error", "Error", "Error", "ENTER", "Error", "Error", "SHIFT",
				"CTRL", "ALT", "PAUSE", "CAPS LOCK", "Error", "Error", "Error", "Error", "Error", "Error",
				"Error", "Error", "Error", "Error", "Error", "SPACEBAR", "PG UP", "PG DOWN", "END", "HOME", "Left",
				"Up", "Right", "Down", "Error", "Print", "Error", "Print Screen", "Insert", "Delete", "Error", "0", "1",
				"2", "3", "4", "5", "6", "7", "8", "9", "Error", "Error", "Error", "Error", "Error", "Error",
				"Error", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
				"V", "W", "X", "Y", "Z", "Left Windows", "Right Windows", "Error", "Error", "Error", "NUM 0", "NUM 1",
				"NUM 2", "NUM 3", "NUM 4", "NUM 5", "NUM 6", "NUM 7", "NUM 8", "NUM 9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3",
				"F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21",
				"F22", "F23", "F24", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
				"NUM LOCK", "SCROLL LOCK", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
				"Error", "Error", "Error", "Error", "Error", "Error", "Error", "LSHIFT", "RSHIFT", "LCONTROL",
				"RCONTROL", "LMENU", "RMENU", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
				"Error", "Error", "Error", "Next Track", "Previous Track", "Stop", "Play/Pause", "Error", "Error",
				"Error", "Error", "Error", "Error", ";", "+", ",", "-", ".", "/?", "~", "Error", "Error",
				"Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
				"Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
				"Error", "Error", "Error", "Error", "Error", "Error", "[{", "\\|", "}]", "'\"", "Error",
				"Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
				"Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
				"Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
				"Error", "Error"
};

namespace menu_framework {

#define THEME_COLOR Color3(255, 173, 31)

	namespace variables {
		inline int x = 140, y = 140;
		inline int w = 400, h = 300;
	}

	inline int current_tab{ 0 };

	inline bool should_drag = false;
	inline bool should_move = false;

	void group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, const std::string string, bool show_label) {
		Renderer::rectangle_filled(Vector2(x, y), Vector2(w, h), Color3(25, 25, 25, 255));

		Renderer::rectangle(Vector2(x, y), Vector2(w, h), Color3(45, 45, 45, 255));

		if (show_label)
			Renderer::text(Vector2(x + 2, y - 12), Color3(255, 255, 255, 255), 12.f, false, false, StringConverter::ToUnicode(string));
	}

	void tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, const std::string string, std::int32_t& tab, std::int32_t count, bool show_outline) {
		GetCursorPos(&cursor);

		if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h) && (GetAsyncKeyState(VK_LBUTTON) & 1))
			tab = count;

		if (show_outline)
			Renderer::rectangle(Vector2(x, y), Vector2(w, h), tab == count ? THEME_COLOR : Color3(25, 25, 25, 255));

		Renderer::text(Vector2(x - Renderer::get_text_size(StringConverter::ToUnicode(string), 12.f).x / 2 + 50, y + h / 2 - 8), show_outline ? Color3(255, 255, 255, 255) : tab == count ? THEME_COLOR : Color3(255, 255, 255, 255), 12.f, false, false, StringConverter::ToUnicode(string));
	}

	void checkbox(std::int32_t x, std::int32_t y, std::int32_t position, const std::string string, bool& value) {
		GetCursorPos(&cursor);

		int w = 10, h = 10;

		if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_LBUTTON) & 1)
			value = !value;

		Renderer::rectangle_filled(Vector2(position, y), Vector2(w, h), value ? THEME_COLOR : Color3(36, 36, 36, 255));

		Renderer::text(Vector2(x + 2, y - 1), Color3(255, 255, 255, 255), 12.f, false, false, StringConverter::ToUnicode(string));
	}

	void keybind(std::int32_t x, std::int32_t y, std::int32_t position, const std::string string, int& value) {
		GetCursorPos(&cursor);

		int w = 40, h = 10;

		if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h)) {
			for (int i = 0; i < 256; i++) {
				if (GetAsyncKeyState(i) && keys_list[ i ] != "Error") {
					value = i;
					break;
				}
			}
		}

		Renderer::rectangle_filled(Vector2(position, y), Vector2(w, h), THEME_COLOR);

		Renderer::text(Vector2(x + 2, y - 1), Color3(255, 255, 255), 12.f, false, false, StringConverter::ToUnicode(string + std::string(": ") + keys_list[ value ].data( )));
	}

	void slider(std::int32_t x, std::int32_t y, std::int32_t position, const std::string string, float& value, float min_value, float max_value) {
		GetCursorPos(&cursor);

		int ix = x + 140;
		int yi = y + 4;

		if ((cursor.x > ix) && (cursor.x < ix + position) && (cursor.y > yi) && (cursor.y < yi + 6) && (GetAsyncKeyState(VK_LBUTTON)))
			value = (cursor.x - ix) / (float(position) / float(max_value));

		Renderer::rectangle_filled(Vector2(ix, yi), Vector2(position, 6), Color3(36, 36, 36, 255));
		Renderer::rectangle_filled(Vector2(ix, yi), Vector2(value * (float(position) / float(max_value)), 6), THEME_COLOR);

		Renderer::text(Vector2(x + 2, y - 1), Color3(255, 255, 255), 12.f, false, false, StringConverter::ToUnicode((std::stringstream{ } << string << ": " << std::setprecision(3) << value).str( )));
	}

	void menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
		GetCursorPos(&cursor);

		if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
			should_drag = true;

			if (!should_move) {
				cursor_corrected.x = cursor.x - x;
				cursor_corrected.y = cursor.y - y;
				should_move = true;
			}
		}

		if (should_drag) {
			x = cursor.x - cursor_corrected.x;
			y = cursor.y - cursor_corrected.y;
		}

		if (GetAsyncKeyState(VK_LBUTTON) == 0) {
			should_drag = false;
			should_move = false;
		}
	}
	auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, Color3 bg, Color3 header_text, Color3 header_line, const std::string& name) {
		Renderer::rectangle_filled(Vector2(x, y), Vector2(w, h), bg);
		Renderer::rectangle_filled(Vector2(x, y), Vector2(w, 30), header_text);
		Renderer::rectangle_filled(Vector2(x, y + 30), Vector2(w, 2), header_line);
		Renderer::text(Vector2(x + 10, y + 8), Color3(255, 255, 255), 12.f, false, false, StringConverter::ToUnicode(name));
	};
	void render( ) {
		if (!settings::menu)
			return;

		do_frame(variables::x, variables::y, variables::w, variables::h, Color3(36, 36, 36, 255), Color3(25, 25, 25, 255), Color3(36, 36, 36, 255), "plusminus");

		menu_framework::group_box(variables::x + 5, variables::y + 35, 100, 260, "tabs", false); {
			menu_framework::tab(variables::x + 5, variables::y + (260 / 2) - 35, 100, 30, "combat", current_tab, 0, false);
			menu_framework::tab(variables::x + 5, variables::y + (260 / 2), 100, 30, "visuals", current_tab, 1, false);
			menu_framework::tab(variables::x + 5, variables::y + (260 / 2) + 35, 100, 30, "misc", current_tab, 2, false);
		}

		switch (current_tab) {
		case 0:
			menu_framework::group_box(variables::x + 110, variables::y + 35, 285, 260, "combat", false); {
				int combat_y = 45;

				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, "psilent", settings::psilent); combat_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, "manipulator", settings::manipulator); combat_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, "desync", settings::desync); combat_y += 15;

				if (settings::desync) {
					static float offset = 15;
					menu_framework::keybind(variables::x + 165 + offset, variables::y + combat_y, variables::x + 120 + offset, "desync bind", settings::desync_key); combat_y += 15;
				}

				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, "autoshoot", settings::autoshoot); combat_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, "pierce", settings::penetrate); combat_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, "big bullets", settings::bigger_bullets); combat_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, "fast bullets", settings::faster_bullets); combat_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, "insta eoka", settings::always_eoka); combat_y += 15;
			}
			break;
		case 1:
			menu_framework::group_box(variables::x + 110, variables::y + 35, 285, 260, "visuals", false); {

			}
			break;
		case 2:
			menu_framework::group_box(variables::x + 110, variables::y + 35, 285, 260, "misc", false); {
				int misc_y = 45;

				menu_framework::slider(variables::x + 120, variables::y + misc_y, 125, "camera fov", settings::camera_fov, 30.f, 160.f); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, "bullet tracers", settings::bullet_tracers); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, "fast loot", settings::fastloot); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, "fake shots", settings::weapon_spam); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, "no jumping restrictions", settings::infinite_jump); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, "no attack restrictions", settings::freeaim); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, "no sprinting restrictions", settings::omnisprint); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, "no fall damage", settings::nofall); misc_y += 15;
			}
			break;
		}

		menu_framework::menu_movement(variables::x, variables::y, variables::w, 30);
	}
}