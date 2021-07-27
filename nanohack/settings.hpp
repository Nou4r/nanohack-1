namespace settings {
	bool menu = true;
	bool cheat_init = false;

	bool nofall = false;
	bool always_eoka = false;
	bool infinite_jump = false;
	bool weapon_spam = false;
	bool players = true;
	bool npcs = false;
	bool look_dir = false;
	bool bullet_tracers = false;
	float camera_fov = 90.f;
	bool manipulator = false;
	bool desync = false;
	int desync_key = 5;
	bool autoshoot = false;
	bool walkonwater = false;
	bool farm_assist = false;
	bool omnisprint = false;
	bool freeaim = false;
	bool psilent = false;
	bool bigger_bullets = false;
	bool chams = false;
	bool fastloot = false;
	bool faster_bullets = false;
	bool penetrate = false;

	float test1 = 1.f;
	float test2 = 0.f;

	int lightning = 0;
	int h_override = 0;
	int killsay = 0;

	namespace tr {
		bool desyncing = false;
	}
}

uintptr_t game_assembly = 0;
uintptr_t unity_player = 0;