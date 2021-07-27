namespace players {
	float dfc(BasePlayer* player) {
		if (!player)
			return 1000.f;

		if (!player->isCached( ))
			return 1000.f;

		if (player->bones( )->dfc.empty( ))
			return 1000.f;

		return screen_center.distance_2d(player->bones( )->dfc);
	}
	void loop( ) {
		switch (settings::crosshair) {
		case 1:
			Renderer::circle(screen_center - Vector2(2, 2), Color3(255, 255, 255), 4.f, 0.75f);
			Renderer::circle(screen_center + Vector2(2, 2), Color3(0, 0, 0), 4.f, 0.75f);
			break;
		case 2:
			Renderer::line(screen_center, Vector2{ screen_center.x + 4, screen_center.y + 4 }, Color3(255, 255, 255), false, 0.7f);
			Renderer::line(screen_center, Vector2{ screen_center.x + 4, screen_center.y - 4 }, Color3(255, 255, 255), false, 0.7f);
			Renderer::line(screen_center, Vector2{ screen_center.x - 4, screen_center.y - 4 }, Color3(255, 255, 255), false, 0.7f);
			Renderer::line(screen_center, Vector2{ screen_center.x - 4, screen_center.y + 4 }, Color3(255, 255, 255), false, 0.7f);

			Renderer::line(Vector2{ screen_center.x + 4, screen_center.y + 4 }, Vector2{ screen_center.x + 4 + 4, screen_center.y + 4 + 4 }, Color3(0, 0, 0), false, 0.7f);
			Renderer::line(Vector2{ screen_center.x + 4, screen_center.y - 4 }, Vector2{ screen_center.x + 4 + 4, screen_center.y - 4 - 4 }, Color3(0, 0, 0), false, 0.7f);
			Renderer::line(Vector2{ screen_center.x - 4, screen_center.y - 4 }, Vector2{ screen_center.x - 4 - 4, screen_center.y - 4 - 4 }, Color3(0, 0, 0), false, 0.7f);
			Renderer::line(Vector2{ screen_center.x - 4, screen_center.y + 4 }, Vector2{ screen_center.x - 4 - 4, screen_center.y + 4 + 4 }, Color3(0, 0, 0), false, 0.7f);
			break;
		case 3:
			break;
		default:
			break;
		}
		

		auto local = LocalPlayer::Entity( );
		if (local == nullptr || !local->isCached( )) {
			target_ply = nullptr;
			return;
		}

		auto playerList = BasePlayer::visiblePlayerList( );
		if (!playerList) {
			target_ply = nullptr;
			return;
		}

		if (playerList->vals->size <= 1) {
			target_ply = nullptr;
			return;
		}

		if (target_ply != nullptr) {
			if (!target_ply->IsValid( ) || target_ply->health( ) <= 0 || target_ply->HasPlayerFlag(PlayerFlags::Sleeping) || (target_ply->playerModel( )->isNpc( ) && !settings::npcs))
				target_ply = nullptr;
			else
				if (target_ply->isCached( )) {
					auto bounds = target_ply->bones( )->bounds;
					if (!bounds.empty( ))
						Renderer::line({ bounds.left + ((bounds.right - bounds.left) / 2), bounds.bottom }, { screen_center.x, screen_size.y }, Color3(255, 0, 0), true);

					if (settings::manipulator && !other::m_manipulate.empty( ))
						Renderer::boldtext({ screen_center.x - 20, screen_center.y - 20 }, Color3(200, 0, 0), 12.f, true, true, wxorstr_(L"[m]"));

					if (target_ply->find_mpv_bone( )->visible)
						Renderer::boldtext({ screen_center.x + 20, screen_center.y - 20 }, Color3(66, 135, 245), 12.f, true, true, wxorstr_(L"[s]"));

					Renderer::boldtext({ screen_center.x - 20, screen_center.y + 20 }, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"[t]"));
				}
		}
		if (settings::tr::desyncing) {
			Renderer::text({ screen_center.x, screen_center.y + 150 }, Color3(173, 0, 0), 13.5f, true, true, wxorstr_(L"desync'ed"));
		}

		auto held = local->GetHeldEntity<BaseProjectile>( );
		if (held) {
			if (held->HasReloadCooldown( ) && held->class_name_hash( ) != STATIC_CRC32("BowWeapon") && held->class_name_hash( ) != STATIC_CRC32("CompoundBowWeapon")) { // im sorry for my sins
				float time_left = held->nextReloadTime( ) - GLOBAL_TIME;
				float time_full = held->CalculateCooldownTime(held->nextReloadTime( ), held->reloadTime( )) - GLOBAL_TIME;

				Renderer::rectangle_filled({ screen_center.x - 26, screen_center.y + 30 }, { 51, 5 }, Color3(0, 0, 0));
				Renderer::rectangle_filled({ screen_center.x - 25, screen_center.y + 31 }, { 50 * (time_left / time_full), 4 }, Color3(0, 255, 0));
				Renderer::text({ (screen_center.x - 25) + (50 * (time_left / time_full)), screen_center.y + 31 + 2 }, Color3(255, 255, 255), 12.f, true, true, wxorstr_(L"%d"), (int)ceil(time_left));
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
				int y_ = 0;

				float box_width = bounds.right - bounds.left;
				float box_height = bounds.bottom - bounds.top;
				Vector2 footPos = { bounds.left + (box_width / 2), bounds.bottom + 7.47f };
				Vector2 headPos = { bounds.left + (box_width / 2), bounds.top - 9.54f };

				Color3 col_c = player->is_target( ) ? player->is_visible( ) ? Color3(255, 0, 0) : Color3(184, 0, 0) : player->playerModel( )->isNpc( ) ? Color3(71, 209, 255) : player->is_visible( ) ? Color3(255, 255, 255) : Color3(186, 186, 186);
				Color3 col = Color3(col_c.r, col_c.g, col_c.b/*, 255 - (player->bones()->head->position.distance(local->bones()->head->position) / 2.5)*/);

				Renderer::text(headPos, col, 12.f, true, true, wxorstr_(L"%s [%dhp]"), player->_displayName( ), (int)ceil(player->health( )));

				if (settings::look_dir)
					Renderer::line(player->bones( )->dfc, player->bones( )->forward, col, true);

				Renderer::line({ bounds.left, bounds.top }, { bounds.left + (box_width / 3.5f), bounds.top }, col, true, 1.5f);
				Renderer::line({ bounds.right, bounds.top }, { bounds.right - (box_width / 3.5f), bounds.top }, col, true, 1.5f);

				Renderer::line({ bounds.left, bounds.bottom }, { bounds.left + (box_width / 3.5f), bounds.bottom }, col, true, 1.5f);
				Renderer::line({ bounds.right, bounds.bottom }, { bounds.right - (box_width / 3.5f), bounds.bottom }, col, true, 1.5f);

				Renderer::line({ bounds.left, bounds.top }, { bounds.left, bounds.top + (box_width / 3.5f) }, col, true, 1.5f);
				Renderer::line({ bounds.right, bounds.top }, { bounds.right, bounds.top + (box_width / 3.5f) }, col, true, 1.5f);

				Renderer::line({ bounds.left, bounds.bottom }, { bounds.left, bounds.bottom - (box_width / 3.5f) }, col, true, 1.5f);
				Renderer::line({ bounds.right, bounds.bottom }, { bounds.right, bounds.bottom - (box_width / 3.5f) }, col, true, 1.5f);

				if (player->GetHeldItem( )) {
					Renderer::text(footPos, col, 12.f, true, true, player->GetHeldItem( )->info( )->displayName( )->english( ));
					y_ += 16;
				}
				if (player->HasPlayerFlag(PlayerFlags::Wounded)) {
					Renderer::boldtext(footPos + Vector2(0, y_), Color3(255, 0, 0, 255), 12.f, true, true, wxorstr_(L"*wounded*"));
					y_ += 16;
				}

				if (target_ply == nullptr)
					target_ply = player;
				else
					if (dfc(target_ply) > dfc(player))
						target_ply = player;
			}
		}
	}
	void gamethread( ) {
		auto playerList = BasePlayer::visiblePlayerList( );
		if (playerList) {
			for (int i = 0; i < playerList->vals->size; i++) {
				auto ppp = *reinterpret_cast<BasePlayer**>(std::uint64_t(playerList->vals->buffer) + (0x20 + (sizeof(void*) * i)));

				if (!ppp) continue;
				if (!ppp->IsValid( )) continue;
				if (!ppp->isCached( )) continue;
				if (ppp->health( ) <= 0.0f) continue;
				if (ppp->HasPlayerFlag(PlayerFlags::Sleeping)) continue;
				if (ppp->playerModel( )->isNpc( ) && !settings::npcs) continue;
				if (ppp->userID( ) == LocalPlayer::Entity( )->userID( )) continue;

				if (settings::chams) {
					/*static auto bundle = AssetBundle::LoadFromFile(xorstr_("C:/Users/sduihfoqawhf/Downloads/shadersbundle"));
					static auto shader_b = bundle->LoadAsset< Shader >(xorstr_("assets/assets/resources/chamsshader.shader"), Type::Shader( ));

					auto renderer_list = ppp->playerModel( )->_multiMesh( )->Renderers( );
					if (renderer_list && shader_b && bundle) {
						for (int j = 0; j < renderer_list->size; j++) {
							auto renderer = (Renderer_*)renderer_list->get(j);
							if (!renderer)
								continue;

							auto material = renderer->material( );
							if (!material)
								continue;

							auto shader = material->shader( );
							if (!shader)
								continue;

							if (shader == shader_b)
								continue;

							material->set_shader(shader_b);
						}
					}*/
					auto renderer_list = ppp->playerModel( )->_multiMesh( )->Renderers( );
					if (renderer_list) {
						for (int j = 0; j < renderer_list->size; j++) {
							auto renderer = (Renderer_*)renderer_list->get(j);
							if (!renderer)
								continue;

							auto material = renderer->material( );
							if (!material)
								continue;

							auto shader = material->shader( );
							if (!shader)
								continue;

							material->set_shader(nullptr);
						}
					}
				}
			}
		}
	}
}