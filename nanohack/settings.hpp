namespace settings {
	std::string data_dir = xorstr_("");
	bool cheat_init = false;

	bool panic = false;

	namespace tr {
		bool manipulated = false;
	}
	namespace auth {
		std::wstring username = wxorstr_(L"dev");
		std::wstring days_left = wxorstr_(L"unlimited");
	}
}

uintptr_t game_assembly = 0;
uintptr_t unity_player = 0;