namespace entities {
	namespace belt {
		bool should_drag = false;
		bool should_move = false;

		POINT cursor;
		POINT cursor_corrected;
		void belt_tab_mov(int& x, int& y, int w, int h) {
			GetCursorPos(&belt::cursor);

			if (GetAsyncKeyState(VK_LBUTTON) < 0 && (belt::cursor.x > x && belt::cursor.x < x + w && belt::cursor.y > y && belt::cursor.y < y + h)) {
				belt::should_drag = true;

				if (!belt::should_move) {
					belt::cursor_corrected.x = belt::cursor.x - x;
					belt::cursor_corrected.y = belt::cursor.y - y;
					belt::should_move = true;
				}
			}

			if (belt::should_drag) {
				x = belt::cursor.x - belt::cursor_corrected.x;
				y = belt::cursor.y - belt::cursor_corrected.y;
			}

			if (GetAsyncKeyState(VK_LBUTTON) == 0) {
				belt::should_drag = false;
				belt::should_move = false;
			}
		}
	}

	float dfc(BasePlayer* player) {
		if (!player)
			return 1000.f;

		if (!player->isCached( ))
			return 1000.f;

		if (player->bones( )->dfc.empty( ))
			return 1000.f;

		return screen_center.distance_2d(player->bones( )->dfc);
	}
	Color3 get_color(BasePlayer* player) {
		if (!player->playerModel( )->isNpc( )) {
			if (player->is_target( ))
				if (player->is_visible( ))
					return Color3(255, 0, 0);
				else
					return Color3(184, 0, 0);
			else
				if (player->is_teammate( ))
					if (player->is_visible( ))
						return Color3(53, 232, 21);
					else
						return Color3(31, 153, 9);
				else
					if (player->is_visible( ))
						return Color3(255, 255, 255);
					else
						return Color3(186, 186, 186);
		}
		else {
			if (!player->is_target( ))
				if (player->is_visible( ))
					return Color3(71, 209, 255);
				else
					return Color3(7, 124, 163);
			else
				if (player->is_visible( ))
					return Color3(255, 0, 0);
				else
					return Color3(184, 0, 0);
		}
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
			Renderer::circle(screen_center, Color3(0, 0, 0), 2.f, 1.f);
			Renderer::circle(screen_center, Color3(0, 0, 0), 4.f, 1.f);
			Renderer::circle(screen_center, Color3(255, 255, 255), 3.f, 1.f);
			break;
		default:
			break;
		}

		if (settings::tr::desyncing) {
			Renderer::text({ screen_center.x, screen_center.y + 150 }, Color3(173, 0, 0), 13.5f, true, true, wxorstr_(L"desync'ed"));
		}

		auto local = LocalPlayer::Entity( );
		if (local == nullptr) {
			target_ply = nullptr;
			return;
		}


		if (settings::reload_indicator) {
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
		}
		if (settings::players) {
			auto entityList = BaseNetworkable::clientEntities( )->entityList( );
			if (!entityList) {
				target_ply = nullptr;
				return;
			}

			if (entityList->vals->size <= 1) {
				target_ply = nullptr;
				return;
			}

			viewMatrix = Camera::getViewMatrix( );

			if (target_ply != nullptr) {
				if (target_ply->isCached( )) {
					auto bounds = target_ply->bones( )->bounds;
					if (!bounds.empty( ))
						Renderer::line({ bounds.left + ((bounds.right - bounds.left) / 2), bounds.bottom }, { screen_center.x, screen_size.y }, Color3(255, 0, 0), true);

					if (settings::manipulator && !other::m_manipulate.empty( ))
						Renderer::boldtext({ screen_center.x - 20, screen_center.y - 20 }, Color3(200, 0, 0), 12.f, true, true, wxorstr_(L"[m]"));

					auto mpv = target_ply->find_mpv_bone( );
					Bone* target;
					if (mpv != nullptr)
						target = mpv;
					else
						target = target_ply->bones( )->head;

					if (target->visible)
						Renderer::boldtext({ screen_center.x + 20, screen_center.y - 20 }, Color3(66, 135, 245), 12.f, true, true, wxorstr_(L"[s]"));

					if (settings::desync && target_ply->bones( )->desyncable)
						Renderer::boldtext({ screen_center.x + 20, screen_center.y + 20 }, Color3(173, 0, 0), 12.f, true, true, wxorstr_(L"[d]"));

					Renderer::boldtext({ screen_center.x - 20, screen_center.y + 20 }, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"[t]"));

					if (settings::belt && !settings::menu) {
						int w = 200, h = 102;

						belt::belt_tab_mov(settings::g::b_x, settings::g::b_y, w, -20);

						Renderer::rectangle_filled({ float(settings::g::b_x), settings::g::b_y - float(20) }, Vector2(w, 20), Color3(25, 25, 25));
						Renderer::rectangle_filled(Vector2(settings::g::b_x, settings::g::b_y), Vector2(w, h), Color3(36, 36, 36));
						Renderer::rectangle_filled(Vector2(settings::g::b_x + float(5), settings::g::b_y + float(5)), Vector2(w - 10, h - 10), Color3(25, 25, 25));

						Renderer::text({ settings::g::b_x + float(7), settings::g::b_y - float(16) }, Color3(255, 255, 255), 12.f, false, false, target_ply->_displayName( ));

						auto list = target_ply->inventory( )->containerBelt( )->itemList( );
						if (list) {
							int y = 0;
							for (int i = 0; i < list->size; i++) {
								auto item = (Item*)list->get(i);
								if (!item)
									continue;

								Color3 col = item->uid( ) == target_ply->clActiveItem( ) ? Color3(255, 0, 0) : Color3(255, 255, 255);

								Renderer::text({ settings::g::b_x + float(7), settings::g::b_y + float(7) + y }, col, 12.f, false, false, wxorstr_(L"%s [x%d]"), item->info( )->displayName( )->english( ), item->amount( ));

								y += 15;
							}
						}
					}
				}
			}

