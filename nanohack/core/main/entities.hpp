namespace entities {
	namespace belt {
		Vector2 pos = Vector2(200, 200);
		bool should_drag = false;
		bool should_move = false;

		POINT cursor;
		POINT cursor_corrected;
		void belt_tab_mov(Vector2 size) {
			GetCursorPos(&cursor);

			if (GetAsyncKeyState(VK_LBUTTON) && (cursor.x > pos.x && cursor.y > pos.y && cursor.x < pos.x + size.x && cursor.y < pos.y + size.y)) {
				should_drag = true;
				if (!should_move) {
					cursor_corrected.x = cursor.x - pos.x;
					cursor_corrected.y = cursor.y - pos.y;
					should_move = true;
				}
			}

			if (should_drag) {
				pos.x = cursor.x - cursor_corrected.x;
				pos.y = cursor.y - cursor_corrected.y;
			}

			if (GetAsyncKeyState(VK_LBUTTON) == 0) {
				should_drag = false;
				should_move = false;
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
	Color3 get_color(BasePlayer* player, bool boxes = false) {
		if (!boxes) {
			if (player->HasPlayerFlag(PlayerFlags::Sleeping)) {
				if (player->is_visible( ))
					return plusminus::ui::get_color("visible sleepers");
				else
					return plusminus::ui::get_color("invisible sleepers");
			}

			if (!player->playerModel( )->isNpc( )) {
				if (player->is_target( ))
					if (player->is_visible( ))
						return plusminus::ui::get_color("visible players");
					else
						return plusminus::ui::get_color("invisible players");
				else
					if (player->is_teammate( ))
						if (player->is_visible( ))
							return plusminus::ui::get_color("visible teammate");
						else
							return plusminus::ui::get_color("invisible teammate");
					else
						if (player->is_visible( ))
							return plusminus::ui::get_color("visible players");
						else
							return plusminus::ui::get_color("invisible players");
			}
			else {
				if (!player->is_target( ))
					if (player->is_visible( ))
						return plusminus::ui::get_color("visible npcs");
					else
						return plusminus::ui::get_color("invisible npcs");
				else
					if (player->is_visible( ))
						return plusminus::ui::get_color("visible npcs");
					else
						return plusminus::ui::get_color("invisible npcs");
			}
		}

		if (boxes) {
			if (player->is_target( ))
				return plusminus::ui::get_color("targeted boxes");
			else
				if (player->is_visible( ))
					return plusminus::ui::get_color("visible boxes");
				else
					return plusminus::ui::get_color("invisible boxes");
		}
	}

	float BOG_TO_GRD(float BOG) {
		return (180 / M_PI) * BOG;
	}

	float GRD_TO_BOG(float GRD) {
		return (M_PI / 180) * GRD;
	}

	__forceinline uint32_t RandomInteger(uint32_t Min, uint32_t Max)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		const std::uniform_int_distribution<uint32_t> distr(Min, Max);
		return distr(eng);
	}

	void loop( ) {
		static Color3 clr = Color3(RandomInteger(100, 255), RandomInteger(100, 255), RandomInteger(100, 255), 255);
		static float faken_rot = 0.0f;
		static int gamerjuice = 0;
		float a = screen_center.y / 30.0f;
		float gamma = atan(a / a);

		switch (plusminus::ui::get_combobox(xorstr_("crosshair"))) {
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
		case 4:

			if ((int)faken_rot > 89) { faken_rot = (float)0; }
			faken_rot++;

			if (gamerjuice > 30)
			{
				gamerjuice = 0;
				clr = Color3(RandomInteger(0, 255), RandomInteger(0, 255), RandomInteger(0, 255), 255);
			}
			else
				gamerjuice++;

			for (int i = 0; i < 4; i++)
			{
				std::vector <int> p;
				p.push_back(a * sin(GRD_TO_BOG(faken_rot + (i * 90))));									//p[0]		p0_A.x
				p.push_back(a * cos(GRD_TO_BOG(faken_rot + (i * 90))));									//p[1]		p0_A.y
				p.push_back((a / cos(gamma)) * sin(GRD_TO_BOG(faken_rot + (i * 90) + BOG_TO_GRD(gamma))));	//p[2]		p0_B.x
				p.push_back((a / cos(gamma)) * cos(GRD_TO_BOG(faken_rot + (i * 90) + BOG_TO_GRD(gamma))));	//p[3]		p0_B.y


				Renderer::line(screen_center, { screen_center.x + p[0], screen_center.y - p[1] }, clr, true);
				Renderer::line({ screen_center.x + p[0], screen_center.y - p[1] }, { screen_center.x + p[2], screen_center.y - p[3] }, clr, true);
			}
		default:
			break;
		}

		if (plusminus::ui::get_bool(xorstr_("target player belt")) && plusminus::ui::is_menu_open( )) {
			int w = 200, h = 102;

			belt::belt_tab_mov(Vector2(w, h + 20.0f));

			Renderer::rectangle_filled({ belt::pos.x, belt::pos.y }, Vector2(w, 20), Color3(25, 25, 25));
			Renderer::rectangle_filled(Vector2(belt::pos.x, belt::pos.y + 20.0f), Vector2(w, h), Color3(36, 36, 36));
			Renderer::rectangle_filled(Vector2(belt::pos.x + 5.0f, belt::pos.y + 20.0f + 5.0f), Vector2(w - 10, h - 10), Color3(25, 25, 25));
		}

		if (settings::tr::manipulated) {
			Renderer::text({ screen_center.x, screen_center.y + 150 }, Color3(173, 0, 0), 11.f, true, true, wxorstr_(L"[manipulated]"));
		}

		auto local = LocalPlayer::Entity( );
		if (local == nullptr) {
			target_ply = nullptr;
			return;
		}

		if (plusminus::ui::get_bool(xorstr_("reload indicator"))) {
			auto held = local->GetHeldEntity<BaseProjectile>( );
			if (held) {
				if (held->HasReloadCooldown( ) && held->class_name_hash( ) != STATIC_CRC32("BowWeapon") && held->class_name_hash( ) != STATIC_CRC32("CompoundBowWeapon")) { // im sorry for my sins
					float time_left = held->nextReloadTime( ) - GLOBAL_TIME;
					float time_full = held->CalculateCooldownTime(held->nextReloadTime( ), held->reloadTime( )) - GLOBAL_TIME;

					Renderer::rectangle_filled({ screen_center.x - 26, screen_center.y + 30 }, { 51, 5 }, Color3(0, 0, 0));
					Renderer::rectangle_filled({ screen_center.x - 25, screen_center.y + 31 }, { 50 * (time_left / time_full), 4 }, Color3(0, 255, 0));
					Renderer::text({ (screen_center.x - 25) + (50 * (time_left / time_full)), screen_center.y + 31 + 2 }, Color3(255, 255, 255), 12.f, true, true, wxorstr_(L"%d"), (int)ceil(time_left));
				}
				if (held->class_name_hash( ) == STATIC_CRC32("BaseProjectile") ||
					held->class_name_hash( ) == STATIC_CRC32("BowWeapon") ||
					held->class_name_hash( ) == STATIC_CRC32("CompoundBowWeapon") ||
					held->class_name_hash( ) == STATIC_CRC32("BaseLauncher") ||
					held->class_name_hash( ) == STATIC_CRC32("CrossbowWeapon")) {
					if (held->Empty( )) {
						Renderer::text({ screen_center.x, screen_center.y + 48 }, Color3(89, 227, 255), 12.f, true, true, wxorstr_(L"[empty weapon]"));
					}
				}
			}
		}
		if (plusminus::ui::get_bool(xorstr_("players")) ||
			plusminus::ui::get_bool(xorstr_("ores")) ||
			plusminus::ui::get_bool(xorstr_("stashes")) ||
			plusminus::ui::get_bool(xorstr_("corpses")) ||
			plusminus::ui::get_bool(xorstr_("traps")) ||
			plusminus::ui::get_bool(xorstr_("hemp")) ||
			plusminus::ui::get_bool(xorstr_("vehicles"))) {
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
				if (!target_ply->IsValid( ) || target_ply->health( ) <= 0 || target_ply->is_teammate( ) || target_ply->HasPlayerFlag(PlayerFlags::Sleeping) || entities::dfc(target_ply) > plusminus::ui::get_float(xorstr_("targeting fov")) || (target_ply->playerModel( )->isNpc( ) && !plusminus::ui::get_bool(xorstr_("npc")))) {
					target_ply = nullptr;
				}
				else {
					if (target_ply->isCached( )) {
						auto bounds = target_ply->bones( )->bounds;
						if (!bounds.empty( ))
							Renderer::line({ bounds.left + ((bounds.right - bounds.left) / 2), bounds.bottom }, { screen_center.x, screen_size.y }, Color3(255, 0, 0), true);

						auto mpv = target_ply->find_mpv_bone( );
						Bone* target;
						if (mpv != nullptr)
							target = mpv;
						else
							target = target_ply->bones( )->head;

						if (target->visible)
							Renderer::boldtext({ screen_center.x + 20, screen_center.y - 20 }, Color3(66, 135, 245), 12.f, true, true, wxorstr_(L"[s]"));

						Renderer::boldtext({ screen_center.x - 20, screen_center.y + 20 }, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"[t]"));

						if (plusminus::ui::get_bool(xorstr_("target player belt")) && !plusminus::ui::is_menu_open( )) {
							int w = 200, h = 102;

							belt::belt_tab_mov(Vector2(w, -20));

							Renderer::rectangle_filled({ belt::pos.x, belt::pos.y - 20.0f }, Vector2(w, 20), Color3(25, 25, 25));
							Renderer::rectangle_filled(Vector2(belt::pos.x, belt::pos.y), Vector2(w, h), Color3(36, 36, 36));
							Renderer::rectangle_filled(Vector2(belt::pos.x + 5.0f, belt::pos.y + 5.0f), Vector2(w - 10, h - 10), Color3(25, 25, 25));

							Renderer::text({ belt::pos.x + 7.0f, belt::pos.y - 16.0f }, Color3(255, 255, 255), 12.f, false, false, target_ply->_displayName( ));

							auto list = target_ply->inventory( )->containerBelt( )->itemList( );
							if (list) {
								if (list->size) {
									int y = 0;
									for (int i = 0; i < list->size; i++) {
										auto item = (Item*)list->get(i);
										if (!item)
											continue;

										Color3 col = item->uid( ) == target_ply->clActiveItem( ) ? Color3(255, 0, 0) : Color3(255, 255, 255);

										Renderer::text({ belt::pos.x + 7.0f, belt::pos.y + 7.0f + y }, col, 12.f, false, false, wxorstr_(L"%s [x%d]"), item->info( )->displayName( )->english( ), item->amount( ));

										y += 15;
									}
								}
							}
						}
					}
				}
			}

			for (int i = 0; i < entityList->vals->size; i++) {
				auto entity = *reinterpret_cast<BaseEntity**>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
				if (!entity) continue;

				if (!entity->IsValid( )) continue;

#ifndef authh
				if (plusminus::ui::get_bool(xorstr_("debug")) && entity->transform( )->position( ).distance(LocalPlayer::Entity( )->transform( )->position( )) <= 5.0f) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform( )->position( ), screen)) {
						Renderer::text(screen, Color3(0, 255, 0), 12.f, true, true, wxorstr_(L"%s"), StringConverter::ToUnicode(entity->class_name( )).c_str( ));
						Renderer::text(screen + Vector2(0, 15), Color3(0, 255, 0), 12.f, true, true, wxorstr_(L"%s"), entity->ShortPrefabName( ));
					}
				}
#endif
				if (entity->class_name_hash( ) == STATIC_CRC32("BasePlayer") ||
					entity->class_name_hash( ) == STATIC_CRC32("NPCPlayerApex") ||
					entity->class_name_hash( ) == STATIC_CRC32("NPCMurderer") ||
					entity->class_name_hash( ) == STATIC_CRC32("NPCPlayer") ||
					entity->class_name_hash( ) == STATIC_CRC32("HumanNPC") ||
					entity->class_name_hash( ) == STATIC_CRC32("Scientist") ||
					entity->class_name_hash( ) == STATIC_CRC32("HTNPlayer") ||
					entity->class_name_hash( ) == STATIC_CRC32("HumanNPCNew") ||
					entity->class_name_hash( ) == STATIC_CRC32("ScientistNPCNew") ||
					entity->class_name_hash( ) == STATIC_CRC32("TunnelDweller") ||
					entity->class_name_hash( ) == STATIC_CRC32("BanditGuard")) {
					if (!plusminus::ui::get_bool(xorstr_("players")) && !plusminus::ui::get_bool(xorstr_("sleepers"))) continue;
					auto player = reinterpret_cast<BasePlayer*>(entity);

					if (!player->isCached( )) continue;
					if (player->health( ) <= 0.0f) continue;
					if (player->HasPlayerFlag(PlayerFlags::Sleeping) && !plusminus::ui::get_bool(xorstr_("sleepers"))) continue;
					if (player->playerModel( )->isNpc( ) && !plusminus::ui::get_bool(xorstr_("npc"))) continue;
					if (player->userID( ) == LocalPlayer::Entity( )->userID( )) continue;

					auto bounds = player->bones( )->bounds;
					if (!bounds.empty( )) {
						int y_ = 0;

						float box_width = bounds.right - bounds.left;
						float box_height = bounds.bottom - bounds.top;
						Vector2 footPos = { bounds.left + (box_width / 2), bounds.bottom + 7.47f };
						Vector2 headPos = { bounds.left + (box_width / 2), bounds.top - 9.54f };

						Color3 col = get_color(player);
						Color3 box_col = get_color(player, true);

						if (plusminus::ui::get_bool(xorstr_("skeleton"))) {
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

									if (player->HasPlayerFlag(PlayerFlags::Sleeping)) {
										viscol = plusminus::ui::get_color("visible skeleton");
										inviscol = plusminus::ui::get_color("invisible skeleton");
									}
									else if (!player->playerModel( )->isNpc( )) {
										viscol = plusminus::ui::get_color("visible skeleton players");
										inviscol = plusminus::ui::get_color("invisible skeleton players");
									}
									else {
										viscol = plusminus::ui::get_color("visible skeleton npc");
										inviscol = plusminus::ui::get_color("invisible skeleton npc");
									}

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

						if (plusminus::ui::get_bool(xorstr_("looking direction")) && !player->HasPlayerFlag(PlayerFlags::Sleeping))
							Renderer::line(player->bones( )->dfc, player->bones( )->forward, plusminus::ui::get_color(xorstr_("looking direction color")), true);

						if (plusminus::ui::get_bool(xorstr_("box"))) {
							switch (plusminus::ui::get_combobox(xorstr_("box type"))) {
							case 0: // cornered
								Renderer::line({ bounds.left, bounds.top }, { bounds.left + (box_width / 3.5f), bounds.top }, box_col, true, 1.5f);
								Renderer::line({ bounds.right, bounds.top }, { bounds.right - (box_width / 3.5f), bounds.top }, box_col, true, 1.5f);

								Renderer::line({ bounds.left, bounds.bottom }, { bounds.left + (box_width / 3.5f), bounds.bottom }, box_col, true, 1.5f);
								Renderer::line({ bounds.right, bounds.bottom }, { bounds.right - (box_width / 3.5f), bounds.bottom }, box_col, true, 1.5f);

								Renderer::line({ bounds.left, bounds.top }, { bounds.left, bounds.top + (box_width / 3.5f) }, box_col, true, 1.5f);
								Renderer::line({ bounds.right, bounds.top }, { bounds.right, bounds.top + (box_width / 3.5f) }, box_col, true, 1.5f);

								Renderer::line({ bounds.left, bounds.bottom }, { bounds.left, bounds.bottom - (box_width / 3.5f) }, box_col, true, 1.5f);
								Renderer::line({ bounds.right, bounds.bottom }, { bounds.right, bounds.bottom - (box_width / 3.5f) }, box_col, true, 1.5f);
								break;
							case 1: // 2d
								Renderer::line({ bounds.left, bounds.top }, { bounds.right, bounds.top }, box_col, true, 1.5f);
								Renderer::line({ bounds.left, bounds.bottom }, { bounds.right, bounds.bottom }, box_col, true, 1.5f);

								Renderer::line({ bounds.left, bounds.top }, { bounds.left, bounds.bottom }, box_col, true, 1.5f);
								Renderer::line({ bounds.right, bounds.top }, { bounds.right, bounds.bottom }, box_col, true, 1.5f);
								break;
							case 2: // 3d (hippity hoppity your 3d box is now my property)
								CBounds bounds = CBounds( );

								if (player->IsDucked( )) {
									bounds.center = player->midPoint( ) + Vector3(0.0f, 0.55f, 0.0f);
									bounds.extents = Vector3(0.4f, 0.65f, 0.4f);
								}
								else {
									if (player->HasPlayerFlag(PlayerFlags::Wounded) || player->HasPlayerFlag(PlayerFlags::Sleeping)) {
										bounds.center = player->bones( )->pelvis->position;
										bounds.extents = Vector3(0.9f, 0.2f, 0.4f);
									}
									else {
										bounds.center = player->midPoint( ) + Vector3(0.0f, 0.85f, 0.0f);
										bounds.extents = Vector3(0.4f, 0.9f, 0.4f);
									}
								}

								float y = math::euler_angles(player->bones( )->eye_rot).y;
								Vector3 center = bounds.center;
								Vector3 extents = bounds.extents;
								Vector3 frontTopLeft = math::rotate_point(center, Vector3(center.x - extents.x, center.y + extents.y, center.z - extents.z), y);
								Vector3 frontTopRight = math::rotate_point(center, Vector3(center.x + extents.x, center.y + extents.y, center.z - extents.z), y);
								Vector3 frontBottomLeft = math::rotate_point(center, Vector3(center.x - extents.x, center.y - extents.y, center.z - extents.z), y);
								Vector3 frontBottomRight = math::rotate_point(center, Vector3(center.x + extents.x, center.y - extents.y, center.z - extents.z), y);
								Vector3 backTopLeft = math::rotate_point(center, Vector3(center.x - extents.x, center.y + extents.y, center.z + extents.z), y);
								Vector3 backTopRight = math::rotate_point(center, Vector3(center.x + extents.x, center.y + extents.y, center.z + extents.z), y);
								Vector3 backBottomLeft = math::rotate_point(center, Vector3(center.x - extents.x, center.y - extents.y, center.z + extents.z), y);
								Vector3 backBottomRight = math::rotate_point(center, Vector3(center.x + extents.x, center.y - extents.y, center.z + extents.z), y);

								Vector2 frontTopLeft_2d, frontTopRight_2d, frontBottomLeft_2d, frontBottomRight_2d, backTopLeft_2d, backTopRight_2d, backBottomLeft_2d, backBottomRight_2d;
								if (Camera::world_to_screen(frontTopLeft, frontTopLeft_2d) &&
									Camera::world_to_screen(frontTopRight, frontTopRight_2d) &&
									Camera::world_to_screen(frontBottomLeft, frontBottomLeft_2d) &&
									Camera::world_to_screen(frontBottomRight, frontBottomRight_2d) &&
									Camera::world_to_screen(backTopLeft, backTopLeft_2d) &&
									Camera::world_to_screen(backTopRight, backTopRight_2d) &&
									Camera::world_to_screen(backBottomLeft, backBottomLeft_2d) &&
									Camera::world_to_screen(backBottomRight, backBottomRight_2d)) {

									Renderer::line(frontTopLeft_2d, frontTopRight_2d, box_col, true, 1.5f);
									Renderer::line(frontTopRight_2d, frontBottomRight_2d, box_col, true, 1.5f);
									Renderer::line(frontBottomRight_2d, frontBottomLeft_2d, box_col, true, 1.5f);
									Renderer::line(frontBottomLeft_2d, frontTopLeft_2d, box_col, true, 1.5f);
									Renderer::line(backTopLeft_2d, backTopRight_2d, box_col, true, 1.5f);
									Renderer::line(backTopRight_2d, backBottomRight_2d, box_col, true, 1.5f);
									Renderer::line(backBottomRight_2d, backBottomLeft_2d, box_col, true, 1.5f);
									Renderer::line(backBottomLeft_2d, backTopLeft_2d, box_col, true, 1.5f);
									Renderer::line(frontTopLeft_2d, backTopLeft_2d, box_col, true, 1.5f);
									Renderer::line(frontTopRight_2d, backTopRight_2d, box_col, true, 1.5f);
									Renderer::line(frontBottomRight_2d, backBottomRight_2d, box_col, true, 1.5f);
									Renderer::line(frontBottomLeft_2d, backBottomLeft_2d, box_col, true, 1.5f);
								}
								break;
							}
						}

						if (player->GetHeldItem( ) && !player->HasPlayerFlag(PlayerFlags::Sleeping)) {
							Renderer::text(footPos, col, 12.f, true, true, player->GetHeldItem( )->info( )->displayName( )->english( ));
							y_ += 16;
						}
						if (player->HasPlayerFlag(PlayerFlags::Wounded)) {
							Renderer::boldtext(footPos + Vector2(0, y_), Color3(255, 0, 0, 255), 12.f, true, true, wxorstr_(L"*wounded*"));
							y_ += 16;
						}

						if (!player->is_teammate( ) && !player->HasPlayerFlag(PlayerFlags::Sleeping)) {
							if (dfc(player) < plusminus::ui::get_float(xorstr_("targeting fov"))) {
								if (target_ply == nullptr)
									target_ply = player;
								else
									if (dfc(target_ply) > dfc(player))
										target_ply = player;
							}
						}
					}
				}

				if (entity->class_name_hash( ) == STATIC_CRC32("OreResourceEntity") && plusminus::ui::get_bool(xorstr_("ores"))) {
					if (entity->ShortPrefabName_hash( ) == STATIC_CRC32("stone-ore") ||
						entity->ShortPrefabName_hash( ) == STATIC_CRC32("sulfur-ore") ||
						entity->ShortPrefabName_hash( ) == STATIC_CRC32("metal-ore")) {
						Vector2 screen;
						if (Camera::world_to_screen(entity->transform( )->position( ), screen)) {
							switch (entity->ShortPrefabName_hash( )) {
							case STATIC_CRC32("stone-ore"):
								Renderer::text(screen, plusminus::ui::get_color(xorstr_("ores color")), 12.f, true, true, wxorstr_(L"Stone Ore"));
								break;
							case STATIC_CRC32("sulfur-ore"):
								Renderer::text(screen, plusminus::ui::get_color(xorstr_("ores color")), 12.f, true, true, wxorstr_(L"Sulfur Ore"));
								break;
							case STATIC_CRC32("metal-ore"):
								Renderer::text(screen, plusminus::ui::get_color(xorstr_("ores color")), 12.f, true, true, wxorstr_(L"Metal Ore"));
								break;
							}
						}
					}
				}
				if (entity->ShortPrefabName_hash( ) == STATIC_CRC32("small_stash_deployed") && plusminus::ui::get_bool(xorstr_("stashes"))) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform( )->position( ), screen)) {
						if (entity->HasFlag(BaseEntity::Flags::Reserved5)) {
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("stashes color")), 12.f, true, true, wxorstr_(L"Stash (Hidden)"));
						}
						else {
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("stashes color")), 12.f, true, true, wxorstr_(L"Stash"));
						}
					}

				}
				if ((entity->ShortPrefabName_hash( ) == STATIC_CRC32("rhib") ||
					entity->ShortPrefabName_hash( ) == STATIC_CRC32("rowboat") ||
					entity->ShortPrefabName_hash( ) == STATIC_CRC32("hotairballoon") ||
					entity->ShortPrefabName_hash( ) == STATIC_CRC32("minicopter.entity") ||
					entity->ShortPrefabName_hash( ) == STATIC_CRC32("scraptransporthelicopter") ||
					entity->ShortPrefabName_hash( ) == STATIC_CRC32("submarineduo.entity") ||
					entity->ShortPrefabName_hash( ) == STATIC_CRC32("submarinesolo.entity") ||
					entity->ShortPrefabName_hash( ) == STATIC_CRC32("testridablehorse")) && plusminus::ui::get_bool(xorstr_("vehicles"))) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform( )->position( ), screen)) {
						switch (entity->ShortPrefabName_hash( )) {
						case STATIC_CRC32("rhib"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("vehicles color")), 12.f, true, true, wxorstr_(L"RHIB"));
							break;
						case STATIC_CRC32("rowboat"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("vehicles color")), 12.f, true, true, wxorstr_(L"Boat"));
							break;
						case STATIC_CRC32("hotairballoon"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("vehicles color")), 12.f, true, true, wxorstr_(L"Hot Air Balloon"));
							break;
						case STATIC_CRC32("minicopter.entity"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("vehicles color")), 12.f, true, true, wxorstr_(L"Minicopter"));
							break;
						case STATIC_CRC32("scraptransporthelicopter"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("vehicles color")), 12.f, true, true, wxorstr_(L"Scrap Heli"));
							break;
						case STATIC_CRC32("testridablehorse"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("vehicles color")), 12.f, true, true, wxorstr_(L"Horse"));
							break;
						case STATIC_CRC32("submarineduo.entity"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("vehicles color")), 12.f, true, true, wxorstr_(L"Submarine (Duo)"));
							break;
						case STATIC_CRC32("submarinesolo.entity"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("vehicles color")), 12.f, true, true, wxorstr_(L"Submarine (Solo)"));
							break;
						}
					}
				}
				if (entity->ShortPrefabName_hash( ) == STATIC_CRC32("hemp-collectable") && plusminus::ui::get_bool(xorstr_("hemp"))) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform( )->position( ), screen)) {
						Renderer::text(screen, plusminus::ui::get_color(xorstr_("hemp color")), 12.f, true, true, wxorstr_(L"Hemp"));
					}

				}
				if ((entity->class_name_hash( ) == STATIC_CRC32("PlayerCorpse") ||
					entity->class_name_hash( ) == STATIC_CRC32("NPCPlayerCorpse")) && plusminus::ui::get_bool(xorstr_("corpses"))) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform( )->position( ), screen)) {
						Renderer::text(screen, plusminus::ui::get_color(xorstr_("corpses color")), 12.f, true, true, wxorstr_(L"Corpse"));
					}
				}
				if ((entity->class_name_hash( ) == STATIC_CRC32("AutoTurret") ||
					entity->class_name_hash( ) == STATIC_CRC32("Landmine") ||
					entity->class_name_hash( ) == STATIC_CRC32("BearTrap") ||
					entity->class_name_hash( ) == STATIC_CRC32("SamSite") ||
					entity->class_name_hash( ) == STATIC_CRC32("GunTrap")) && plusminus::ui::get_bool(xorstr_("traps"))) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform( )->position( ), screen)) {
						switch (entity->class_name_hash( )) {
						case STATIC_CRC32("AutoTurret"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("traps color")), 12.f, true, true, wxorstr_(L"Auto Turret"));
							break;
						case STATIC_CRC32("Landmine"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("traps color")), 12.f, true, true, wxorstr_(L"Landmine"));
							break;
						case STATIC_CRC32("BearTrap"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("traps color")), 12.f, true, true, wxorstr_(L"Bear Trap"));
							break;
						case STATIC_CRC32("SamSite"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("traps color")), 12.f, true, true, wxorstr_(L"SAM Site"));
							break;
						case STATIC_CRC32("GunTrap"):
							Renderer::text(screen, plusminus::ui::get_color(xorstr_("traps color")), 12.f, true, true, wxorstr_(L"Shotgun Trap"));
							break;
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