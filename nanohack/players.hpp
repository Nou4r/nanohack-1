namespace players {
	float dfc(BasePlayer* player) {
		if (!player) 
			return 1000.f;

		if (!player->isCached( )) 
			return 1000.f;

		if (player->bones()->dfc.empty( )) 
			return 1000.f;

		return screen_center.distance_2d(player->bones( )->dfc);
	}
	void loop( ) {
		auto local = LocalPlayer::Entity( );
		if (local == nullptr || !local->isCached( ))
			return;

		auto playerList = BasePlayer::visiblePlayerList( );
		if (!playerList)
			return;

		if (target_ply != nullptr) {
			if (!target_ply->IsValid( ) || target_ply->health( ) <= 0 || target_ply->HasPlayerFlag(PlayerFlags::Sleeping) || (target_ply->playerModel( )->isNpc( ) && !settings::npcs))
				target_ply = nullptr;
			else
				if (target_ply->isCached( )) {
					auto bounds = target_ply->bones( )->bounds;
					if (!bounds.empty( ))
						Renderer::line({ bounds.left + ((bounds.right - bounds.left) / 2), bounds.bottom }, { screen_center.x, screen_size.y }, Color3(255, 0, 0), true);

					if (settings::manipulator && !other::m_manipulate.empty( ))
						Renderer::text({ screen_center.x, screen_center.y + 55 }, Color3(200, 0, 0), 12.f, true, true, wxorstr_(L"[manipulated]"));

					if (target_ply->find_mpv_bone( )->visible)
						Renderer::text({ screen_center.x, screen_center.y + 40 }, Color3(66, 135, 245), 12.f, true, true, wxorstr_(L"[shootable]"));

					Renderer::text({ screen_center.x, screen_center.y + 25 }, Color3(255, 0, 0), 12.f, true, true, target_ply->_displayName( ));
				}
		}

		auto held = local->GetHeldEntity<BaseProjectile>( );
		if (held) {
			if (held->HasReloadCooldown( ) && held->class_name_hash() != STATIC_CRC32("BowWeapon") && held->class_name_hash() != STATIC_CRC32("CompoundBowWeapon")) { // im sorry for my sins
				float time_left = held->nextReloadTime( ) - GLOBAL_TIME;
				float time_full = held->CalculateCooldownTime(held->nextReloadTime( ), held->reloadTime( )) - GLOBAL_TIME;
				
				Renderer::rectangle_filled({ screen_center.x - 26, screen_center.y + 64 }, { 51, 5 }, Color3(0, 0, 0));
				Renderer::rectangle_filled({ screen_center.x - 25, screen_center.y + 65 }, { 50 * (time_left / time_full), 4 }, Color3(0, 255, 0));
				Renderer::text({ (screen_center.x - 25) + (50 * (time_left / time_full)), screen_center.y + 65 + 2 }, Color3(255, 255, 255), 12.f, true, true, wxorstr_(L"%d"), (int)ceil(time_left));
			}
		}

		for (int i = 0; i < playerList->vals->size; i++) {
			auto player = *reinterpret_cast<BasePlayer**>(std::uint64_t(playerList->vals->buffer) + (0x20 + (sizeof(void*) * i)));

			if (!player) continue;
			if (!player->IsValid( )) continue;
			if (!player->isCached( )) continue;
			if (player->health( ) <= 0.0f) continue;
			if (player->HasPlayerFlag(PlayerFlags::Sleeping)) continue;
			if (player->playerModel( )->isNpc( ) && !settings::npcs) continue;
			if (player->userID( ) == LocalPlayer::Entity( )->userID( )) continue;

			auto bounds = player->bones( )->bounds;
			if (!bounds.empty( )) {

				float box_width = bounds.right - bounds.left;
				float box_height = bounds.bottom - bounds.top;
				Vector2 footPos = { bounds.left + (box_width / 2), bounds.bottom + 7.47f };
				Vector2 headPos = { bounds.left + (box_width / 2), bounds.top - 9.54f };

				Color3 col = player->is_target( ) ? Color3(255, 0, 0) : player->playerModel( )->isNpc( ) ? Color3(71, 209, 255) : Color3(255, 255, 255);

				Renderer::text(headPos, col, 13.f, true, true, wxorstr_(L"%s [%dhp %dm]"), player->_displayName( ), (int)ceil(player->health( )), (int)ceil(player->bones( )->head->position.distance(local->bones( )->head->position)));

				if (player->GetHeldItem( ))
					Renderer::text(footPos, col, 13.f, true, true, player->GetHeldItem( )->info( )->shortname( ));

				if (target_ply == nullptr)
					target_ply = player;
				else
					if (dfc(target_ply) > dfc(player))
						target_ply = player;
			}
		}
	}
}