namespace bonecache {
	std::array<int, 20> bones = {
		1, 2, 3, 5, 6, 14, 15, 17, 18, 21, 23, 24, 25, 26, 27, 48, 55, 56, 57, 76
	};
	bool get_bounds(box_bounds& out, BasePlayer* player, float expand = 0) {
		out = { FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN };

		for (auto& j : bones) {

			if (player->model( )) {
				if (player->model( )->boneTransforms( )) {
					auto transform = player->model( )->boneTransforms( )->gett(j);
					if (transform) {
						Vector3 world_position = transform->position( );

						if (j == 48) // jaw
							world_position.y += 0.2f;

						Vector2 sc;
						if (Camera::world_to_screen(world_position, sc)) {
							Vector2 bone_screen = sc;
							if (bone_screen.x < out.left)
								out.left = bone_screen.x;
							if (bone_screen.x > out.right)
								out.right = bone_screen.x;
							if (bone_screen.y < out.top)
								out.top = bone_screen.y;
							if (bone_screen.y > out.bottom)
								out.bottom = bone_screen.y;
						}
					}
				}
			}
		}

		if (out.left == FLT_MAX)
			return false;
		if (out.right == FLT_MIN)
			return false;
		if (out.top == FLT_MAX)
			return false;
		if (out.bottom == FLT_MIN)
			return false;

		out.left -= expand;
		out.right += expand;
		out.top -= expand;
		out.bottom += expand;

		return true;
	};
	void cachePlayer(BasePlayer* player) {
		auto model = player->model( );
		if (model) {
			auto cache = new BoneCache( );

			cache->head = model->resolve(STATIC_CRC32("head"));
			cache->neck = model->resolve(STATIC_CRC32("neck"));
			cache->root = model->resolve(STATIC_CRC32("root"));
			cache->spine4 = model->resolve(STATIC_CRC32("spine4"));
			cache->spine1 = model->resolve(STATIC_CRC32("spine1"));
			cache->l_clavicle = model->resolve(STATIC_CRC32("l_clavicle"));
			cache->l_upperarm = model->resolve(STATIC_CRC32("l_upperarm"));
			cache->l_forearm = model->resolve(STATIC_CRC32("l_forearm"));
			cache->l_hand = model->resolve(STATIC_CRC32("l_hand"));
			cache->r_clavicle = model->resolve(STATIC_CRC32("r_clavicle"));
			cache->r_upperarm = model->resolve(STATIC_CRC32("r_upperarm"));
			cache->r_forearm = model->resolve(STATIC_CRC32("r_forearm"));
			cache->r_hand = model->resolve(STATIC_CRC32("r_hand"));
			cache->pelvis = model->resolve(STATIC_CRC32("pelvis"));
			cache->l_hip = model->resolve(STATIC_CRC32("l_hip"));
			cache->l_knee = model->resolve(STATIC_CRC32("l_knee"));
			cache->l_ankle_scale = model->resolve(STATIC_CRC32("l_ankle_scale"));
			cache->l_foot = model->resolve(STATIC_CRC32("l_foot"));
			cache->r_hip = model->resolve(STATIC_CRC32("r_hip"));
			cache->r_knee = model->resolve(STATIC_CRC32("r_knee"));
			cache->r_ankle_scale = model->resolve(STATIC_CRC32("r_ankle_scale"));
			cache->r_foot = model->resolve(STATIC_CRC32("r_foot"));

			box_bounds bo;
			if (player->userID( ) != LocalPlayer::Entity( )->userID( )) {
				if (get_bounds(bo, player, 4.f))
					if (!bo.empty( ))
						cache->bounds = bo;

				Vector2 footPos;
				if (Camera::world_to_screen(cache->head->position, footPos)) {
					cache->dfc = footPos;
				}
			}
				

			if (LocalPlayer::Entity( )->userID( ) == player->userID( ))
				cache->bottom = LocalPlayer::Entity( )->transform( )->position( );

			if (!map_contains_key(cachedBones, player->userID( )))
				cachedBones.insert(std::make_pair(player->userID( ), cache));
			else
				cachedBones[ player->userID( ) ] = cache;
		}
	}
}