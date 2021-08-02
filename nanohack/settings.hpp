namespace settings {
	bool menu = true;
	bool cheat_init = false;

	bool nofall = false;
	bool always_eoka = false;
	bool infinite_jump = false;
	bool weapon_spam = false;
	int weapon_spam_key = 76;
	bool players = true;
	bool npcs = false;
	bool look_dir = false;
	bool belt = false;
	bool bullet_tracers = false;
	float camera_fov = 90.f;
	float targeting_fov = 300.f;
	float recoil_p = 100.f;
	float spread_p = 100.f;
	bool draw_fov = false;
	bool manipulator = false;
	bool desync = false;
	int desync_key = 5;
	bool autoshoot = false;
	bool fakeadmin = false;
	bool walkonwater = false;
	int zoom_key = 6;
	bool reload_indicator = false;
	bool farm_assist = false;
	bool omnisprint = false;
	bool freeaim = false;
	bool psilent = false;
	bool bigger_bullets = false;
	bool skeleton = false;
	bool box = false;
	bool fastloot = false;
	bool faster_bullets = false;
	bool penetrate = false;
	bool nosway = false;

	int lightning = 0;
	int h_override = 0;
	int killsay = 0;
	int crosshair = 0;

	bool debug = false;

	namespace variables {
		int x = 140, y = 140;
		int w = 400, h = 300;
	}

	namespace esp {
		namespace resources {
			bool enabled = false;

			bool stone = false;
			bool sulfur = false;
			bool metal = false;
			bool hemp = false;
		}
		namespace base {
			bool enabled = false;

			bool tc = false;
		}
		namespace others {
			bool enabled = false;

			bool corpses = false;
			bool stashes = false;
		}
	}

	bool panic = false;

	namespace tr {
		bool desyncing = false;
	}
	namespace g {
		int b_x = 200, b_y = 200;
	}
	namespace auth {
		std::wstring username = L"dev";
		int days_left = 999;
	}
}

uintptr_t game_assembly = 0;
uintptr_t unity_player = 0;