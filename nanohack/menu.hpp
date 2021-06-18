namespace menu {
	class item {
	public:
		bool* val;

		std::wstring label;
		int id = 0;

		int type = 0;
		float* f_val;
		float power;

		int* int_val;
		std::vector<std::wstring> strings;

		item(bool* val_, std::wstring labl_, int id_, int type_ = 0, float* f_val_ = 0, float power_ = 0, std::vector<std::wstring> strings_ = {}, int* i_val = 0) {
			this->val = val_;

			this->label = labl_;
			this->id = id_;

			this->type = type_;
			this->f_val = f_val_;
			this->power = power_;

			this->strings = strings_;
			this->int_val = i_val;
		}
	};
	std::vector<item> items = std::vector<item>( );

	int item_amount = 0;
	int last_id = -1;
	void add_item(bool* value, std::wstring label, /**/int type = 0, float* f_value = 0, float power = 0,/**/ std::vector<std::wstring> strings = { }, int* i_val = 0) {
		int id = 0;

		if (type != 3) {
			id = last_id + 1;
			last_id += 1;
		}

		items.emplace_back(value, label, id, type, f_value, power, strings, i_val);

		if (type != 3)
			if (items.size( ) >= 1)
				item_amount += 1;
	}
	void add_checkbox(bool* value, std::wstring label) {
		add_item(value, label, 0);
	}
	void add_slider(float* value, std::wstring label, float power = 0.1) {
		add_item(0, label, 1, value, power);
	}
	void add_selectable(int* controllable, std::wstring label, std::vector<std::wstring> strings) {
		add_item(0, label, 2, 0, 0, strings, controllable);
	}
	void add_spacer( ) {
		add_item(0, wxorstr_(L""), 3);
	}
	int current_ = 0;
	void draw( ) {
		int y_pos = 0;

		if (GetAsyncKeyState(VK_UP) & 1)
			current_ -= 1;
		else if (GetAsyncKeyState(VK_DOWN) & 1)
			current_ += 1;

		if (current_ < 0)
			current_ = item_amount - 1;
		if (current_ > item_amount - 1)
			current_ = 0;

		Renderer::text({ 200, 400.f - 17 }, Color3(0, 127, 255), 14.f, true, true, wxorstr_(L"nanohack"));

		for (auto& entry : items) {

			bool this_is_active = entry.id == current_;

			if (entry.type == 0) {
				Color3 col = this_is_active ? *entry.val == true ? Color3(219, 255, 219) : Color3(255, 158, 158) : *entry.val == true ? Color3(0, 181, 0) : Color3(255, 0, 0);

				if (this_is_active) {
					if (GetAsyncKeyState(VK_LEFT) & 1)
						*entry.val = !*entry.val;
					else if (GetAsyncKeyState(VK_RIGHT) & 1)
						*entry.val = !*entry.val;
				}
				if (this_is_active)
					Renderer::text({ 200, 400.f + y_pos }, col, 14.f, true, true, wxorstr_(L"-> %s <-"), entry.label.c_str( ));
				else
					Renderer::text({ 200, 400.f + y_pos }, col, 14.f, true, true, entry.label);
			}
			else if (entry.type == 1) {
				Color3 col = this_is_active ? Color3(255, 255, 181) : Color3(255, 255, 0);

				if (this_is_active) {
					if (GetAsyncKeyState(VK_LEFT) & 1)
						*entry.f_val -= entry.power;
					else if (GetAsyncKeyState(VK_RIGHT) & 1)
						*entry.f_val += entry.power;
				}
				if (this_is_active)
					Renderer::text({ 200, 400.f + y_pos }, col, 14.f, true, true, wxorstr_(L"-> %s [%.1f] <-"), entry.label.c_str( ), *entry.f_val);
				else
					Renderer::text({ 200, 400.f + y_pos }, col, 14.f, true, true, wxorstr_(L"%s [%.1f]"), entry.label.c_str( ), *entry.f_val);
			}
			else if (entry.type == 2) {
				Color3 col = this_is_active ? Color3(255, 181, 138) : Color3(255, 94, 0);

				int max = entry.strings.size( );

				if (this_is_active) {
					if (GetAsyncKeyState(VK_LEFT) & 1)
						*entry.int_val -= 1;
					else if (GetAsyncKeyState(VK_RIGHT) & 1)
						*entry.int_val += 1;

					if (*entry.int_val >= max)
						*entry.int_val = 0;

					if (*entry.int_val < 0)
						*entry.int_val = max - 1;
				}
				if (this_is_active)
					Renderer::text({ 200, 400.f + y_pos }, col, 14.f, true, true, wxorstr_(L"-> %s: %s <-"), entry.label.c_str( ), entry.strings[ *entry.int_val ].c_str( ));
				else
					Renderer::text({ 200, 400.f + y_pos }, col, 14.f, true, true, wxorstr_(L"%s: %s"), entry.label.c_str( ), entry.strings[ *entry.int_val ].c_str( ));
			}
			else if (entry.type == 3) {
				// nothing lol
			}
			y_pos += 17;
		}
	}
}