#pragma once


namespace plusminus::ui
{
	static bool init = false;
	static FGUI::CBuilder ptrnBuilder;

	namespace vars
	{
		inline std::shared_ptr<FGUI::CContainer> Container;
		inline std::shared_ptr<FGUI::CTabPanel> Tabs;

		std::unordered_map<std::string, std::shared_ptr<FGUI::CCheckBox>> checkBoxes;
		std::unordered_map<std::string, std::shared_ptr<FGUI::CButton>> buttons;
		std::unordered_map<std::string, std::shared_ptr<FGUI::CSlider>> sliders;
		std::unordered_map<std::string, std::shared_ptr<FGUI::CComboBox>> comboBoxes;
		std::unordered_map<std::string, std::shared_ptr<FGUI::CColorPicker>> colorPickers;
		std::unordered_map<std::string, std::shared_ptr<FGUI::CContainer>> groupBoxes;
		std::unordered_map<std::string, std::shared_ptr<FGUI::CKeyBinder>> keybinders;
		std::unordered_map<std::string, std::shared_ptr<FGUI::CTextBox>> textBoxes;
		std::unordered_map<std::string, std::shared_ptr<FGUI::CLabel>> labels;
	}

	bool is_menu_open()
	{
		return vars::Container.get()->GetState();
	}

	bool get_bool(std::string name)
	{
		if (vars::checkBoxes.contains(name))
			return vars::checkBoxes[name]->GetState();
		
		//printf("[checkbox] %s\n", name.c_str());

		return false;
	}

	std::size_t get_combobox(std::string name)
	{
		if (vars::comboBoxes.contains(name))
			return vars::comboBoxes[name]->GetIndex();
		
		//printf("[combo] %s\n", name.c_str());

		return 0;
	}

	float get_float(std::string name)
	{
		if(vars::sliders.contains(name))
			return vars::sliders[name]->GetValue();
		
		//printf("[slider] %s\n", name.c_str());

		return 0.0f;
	}

	std::string get_text(std::string name)
	{
		if (vars::textBoxes.contains(name))
			return vars::textBoxes[name]->GetText();

		//printf("[text] %s\n", name.c_str());

		return "";
	}

	Color3 get_color(std::string name)
	{
		if (vars::colorPickers.contains(name))
		{
			auto clr = vars::colorPickers[name]->GetColor();
			return Color3(clr.m_ucRed, clr.m_ucGreen, clr.m_ucBlue, clr.m_ucAlpha);
		}

		//printf("[color] %s\n", name.c_str());

		return Color3(255, 255, 255, 255.0f);
	}

	std::uint32_t get_keybind(std::string name)
	{
		if(vars::keybinders.contains(name))
			return vars::keybinders[name]->GetKey();

		//printf("[keybind] %s\n", name.c_str());

		return 0;
	}

	enum class Tabs : int
	{
		Visual,
		Combat,
		Misc,
		Colors
	};

	namespace wrapper
	{
		static int currentY = 35.0f;

		void reset_height()
		{
			currentY = 35.0f;
		}

		void create_window(std::string name, Vector2 position, Vector2 size, std::uint32_t menu_key)
		{
			ptrnBuilder.Widget(vars::Container).Title(name).Position(position.x, position.y).Size(size.x, size.y).Key(menu_key).Font(xorstr_("Tahoma"), 12, true, 1);
		}

		void button(std::string name, std::function<void()> callback, Vector2 position, Tabs tab = Tabs::Visual)
		{
			vars::buttons[name] = std::make_shared<FGUI::CButton>();
			ptrnBuilder.Widget(vars::buttons[name]).Title(name).Position(position.x, currentY + position.y).Font(xorstr_("Tahoma"), 11).Medium(vars::Tabs, (int)tab).Callback(callback).SpawnIn(vars::Container, false);
			currentY += 25.0f;
		}

