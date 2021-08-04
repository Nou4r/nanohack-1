POINT cursor;
POINT cursor_corrected;

std::string_view keys_list[ ]{
				"press a key", "Left Mouse", "Right Mouse", "Break", "Middle Mouse", "Mouse 4", "Mouse 5",
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
using namespace settings;
namespace menu_framework {

#define THEME_COLOR Color3(255, 173, 31)


	int current_tab{ 0 };

	bool should_drag = false;
	bool should_move = false;

	void group_box(float x, float y, float w, float h, std::string string, bool show_label) {
		Renderer::rectangle_filled(Vector2(x, y), Vector2(w, h), Color3(25, 25, 25, 255));

		Renderer::rectangle(Vector2(x, y), Vector2(w, h), Color3(45, 45, 45, 255));

		if (show_label)
			Renderer::text(Vector2(variables::x + variables::w - 50, variables::y + 8), Color3(255, 255, 255, 255), 12.f, false, false, StringConverter::ToUnicode(string));
	}

	void tab(float x, float y, float w, float h, std::string string, int& tab, float count, bool show_outline) {
		GetCursorPos(&cursor);

		if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h) && (GetAsyncKeyState(VK_LBUTTON) & 1))
			tab = count;

		if (show_outline)
			Renderer::rectangle(Vector2(x, y), Vector2(w, h), tab == count ? THEME_COLOR : Color3(25, 25, 25, 255));

		Renderer::text(Vector2(x - Renderer::get_text_size(StringConverter::ToUnicode(string), 12.f).x / 2 + 50, y + h / 2 - 8), show_outline ? Color3(255, 255, 255, 255) : tab == count ? THEME_COLOR : Color3(255, 255, 255, 255), 12.f, false, false, StringConverter::ToUnicode(string));
	}

	void selector(float x, float y, float position, std::string string, int& value, std::vector<std::string> vals) {
		GetCursorPos(&cursor);

		// for selectors, l and r
		int w = 10, h = 10;

		Vector2 str_size = Renderer::get_text_size(StringConverter::ToUnicode(string + std::string(": ") + vals[ value ]), 12.f);

		// selector l
		Renderer::line(Vector2(position, y + h / 2), Vector2(position + w, y + 1), Color3(255, 255, 255), false, 1.f);
		Renderer::line(Vector2(position, y + h / 2), Vector2(position + w, y + h - 1), Color3(255, 255, 255), false, 1.f);

		// selector r
		
		Renderer::line(Vector2(position + str_size.x + 23 + w, y + h / 2), Vector2(position + str_size.x + 23, y + 1), Color3(255, 255, 255), false, 1.f);
		Renderer::line(Vector2(position + str_size.x + 23 + w, y + h / 2), Vector2(position + str_size.x + 23, y + h - 1), Color3(255, 255, 255), false, 1.f);

		if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_LBUTTON) & 1) {
			if (value > 0)
				value -= 1;
		}
		else if ((cursor.x > position + str_size.x + 23) && (cursor.x < position + w + str_size.x + 23) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_LBUTTON) & 1) {
			if (value < vals.size( ) - 1)
				value += 1;
		}
				

		Renderer::text(Vector2(x + 2, y - 3), Color3(255, 255, 255, 255), 12.f, false, false, StringConverter::ToUnicode(string + std::string(": ") + vals[ value ]));
	}

	void checkbox(float x, float y, float position, std::string string, bool& value, std::string tooltip = xorstr_("")) {
		GetCursorPos(&cursor);

		int w = 10, h = 10;

		if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h)) { // hovered
			if (GetAsyncKeyState(VK_LBUTTON) & 1)
				value = !value;

			if (!tooltip.empty( )) {
				Renderer::text({ screen_center.x, 10 }, Color3(255, 255, 255), 12.f, true, true, StringConverter::ToUnicode(tooltip));
			}
		}
			

		Renderer::rectangle_filled(Vector2(position, y), Vector2(w, h), value ? THEME_COLOR : Color3(36, 36, 36, 255));
		 
		Renderer::text(Vector2(x + 2, y - 3), Color3(255, 255, 255, 255), 12.f, false, false, StringConverter::ToUnicode(string));
	}

	bool button(float x, float y, float position, std::string string) {
		GetCursorPos(&cursor);

		Vector2 str_size = Renderer::get_text_size(StringConverter::ToUnicode(string), 12.f);

		int w = str_size.x + 20, h = 20;

		Renderer::rectangle_filled(Vector2(position, y), Vector2(w, h), Color3(36, 36, 36, 255));
		Renderer::rectangle(Vector2(position, y), Vector2(w, h), Color3(36, 36, 36, 255), 0.7f);

		Renderer::text(Vector2(position + w / 2, y + 10), Color3(255, 255, 255, 255), 12.f, true, false, StringConverter::ToUnicode(string));

		return ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_LBUTTON) & 1);
	}

	void keybind(float x, float y, float position, std::string string, int& value) {
		GetCursorPos(&cursor);

		int w = 40, h = 10;

		if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_LBUTTON) & 1) {
			value = 0;
		}

		if (value == 0) {
			for (int i = 0; i < 256; i++) {
				if (GetAsyncKeyState(i) && keys_list[ i ] != "Error" && keys_list[ i ] != "press a key" && keys_list[ i ] != "Left Mouse") {
					value = i;
					break;
				}
			}
		}

		Renderer::rectangle_filled(Vector2(position, y), Vector2(w, h), value == 0 ? Color3(36, 36, 36, 255) : THEME_COLOR);

		Renderer::text(Vector2(x + 2, y - 3), Color3(255, 255, 255), 12.f, false, false, StringConverter::ToUnicode(string + std::string(": ") + keys_list[ value ].data( )));
	}

	void slider(float x, float y, float position, std::string string, float& value, float min_value, float max_value) {
		GetCursorPos(&cursor);

		int ix = x + 140;
		int yi = y + 2;

		if ((cursor.x > ix - 0.80f) && (cursor.x < ix + position + 0.80f) && (cursor.y > yi) && (cursor.y < yi + 6) && (GetAsyncKeyState(VK_LBUTTON)))
			value = (cursor.x - ix) / (float(position) / float(max_value));

		Renderer::rectangle_filled(Vector2(ix, yi), Vector2(position, 6), Color3(36, 36, 36, 255));
		Renderer::rectangle_filled(Vector2(ix, yi), Vector2(value * (float(position) / float(max_value)), 6), THEME_COLOR);

		Renderer::text(Vector2(x + 2, y - 3), Color3(255, 255, 255), 12.f, false, false, wxorstr_(L"%s: %.2f"), StringConverter::ToUnicode(string).c_str(), value);
	}

	void menu_movement(int& x, int& y, int w, int h) {
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
	auto do_frame = [&](float x, float y, float w, float h, Color3 bg, Color3 header_text, Color3 header_line, std::string name) {
		Renderer::rectangle_filled(Vector2(x, y), Vector2(w, h), bg);
		Renderer::rectangle_filled(Vector2(x, y), Vector2(w, 30), header_text);
		Renderer::rectangle_filled(Vector2(x, y + 30), Vector2(w, 2), header_line);
		Renderer::text(Vector2(x + 10, y + 8), Color3(255, 255, 255), 12.f, false, false, StringConverter::ToUnicode(name));
	};

	void render( ) {
		Renderer::text(Vector2(5, 4), Color3(255, 255, 255), 14.f, false, true, wxorstr_(L"plusminus | %s | days left: %d"), settings::auth::username.c_str(), settings::auth::days_left);

		if (!settings::menu)
			return;

		do_frame(variables::x, variables::y, variables::w, variables::h, Color3(36, 36, 36, 255), Color3(25, 25, 25, 255), Color3(36, 36, 36, 255), xorstr_("plusminus"));

		menu_framework::group_box(variables::x + 5, variables::y + 35, 100, 260, xorstr_("tabs"), false); {
			menu_framework::tab(variables::x + 5, variables::y + (260 / 2) - 70, 100, 30, xorstr_("combat"), current_tab, 0, false);
			menu_framework::tab(variables::x + 5, variables::y + (260 / 2) - 35, 100, 30, xorstr_("weapon"), current_tab, 1, false);
			menu_framework::tab(variables::x + 5, variables::y + (260 / 2), 100, 30, xorstr_("visuals"), current_tab, 2, false);
			menu_framework::tab(variables::x + 5, variables::y + (260 / 2) + 35, 100, 30, xorstr_("misc"), current_tab, 3, false);
			menu_framework::tab(variables::x + 5, variables::y + (260 / 2) + 70, 100, 30, xorstr_("other"), current_tab, 4, false);
		}

		switch (current_tab) {
		case 0:
			menu_framework::group_box(variables::x + 110, variables::y + 35, 285, 260, xorstr_("combat"), true); {
				int combat_y = 45;

				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, xorstr_("psilent"), settings::psilent, xorstr_("makes bullets fly towards the targeted player without directly aiming at them.")); combat_y += 15;
				menu_framework::selector(variables::x + 135, variables::y + combat_y, variables::x + 120, xorstr_("hitbox override"), settings::h_override, { xorstr_("none"), xorstr_("body"), xorstr_("head"), xorstr_("randomize (all)"), xorstr_("randomize (main)") }); combat_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, xorstr_("manipulator"), settings::manipulator, xorstr_("automatically finds the best angle for you to hit a player and shoots for you.")); combat_y += 15;

				if (settings::manipulator) {
					static float offset = 15;
					menu_framework::keybind(variables::x + 165 + offset, variables::y + combat_y, variables::x + 120 + offset, xorstr_("manipulator bind"), settings::manipulate_key); combat_y += 15;
				}

				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, xorstr_("autoshoot"), settings::autoshoot, xorstr_("automatically shoots at the targeted player when they're visible.")); combat_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, xorstr_("pierce"), settings::penetrate, xorstr_("lets you shoot through random entities like trees, ores, cargo ship, monuments, crates, etc..")); combat_y += 15; combat_y += 15;

				menu_framework::checkbox(variables::x + 135, variables::y + combat_y, variables::x + 120, xorstr_("draw targeting fov"), settings::draw_fov); combat_y += 15;
				menu_framework::slider(variables::x + 120, variables::y + combat_y, 125, xorstr_("targeting fov"), settings::targeting_fov, 30.f, 2500.f); combat_y += 15;
			}
			break;
		case 1:
			menu_framework::group_box(variables::x + 110, variables::y + 35, 285, 260, xorstr_("weapon"), true); {
				int weapon_y = 45;

				menu_framework::slider(variables::x + 120, variables::y + weapon_y, 125, xorstr_("recoil %"), settings::recoil_p, 0.f, 100.f); weapon_y += 15;
				menu_framework::slider(variables::x + 120, variables::y + weapon_y, 125, xorstr_("spread %"), settings::spread_p, 0.f, 100.f); weapon_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + weapon_y, variables::x + 120, xorstr_("no sway"), settings::nosway); weapon_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + weapon_y, variables::x + 120, xorstr_("big bullets"), settings::bigger_bullets); weapon_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + weapon_y, variables::x + 120, xorstr_("fast bullets"), settings::faster_bullets); weapon_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + weapon_y, variables::x + 120, xorstr_("insta eoka"), settings::always_eoka); weapon_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + weapon_y, variables::x + 120, xorstr_("automatic"), settings::automatic); weapon_y += 15;
			}
			break;
		case 2:
			menu_framework::group_box(variables::x + 110, variables::y + 35, 285, 260, xorstr_("visuals"), true); {
				int visuals_y = 45;

				menu_framework::checkbox(variables::x + 135, variables::y + visuals_y, variables::x + 120, xorstr_("players"), settings::players); visuals_y += 15;
				if (settings::players) {
					static float offset = 15;
					menu_framework::checkbox(variables::x + 135 + offset, variables::y + visuals_y, variables::x + 120 + offset, xorstr_("npc"), settings::npcs); visuals_y += 15;
					menu_framework::checkbox(variables::x + 135 + offset, variables::y + visuals_y, variables::x + 120 + offset, xorstr_("looking direction"), settings::look_dir); visuals_y += 15;
					menu_framework::checkbox(variables::x + 135 + offset, variables::y + visuals_y, variables::x + 120 + offset, xorstr_("skeleton"), settings::skeleton); visuals_y += 15;
					menu_framework::checkbox(variables::x + 135 + offset, variables::y + visuals_y, variables::x + 120 + offset, xorstr_("box"), settings::box); visuals_y += 15;

					if (settings::box) {
						static float offset2 = 30;
						menu_framework::selector(variables::x + 135 + offset2, variables::y + visuals_y, variables::x + 120 + offset2, xorstr_("box type"), settings::box_type, { xorstr_("cornered"), xorstr_("full"), xorstr_("3 dimension") }); visuals_y += 15;
					}
				}
				menu_framework::checkbox(variables::x + 135, variables::y + visuals_y, variables::x + 120, xorstr_("reload indicator"), settings::reload_indicator, xorstr_("draws your reloading progress.")); visuals_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + visuals_y, variables::x + 120, xorstr_("target player belt"), settings::belt); visuals_y += 15;
			}
			break;
		case 3:
			menu_framework::group_box(variables::x + 110, variables::y + 35, 285, 260, xorstr_("misc"), true); {
				int misc_y = 45;

				menu_framework::slider(variables::x + 120, variables::y + misc_y, 125, xorstr_("camera fov"), settings::camera_fov, 30.f, 160.f); misc_y += 15;
				menu_framework::keybind(variables::x + 165, variables::y + misc_y, variables::x + 120, xorstr_("zoom bind"), settings::zoom_key); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, xorstr_("bullet tracers"), settings::bullet_tracers); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, xorstr_("fake admin"), settings::fakeadmin); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, xorstr_("fast loot"), settings::fastloot); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, xorstr_("farm assist"), settings::farm_assist, xorstr_("will automatically hit tree markers and ore hot spots when you hit the ore / tree.")); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, xorstr_("walk on water"), settings::walkonwater); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, xorstr_("fake shots"), settings::weapon_spam); misc_y += 15;

				if (settings::weapon_spam) {
					static float offset = 15;
					menu_framework::keybind(variables::x + 165 + offset, variables::y + misc_y, variables::x + 120 + offset, xorstr_("fake shots bind"), settings::weapon_spam_key); misc_y += 15;
				}

				menu_framework::selector(variables::x + 135, variables::y + misc_y, variables::x + 120, xorstr_("lightning"), settings::lightning, { xorstr_("default"), xorstr_("dark"), xorstr_("light") }); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, xorstr_("no jumping restrictions"), settings::infinite_jump); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, xorstr_("no attack restrictions"), settings::freeaim); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, xorstr_("no sprinting restrictions"), settings::omnisprint); misc_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + misc_y, variables::x + 120, xorstr_("no fall damage"), settings::nofall); misc_y += 15;
			}
			break;
		case 4:
			menu_framework::group_box(variables::x + 110, variables::y + 35, 285, 260, xorstr_("other"), true); {
				int other_y = 45;

				menu_framework::selector(variables::x + 135, variables::y + other_y, variables::x + 120, xorstr_("crosshair"), settings::crosshair, { xorstr_("none"), xorstr_("plusminus"), xorstr_("evilcheats") , xorstr_("dot") }); other_y += 15;

				if (menu_framework::button(variables::x + 135, variables::y + other_y, variables::x + 120, xorstr_("config save"))) {
					if (!config::save( ))
						MessageBoxA(0, xorstr_("oopsie !! config get broke!!!!!! go complain to sk4ddu that it not work !!!!!!"), xorstr_("oopsie poopie haha 4"), 0);

				} other_y += 27;

				if (menu_framework::button(variables::x + 135, variables::y + other_y, variables::x + 120, xorstr_("config load"))) {
					if (!config::load( ))
						MessageBoxA(0, xorstr_("oopsie !! config get broke!!!!!! go complain to sk4ddu that it not work !!!!!!"), xorstr_("oopsie poopie haha 5"), 0);

				} other_y += 15; other_y += 15;

				menu_framework::checkbox(variables::x + 135, variables::y + other_y, variables::x + 120, xorstr_("yeet"), settings::yeet); other_y += 15;
				menu_framework::checkbox(variables::x + 135, variables::y + other_y, variables::x + 120, xorstr_("yeet2"), settings::yeet2); other_y += 15;
					
			}
			break;
		}

		menu_framework::menu_movement(variables::x, variables::y, variables::w, 30);
	}
}