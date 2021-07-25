namespace settings {
	bool menu = true;
	bool cheat_init = false;

	bool nofall = true;
	bool always_eoka = true;
	bool infinite_jump = true;
	bool delay_shot = false;
	bool weapon_spam = false;
	bool players = true;
	bool npcs = false;
	bool look_dir = true;
	bool bullet_tracers = true;
	float camera_fov = 100.f;
	bool manipulator = true;
	bool desync = false;
	bool autoshoot = false;
	bool walkonwater = true;
	bool omnisprint = true;
	bool freeaim = true;
	bool psilent = true;
	bool bigger_bullets = true;
	bool chams = true;
	bool fastloot = true;
	bool faster_bullets = true;
	bool penetrate = true;

	float test1 = 1.f;
	float test2 = 0.f;

	int lightning = 2;
	int h_override = 2;
	int killsay = 0;

	namespace tr {
		bool desyncing = false;
	}
}

uintptr_t game_assembly = 0;
uintptr_t unity_player = 0;