			for (int i = 0; i < entityList->vals->size; i++) {
				auto entity = *reinterpret_cast<BaseEntity**>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
				if (!entity) continue;

				if (!entity->IsValid( )) continue;

				if (settings::debug) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform( )->position( ), screen))
						Renderer::text(screen, Color3(0, 255, 0), 12.f, true, true, wxorstr_(L"%s"), StringConverter::ToUnicode(entity->class_name( )).c_str( ));
				}

				if (entity->class_name_hash( ) == STATIC_CRC32("BasePlayer") 
					|| entity->class_name_hash( ) == STATIC_CRC32("ScientistNPCNew")
					|| entity->class_name_hash() == STATIC_CRC32("Scientist")) {

					auto player = reinterpret_cast<BasePlayer*>(entity);

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

						Color3 col = get_color(player);

						if (settings::skeleton) {
							float dist = player->bones( )->head->position.distance(local->bones( )->head->position);
							if (dist < 175.f) {

								auto info = player->bones( );

								auto head_b = info->head;
								auto spine4_b = info->spine4;
								auto l_upperarm_b = info->l_upperarm;
								auto l_forearm_b = info->l_forearm;
								auto l_hand_b = info->l_hand;
								auto r_upperarm_b = info->r_upperarm;
								auto r_forearm_b = info->r_forearm;
								auto r_hand_b = info->r_hand;
								auto pelvis_b = info->pelvis;
								auto l_hip_b = info->l_hip;
								auto l_knee_b = info->l_knee;
								auto l_foot_b = info->l_foot;
								auto r_hip_b = info->r_hip;
								auto r_knee_b = info->r_knee;
								auto r_foot_b = info->r_foot;
								auto r_toe_b = info->r_toe;
								auto l_toe_b = info->l_toe;

								Vector2 head, spine, l_upperarm, l_forearm, l_hand, r_upperarm, r_forearm, r_hand, pelvis, l_hip, l_knee, l_foot, r_hip, r_knee, r_foot, l_toe, r_toe;
								if (Camera::world_to_screen(head_b->position, head) &&
									Camera::world_to_screen(spine4_b->position, spine) &&
									Camera::world_to_screen(l_upperarm_b->position, l_upperarm) &&
									Camera::world_to_screen(l_forearm_b->position, l_forearm) &&
									Camera::world_to_screen(l_hand_b->position, l_hand) &&
									Camera::world_to_screen(r_upperarm_b->position, r_upperarm) &&
									Camera::world_to_screen(r_forearm_b->position, r_forearm) &&
									Camera::world_to_screen(r_hand_b->position, r_hand) &&
									Camera::world_to_screen(pelvis_b->position, pelvis) &&
									Camera::world_to_screen(l_hip_b->position, l_hip) &&
									Camera::world_to_screen(l_knee_b->position, l_knee) &&
									Camera::world_to_screen(l_foot_b->position, l_foot) &&
									Camera::world_to_screen(r_hip_b->position, r_hip) &&
									Camera::world_to_screen(r_knee_b->position, r_knee) &&
									Camera::world_to_screen(r_toe_b->position, r_toe) &&
									Camera::world_to_screen(l_toe_b->position, l_toe) &&
									Camera::world_to_screen(r_foot_b->position, r_foot)) {

									Color3 viscol = player->playerModel( )->isNpc( ) ? Color3(38, 255, 0) : Color3(0, 250, 255);
									Color3 inviscol = player->playerModel( )->isNpc( ) ? Color3(22, 145, 0) : Color3(0, 152, 156);

									Renderer::line(head, spine, (head_b->visible || spine4_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(spine, l_upperarm, (spine4_b->visible || l_upperarm_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(l_upperarm, l_forearm, (l_upperarm_b->visible || l_forearm_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(l_forearm, l_hand, (l_forearm_b->visible || l_hand_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(spine, r_upperarm, (spine4_b->visible || r_upperarm_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(r_upperarm, r_forearm, (r_upperarm_b->visible || r_forearm_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(r_forearm, r_hand, (r_forearm_b->visible || r_hand_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(spine, pelvis, (spine4_b->visible || pelvis_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(pelvis, l_hip, (pelvis_b->visible || l_hip_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(l_hip, l_knee, (l_hip_b->visible || l_knee_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(l_knee, l_foot, (l_knee_b->visible || l_foot_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(pelvis, r_hip, (pelvis_b->visible || r_hip_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(r_hip, r_knee, (r_hip_b->visible || r_knee_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(r_knee, r_foot, (r_knee_b->visible || r_foot_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(r_foot, r_toe, (r_foot_b->visible || r_toe_b->visible) ? viscol : inviscol, 3.f);
									Renderer::line(l_foot, l_toe, (l_foot_b->visible || l_toe_b->visible) ? viscol : inviscol, 3.f);
								}
							}
						}

						Renderer::text(headPos, col, 12.f, true, true, wxorstr_(L"%s [%dhp]"), player->_displayName( ), (int)ceil(player->health( )));

						if (settings::look_dir)
							Renderer::line(player->bones( )->dfc, player->bones( )->forward, col, true);

						if (settings::box) {
							Renderer::line({ bounds.left, bounds.top }, { bounds.left + (box_width / 3.5f), bounds.top }, col, true, 1.5f);
							Renderer::line({ bounds.right, bounds.top }, { bounds.right - (box_width / 3.5f), bounds.top }, col, true, 1.5f);

							Renderer::line({ bounds.left, bounds.bottom }, { bounds.left + (box_width / 3.5f), bounds.bottom }, col, true, 1.5f);
							Renderer::line({ bounds.right, bounds.bottom }, { bounds.right - (box_width / 3.5f), bounds.bottom }, col, true, 1.5f);

							Renderer::line({ bounds.left, bounds.top }, { bounds.left, bounds.top + (box_width / 3.5f) }, col, true, 1.5f);
							Renderer::line({ bounds.right, bounds.top }, { bounds.right, bounds.top + (box_width / 3.5f) }, col, true, 1.5f);

							Renderer::line({ bounds.left, bounds.bottom }, { bounds.left, bounds.bottom - (box_width / 3.5f) }, col, true, 1.5f);
							Renderer::line({ bounds.right, bounds.bottom }, { bounds.right, bounds.bottom - (box_width / 3.5f) }, col, true, 1.5f);
						}

						if (player->GetHeldItem( )) {
							Renderer::text(footPos, col, 12.f, true, true, player->GetHeldItem( )->info( )->displayName( )->english( ));
							y_ += 16;
						}
						if (player->HasPlayerFlag(PlayerFlags::Wounded)) {
							Renderer::boldtext(footPos + Vector2(0, y_), Color3(255, 0, 0, 255), 12.f, true, true, wxorstr_(L"*wounded*"));
							y_ += 16;
						}
						if (player->HasPlayerFlag(PlayerFlags::Aiming)) {
							Renderer::boldtext(footPos + Vector2(0, y_), Color3(255, 127, 0, 255), 12.f, true, true, wxorstr_(L"*aiming*"));
							y_ += 16;
						}

						if (!player->is_teammate( )) {
							if (dfc(player) < settings::targeting_fov) {
								if (target_ply == nullptr)
									target_ply = player;
								else
									if (dfc(target_ply) > dfc(player))
										target_ply = player;
							}
						}
					}
				}
			}
		}
		else {
			if (target_ply != nullptr)
				target_ply = nullptr;
		}
	}
}