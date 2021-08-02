#include <filesystem>

nlohmann::json json;

struct item {
	template <typename T>
	item(T& value, const std::string& sz, std::vector<item>& items) 	{
		this->name = sz;
		this->value = reinterpret_cast<void*>(&value);
		this->type = typeid(T).name( );

		items.push_back(*this);
	}
	void* value;
	std::string	name;
	std::string type;
};

void assign_item(item& item) {
	if (json[ item.name.c_str( ) ].empty( ))
		return;

	if (item.type == "float")
		*reinterpret_cast<float*>(item.value) = json[ item.name.c_str( ) ].get< float >( );
	if (item.type == "int")
		*reinterpret_cast<int*>(item.value) = json[ item.name.c_str( ) ].get< int >( );
	if (item.type == "bool")
		*reinterpret_cast<bool*>(item.value) = json[ item.name.c_str( ) ].get< bool >( );
	if (item.type == "float [4]")
		for (int i = 0; i < 4; i++)
			reinterpret_cast<float*>(item.value)[ i ] = json[ item.name.c_str( ) ][ std::to_string(i).c_str( ) ].get< float >( );
}

void save_item(item& item) {
	if (item.type == "float")
		json[ item.name.c_str( ) ] = *reinterpret_cast<float*>(item.value);
	if (item.type == "int")
		json[ item.name.c_str( ) ] = *reinterpret_cast<int*>(item.value);
	if (item.type == "bool")
		json[ item.name.c_str( ) ] = *reinterpret_cast<bool*>(item.value);
	if (item.type == "float [4]")
		for (int i = 0; i < 4; i++)
			json[ item.name.c_str( ) ][ std::to_string(i).c_str( ) ] = reinterpret_cast<float*>(item.value)[ i ];
}

namespace config {
	std::vector< item > m_items;
	std::string m_directory = xorstr_("plusminus");

	std::string file = xorstr_("config");

	bool init( ) {
		if (!std::filesystem::exists(m_directory)) {
			if (!std::filesystem::create_directory(m_directory)) {
				MessageBoxA(0, xorstr_("oopsie !! config get broke!!!!!! go complain to sk4ddu that it not work !!!!!!"), xorstr_("oopsie poopie haha 1"), 0);
				return false;
			}
		}

		item(settings::psilent, "settings::psilent", m_items);
		item(settings::h_override, "settings::h_override", m_items);
		item(settings::manipulator, "settings::manipulator", m_items);
		item(settings::desync, "settings::desync", m_items);
		item(settings::desync_key, "settings::desync_key", m_items);
		item(settings::autoshoot, "settings::autoshoot", m_items);
		item(settings::penetrate, "settings::penetrate", m_items);
		item(settings::draw_fov, "settings::draw_fov", m_items);
		item(settings::targeting_fov, "settings::targeting_fov", m_items);

		item(settings::recoil_p, "settings::recoil_p", m_items);
		item(settings::spread_p, "settings::spread_p", m_items);
		item(settings::nosway, "settings::nosway", m_items);
		item(settings::bigger_bullets, "settings::bigger_bullets", m_items);
		item(settings::faster_bullets, "settings::faster_bullets", m_items);
		item(settings::always_eoka, "settings::always_eoka", m_items);

		item(settings::players, "settings::players", m_items);
		item(settings::npcs, "settings::npcs", m_items);
		item(settings::look_dir, "settings::look_dir", m_items);
		item(settings::skeleton, "settings::skeleton", m_items);
		item(settings::box, "settings::box", m_items);
		item(settings::reload_indicator, "settings::reload_indicator", m_items);
		item(settings::belt, "settings::belt", m_items);

		item(settings::camera_fov, "settings::camera_fov", m_items);
		item(settings::zoom_key, "settings::zoom_key", m_items);
		item(settings::bullet_tracers, "settings::bullet_tracers", m_items);
		item(settings::fakeadmin, "settings::fakeadmin", m_items);
		item(settings::fastloot, "settings::fastloot", m_items);
		item(settings::farm_assist, "settings::farm_assist", m_items);
		item(settings::walkonwater, "settings::walkonwater", m_items);
		item(settings::lightning, "settings::lightning", m_items);
		item(settings::weapon_spam, "settings::weapon_spam", m_items);
		item(settings::weapon_spam_key, "settings::weapon_spam_key", m_items);
		item(settings::infinite_jump, "settings::infinite_jump", m_items);
		item(settings::freeaim, "settings::freeaim", m_items);
		item(settings::omnisprint, "settings::omnisprint", m_items);
		item(settings::nofall, "settings::nofall", m_items);
		item(settings::camera_fov, "settings::camera_fov", m_items);

		item(settings::crosshair, "settings::crosshair", m_items);

		item(settings::variables::x, "variables::x", m_items);
		item(settings::variables::y, "variables::y", m_items);
		item(settings::g::b_x, "variables::b_x", m_items);
		item(settings::g::b_y, "variables::b_y", m_items);

		return true;
	}

	bool load( ) {
		std::ifstream input_file = std::ifstream(m_directory + "/" + file);
		if (!input_file.good())
			return false;

		try {
			input_file >> json;
		}
		catch (const std::exception& ex) {
			UNREFERENCED_PARAMETER(ex);
			MessageBoxA(0, xorstr_("oopsie !! config get broke!!!!!! go complain to sk4ddu that it not work !!!!!!"), xorstr_("oopsie poopie haha 2"), 0);

			input_file.close( );
			return false;
		}

		for (auto& item : m_items)
			assign_item(item);

		input_file.close( );

		return true;
	}
	bool save() {
		try {
			std::ofstream output_file = std::ofstream(m_directory + "/" + file);

			if (!output_file.good( ))
				return false;

			for (auto& item : m_items)
				save_item(item);

			output_file << std::setw(4) << json << std::endl;
			output_file.close( );

			return true;
		}
		catch (std::ofstream::failure& ex) {
			UNREFERENCED_PARAMETER(ex);
			MessageBoxA(0, xorstr_("oopsie !! config get broke!!!!!! go complain to sk4ddu that it not work !!!!!!"), xorstr_("oopsie poopie haha 3"), 0);
			return false;
		}
	}
};