		void checkbox(std::string name, Vector2 position, Tabs tab = Tabs::Visual, bool keybind = false, bool colorpick = false, Vector2 cppos = Vector2(0,0))
		{
			auto text_size = Renderer::get_text_size(StringConverter::ToUnicode(name), 11.0f);
			vars::checkBoxes[name] = std::make_shared<FGUI::CCheckBox>();
			auto cb = ptrnBuilder.Widget(vars::checkBoxes[name]).Title(name).Position(position.x, position.y + currentY).Font(xorstr_("Tahoma"), 11).Medium(vars::Tabs, (int)tab).SpawnIn(vars::Container, false);
			if (colorpick)
			{
				vars::colorPickers[name] = std::make_shared<FGUI::CColorPicker>();
				ptrnBuilder.Widget(vars::colorPickers[name]).Title(name + xorstr_(" color")).Position((text_size.x * 1.5) + 10.0f + position.x + cppos.x, currentY + position.y).Font(xorstr_("Tahoma"), 11).Medium(vars::Tabs, (int)tab).SpawnIn(vars::Container, false);
			}
			currentY += 20.0f;
			if (keybind)
			{
				vars::keybinders[name + xorstr_(" bind")] = std::make_shared<FGUI::CKeyBinder>();
				ptrnBuilder.Widget(vars::keybinders[name + xorstr_(" bind")]).Title(name + xorstr_(" bind")).Position(position.x, position.y + currentY).Font(xorstr_("Tahoma"), 11).Medium(vars::Tabs, (int)tab).SpawnIn(vars::Container, false);
				currentY += 25.0f;
			}
		}

		void label(std::string name, Vector2 position, Tabs tab = Tabs::Visual, bool newline = true)
		{
			vars::labels[name] = std::make_shared<FGUI::CLabel>();
			ptrnBuilder.Widget(vars::labels[name]).Title(name).Position(position.x, position.y + currentY).Font(xorstr_("Tahoma"), 11).Medium(vars::Tabs, (int)tab).SpawnIn(vars::Container, false);
			if(newline)
				currentY += 20.0f;
		}

		void color_picker(std::string name, Vector2 position, Tabs tab = Tabs::Visual)
		{
			vars::colorPickers[name] = std::make_shared<FGUI::CColorPicker>();
			ptrnBuilder.Widget(vars::colorPickers[name]).Title(name).Position(position.x, currentY + position.y).Font(xorstr_("Tahoma"), 11).Medium(vars::Tabs, (int)tab).SpawnIn(vars::Container, false);
			currentY += 20.0f;
		}

		void textbox(std::string name, Vector2 position, Tabs tab = Tabs::Visual, std::string value = "")
		{
			vars::textBoxes[name] = std::make_shared<FGUI::CTextBox>();
			ptrnBuilder.Widget(vars::textBoxes[name]).Title(name).Position(position.x, position.y + currentY).Font(xorstr_("Tahoma"), 11).Text(value).Medium(vars::Tabs, (int)tab).SpawnIn(vars::Container, false);
			currentY += 25.0f;
		}

		void combobox(std::string name, std::vector<std::string> options, Vector2 position, Tabs tab = Tabs::Visual)
		{
			vars::comboBoxes[name] = std::make_shared<FGUI::CComboBox>();
			auto widget = ptrnBuilder.Widget(vars::comboBoxes[name]).Title(name).Position(position.x, currentY + position.y).Font(xorstr_("Tahoma"), 11).Medium(vars::Tabs, (int)tab);
			
			for(auto opt : options)
				widget.Entry(opt);

			widget.SpawnIn(vars::Container, false);
			currentY += 25.0f;
		}

		void keybind(std::string name, Vector2 position, Tabs tab = Tabs::Visual)
		{
			vars::keybinders[name] = std::make_shared<FGUI::CKeyBinder>();
			ptrnBuilder.Widget(vars::keybinders[name]).Title(name).Position(position.x, position.y + currentY).Font(xorstr_("Tahoma"), 11).Medium(vars::Tabs, (int)tab).SpawnIn(vars::Container, false);
			currentY += 25.0f;
		}

		void slider(std::string name, Vector2 position, float value, float min, float max, Tabs tab = Tabs::Combat)
		{
			vars::sliders[name] = std::make_shared<FGUI::CSlider>();
			ptrnBuilder.Widget(vars::sliders[name]).Title(name).Range(min, max).Value(value).Position(position.x, position.y + currentY).Font(xorstr_("Tahoma"), 11).Medium(vars::Tabs, (int)tab).SpawnIn(vars::Container, false);
			currentY += 30.0f;
		}

		void tabs(std::vector<std::string> tabs)
		{
			auto builder = ptrnBuilder.Widget(vars::Tabs).Position(0, 0).Font(xorstr_("Tahoma"), 11, true, 1);
			for (auto tab : tabs)
				builder.Tab(tab);
			
			builder.SpawnIn(vars::Container, false);
		}
	}

	inline void OnSetupDevice()
	{
		VM_EAGLE_BLACK_START

		init = true;

		vars::checkBoxes = {};
		vars::buttons = {};
		vars::sliders = {};
		vars::comboBoxes = {};
		vars::colorPickers = {};
		vars::groupBoxes = {};
		vars::keybinders = {};

		vars::Container = std::make_shared<FGUI::CContainer>();
		vars::Tabs = std::make_shared<FGUI::CTabPanel>();
		wrapper::create_window(xorstr_("plusminus.vip"), Vector2(200, 200), Vector2(470, 310), VK_INSERT);
		wrapper::tabs({ xorstr_("Visuals"), xorstr_("Combat"), xorstr_("Misc"), xorstr_("Colors") });

		// == Visuals == \\

		wrapper::checkbox(xorstr_("players"), Vector2(0, 0), Tabs::Visual);
		wrapper::checkbox(xorstr_("sleepers"), Vector2(0, 0), Tabs::Visual);
		wrapper::checkbox(xorstr_("npc"), Vector2(0, 0), Tabs::Visual);
		wrapper::checkbox(xorstr_("looking direction"), Vector2(0, 0), Tabs::Visual);
		wrapper::checkbox(xorstr_("skeleton"), Vector2(0, 0), Tabs::Visual);
		wrapper::checkbox(xorstr_("box"), Vector2(0, 0), Tabs::Visual);
		wrapper::combobox(xorstr_("box type"), { xorstr_("cornered"), xorstr_("full"), xorstr_("3 dimension") }, Vector2(0, 0), Tabs::Visual);

		wrapper::checkbox(xorstr_("reload indicator"), Vector2(0, 0), Tabs::Visual);
		wrapper::checkbox(xorstr_("target player belt"), Vector2(0, 0), Tabs::Visual);

		wrapper::reset_height();

		wrapper::checkbox(xorstr_("ores"), Vector2(160.0f, 0), Tabs::Visual);
		wrapper::checkbox(xorstr_("stashes"), Vector2(160.0f, 0), Tabs::Visual);
		wrapper::checkbox(xorstr_("corpses"), Vector2(160.0f, 0), Tabs::Visual);
		wrapper::checkbox(xorstr_("traps"), Vector2(160.0f, 0), Tabs::Visual);
		wrapper::checkbox(xorstr_("hemp"), Vector2(160.0f, 0), Tabs::Visual);
		wrapper::checkbox(xorstr_("vehicles"), Vector2(160.0f, 0), Tabs::Visual);
#ifndef authh
		wrapper::checkbox(xorstr_("debug"), Vector2(160.0f, 0), Tabs::Visual, false, true);
#endif

		wrapper::reset_height();

		// == Combat == \\

		wrapper::currentY += 10.0f;

		wrapper::slider(xorstr_("recoil %"), Vector2(0, 0.0f), 100.0f, 0.0f, 100.0f, Tabs::Combat);
		wrapper::slider(xorstr_("spread %"), Vector2(0, 0), 100.0f, 0.0f, 100.0f, Tabs::Combat);
		wrapper::currentY -= 10.0f;
		wrapper::checkbox(xorstr_("no sway"), Vector2(0, 0), Tabs::Combat);
		wrapper::checkbox(xorstr_("big bullets"), Vector2(0, 0), Tabs::Combat);
		wrapper::checkbox(xorstr_("fast bullets"), Vector2(0, 0), Tabs::Combat);
		wrapper::checkbox(xorstr_("insta eoka"), Vector2(0, 0), Tabs::Combat);
		wrapper::checkbox(xorstr_("automatic"), Vector2(0, 0), Tabs::Combat);
		wrapper::checkbox(xorstr_("draw targeting fov"), Vector2(0.0f, 0.0f), Tabs::Combat);
		wrapper::reset_height();
		wrapper::checkbox(xorstr_("psilent"), Vector2(140.0f, 0), Tabs::Combat);
		wrapper::checkbox(xorstr_("always heli weakspot"), Vector2(140.0f, 0), Tabs::Combat);
		//wrapper::checkbox(xorstr_("hitbox override"), Vector2(0, 0), Tabs::Combat);
		wrapper::combobox(xorstr_("hitbox override"), { xorstr_("none"), xorstr_("body"), xorstr_("head"), xorstr_("randomize (all)"), xorstr_("randomize (main)") }, Vector2(140.0f, 0), Tabs::Combat);
		wrapper::checkbox(xorstr_("manipulator"), Vector2(140.0f, 0), Tabs::Combat, true, false);
		wrapper::checkbox(xorstr_("autoshoot"), Vector2(140.0f, 0), Tabs::Combat);
		wrapper::checkbox(xorstr_("pierce"), Vector2(140.0f, 0), Tabs::Combat);
		wrapper::slider(xorstr_("targeting fov"), Vector2(140.0f, 10.0f), 300.0f, 30.0f, 2500.0f, Tabs::Combat);

		wrapper::reset_height();

		// == Misc == \\


		wrapper::currentY += 10.0f;
		wrapper::slider(xorstr_("camera fov"), Vector2(0, 0.0f), 90.0f, 30.0f, 160.0f, Tabs::Misc);
		wrapper::currentY -= 15.0f;
		wrapper::keybind(xorstr_("zoom bind"), Vector2(0, 0.0f), Tabs::Misc);
		wrapper::currentY += 5.0f;
		wrapper::checkbox(xorstr_("bullet tracers"), Vector2(0, 0), Tabs::Misc);
		wrapper::checkbox(xorstr_("fake admin"), Vector2(0, 0), Tabs::Misc);
		wrapper::checkbox(xorstr_("fast loot"), Vector2(0, 0), Tabs::Misc);
		wrapper::checkbox(xorstr_("farm assist"), Vector2(0, 0), Tabs::Misc);
		wrapper::checkbox(xorstr_("walk on water"), Vector2(0, 0), Tabs::Misc);
		wrapper::checkbox(xorstr_("fake shots"), Vector2(0, 0), Tabs::Misc, true, false);
		wrapper::combobox(xorstr_("lightning"), { xorstr_("default"), xorstr_("dark"), xorstr_("light") }, Vector2(0, 0), Tabs::Misc);
		wrapper::reset_height();
		wrapper::checkbox(xorstr_("no jumping restriction"), Vector2(160.0f, 0), Tabs::Misc);
		wrapper::checkbox(xorstr_("no attack restriction"), Vector2(160.0f, 0), Tabs::Misc);
		wrapper::checkbox(xorstr_("no sprinting restriction"), Vector2(160.0f, 0), Tabs::Misc);
		wrapper::checkbox(xorstr_("no fall damage"), Vector2(160.0f, 0), Tabs::Misc);
		wrapper::combobox(xorstr_("crosshair"), { xorstr_("none"), xorstr_("plusminus"), xorstr_("evilcheats"), xorstr_("circle"), xorstr_("swastika") }, Vector2(160.0f, 0), Tabs::Misc);
		wrapper::textbox(xorstr_("config name"), Vector2(160.0f, 0), Tabs::Misc, xorstr_("default"));
		wrapper::button(xorstr_("Save Config"), [&]() {
			PWSTR szPath = NULL;

			if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &szPath)))
			{
				std::wstring configPath = std::wstring(szPath) + wxorstr_(L"\\plusminus");
				if(!get_text(xorstr_("config name")).empty())
					vars::Container->SaveToFile(StringConverter::ToASCII(configPath) + xorstr_("\\") + get_text(xorstr_("config name")) + xorstr_(".cfg"));
			}
		}, Vector2(160.0f, 0), Tabs::Misc);
		wrapper::button(xorstr_("Load Config"), [&]() {
			PWSTR szPath = NULL;

			if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &szPath)))
			{
				std::wstring configPath = std::wstring(szPath) + wxorstr_(L"\\plusminus");
				if (!get_text(xorstr_("config name")).empty())
					vars::Container->LoadFromFile(StringConverter::ToASCII(configPath) + xorstr_("\\") + get_text(xorstr_("config name")) + xorstr_(".cfg"));
			}
		}, Vector2(160.0f, 0), Tabs::Misc);

		wrapper::button(xorstr_("Panic"), [&]() {
			settings::panic = true;
		}, Vector2(160.0f, 0), Tabs::Misc);

		wrapper::reset_height();

		// == Colors == \\

		wrapper::label(xorstr_("players"), Vector2(0, 0), Tabs::Colors);
		wrapper::label(xorstr_("visible"), Vector2(10, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("visible players"), Vector2(45, 0), Tabs::Colors);
		wrapper::label(xorstr_("invisible"), Vector2(10, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("invisible players"), Vector2(50, 0), Tabs::Colors);
		wrapper::label(xorstr_("visible teammate"), Vector2(10, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("visible teammate"), Vector2(95, 0), Tabs::Colors);
		wrapper::label(xorstr_("invisible teammate"), Vector2(10, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("invisible teammate"), Vector2(105, 0), Tabs::Colors);
		wrapper::label(xorstr_("visible skeleton"), Vector2(10, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("visible skeleton players"), Vector2(95, 0), Tabs::Colors);
		wrapper::label(xorstr_("invisible skeleton"), Vector2(10, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("invisible skeleton players"), Vector2(105, 0), Tabs::Colors);

		wrapper::label(xorstr_("sleepers"), Vector2(0, 0), Tabs::Colors);
		wrapper::label(xorstr_("visible"), Vector2(10, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("visible sleepers"), Vector2(45, 0), Tabs::Colors);
		wrapper::label(xorstr_("invisible"), Vector2(10, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("invisible sleepers"), Vector2(50, 0), Tabs::Colors);
		wrapper::label(xorstr_("visible skeleton"), Vector2(10, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("visible skeleton"), Vector2(90, 0), Tabs::Colors);
		wrapper::label(xorstr_("invisible skeleton"), Vector2(10, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("invisible skeleton"), Vector2(100, 0), Tabs::Colors);


		wrapper::reset_height();

		wrapper::label(xorstr_("looking direction"), Vector2(130, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("looking direction color"), Vector2(215, 0), Tabs::Colors);
		wrapper::label(xorstr_("targeting fov"), Vector2(130, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("targeting fov color"), Vector2(195, 0), Tabs::Colors);

		wrapper::label(xorstr_("npcs"), Vector2(130, 0), Tabs::Colors);
		wrapper::label(xorstr_("visible"), Vector2(140, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("visible npcs"), Vector2(175, 0), Tabs::Colors);
		wrapper::label(xorstr_("invisible"), Vector2(140, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("invisible npcs"), Vector2(180, 0), Tabs::Colors);
		wrapper::label(xorstr_("visible skeleton"), Vector2(140, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("visible skeleton npc"), Vector2(220, 0), Tabs::Colors);
		wrapper::label(xorstr_("invisible skeleton"), Vector2(140, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("invisible skeleton npc"), Vector2(225, 0), Tabs::Colors);

		wrapper::label(xorstr_("ores"), Vector2(130, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("ores color"), Vector2(170, 0), Tabs::Colors);
		wrapper::label(xorstr_("stashes"), Vector2(130, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("stashes color"), Vector2(170, 0), Tabs::Colors);
		wrapper::label(xorstr_("corpses"), Vector2(130, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("corpses color"), Vector2(170, 0), Tabs::Colors);
		wrapper::label(xorstr_("traps"), Vector2(130, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("traps color"), Vector2(170, 0), Tabs::Colors);
		wrapper::label(xorstr_("hemp"), Vector2(130, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("hemp color"), Vector2(170, 0), Tabs::Colors);

		wrapper::reset_height();

		wrapper::label(xorstr_("vehicles"), Vector2(250, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("vehicles color"), Vector2(290, 0), Tabs::Colors);

		wrapper::label(xorstr_("boxes"), Vector2(250, 0), Tabs::Colors);
		wrapper::label(xorstr_("visible"), Vector2(260, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("visible boxes"), Vector2(305, 0), Tabs::Colors);
		wrapper::label(xorstr_("invisible"), Vector2(260, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("invisible boxes"), Vector2(305, 0), Tabs::Colors);
		wrapper::label(xorstr_("targeted"), Vector2(260, 0), Tabs::Colors, false);
		wrapper::color_picker(xorstr_("targeted boxes"), Vector2(305, 0), Tabs::Colors);
		
		VM_EAGLE_BLACK_END
	}
}