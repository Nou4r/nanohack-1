#include <intrin.h>
#define CALLED_BY(func,off) (reinterpret_cast<std::uint64_t>(_ReturnAddress()) > func && reinterpret_cast<std::uint64_t>(_ReturnAddress()) < func + off)

void ClientUpdate_hk(BasePlayer* player) {
	auto local = LocalPlayer::Entity( );
	if (local) {
		if (plusminus::ui::get_bool(xorstr_("players")) || plusminus::ui::get_bool(xorstr_("sleepers"))) {
			bonecache::cachePlayer(player);
		}
	}
	return player->ClientUpdate( );
}

void ClientUpdate_Sleeping_hk(BasePlayer* player)
{
	auto local = LocalPlayer::Entity();
	if (local) {
		if (plusminus::ui::get_bool(xorstr_("players")) || plusminus::ui::get_bool(xorstr_("sleepers"))) {
			bonecache::cachePlayer(player);
		}
	}

	return player->ClientUpdate_Sleeping();
}

Vector3 GetModifiedAimConeDirection_hk(float aimCone, Vector3 inputVec, bool anywhereInside = true) {
	if (plusminus::ui::get_bool(xorstr_("psilent")) && target_ply != nullptr && target_ply->isCached( )) {
		inputVec = (aimutils::get_prediction( ) - LocalPlayer::Entity( )->eyes( )->position( )).normalized( );
	}

	aimCone *= plusminus::ui::get_float(xorstr_("spread %")) / 100.0f;

	return AimConeUtil::GetModifiedAimConeDirection(aimCone, inputVec, anywhereInside);
}
Attack* BuildAttackMessage_hk(HitTest* self) {
	auto ret = self->BuildAttackMessage( );
	auto entity = BaseNetworkable::clientEntities()->Find<BasePlayer*>(ret->hitID());

	if (plusminus::ui::get_bool(xorstr_("always heli weakspot")))
	{
		if (entity->class_name_hash() == STATIC_CRC32("BaseHelicopter"))
		{
			if (entity->health() <= 5000.0f)
				ret->hitBone() = StringPool::Get(xorstr_("tail_rotor_col"));
			else
				ret->hitBone() = StringPool::Get(xorstr_("engine_col"));
		}
	}

	auto localPlayer = LocalPlayer::Entity( );
	if (localPlayer) {

		if (reinterpret_cast<BasePlayer*>(self->ignoreEntity( ))->userID( ) == localPlayer->userID( )) { // isAuthoritative
			if (plusminus::ui::get_bool(xorstr_("bullet tracers"))) {
				DDraw::Line(localPlayer->eyes( )->position( ), ret->hitPositionWorld( ), Color(1, 0, 0, 1), 1.5f, false, true);
			}

			if (entity) {
				if (entity->IsPlayer( )) {
					if (entity->isCached( )) {
						if (localPlayer->isCached( )) {
							// trajectory_end = 1 meter
							// player_distance = 0.2 meter
							// profit $$$$$$

							if (plusminus::ui::get_bool(xorstr_("big bullets"))) {
								auto bone = entity->model( )->find_bone(ret->hitPositionWorld( ));
								if (bone.second) {
									ret->hitPositionWorld( ) = bone.first->position( );
								}
							}
							if (plusminus::ui::get_combobox(xorstr_("hitbox override")) != 0) {
								if (plusminus::ui::get_combobox(xorstr_("hitbox override")) == 1)
									ret->hitBone( ) = StringPool::Get(xorstr_("spine4"));
								else if (plusminus::ui::get_combobox(xorstr_("hitbox override")) == 2)
									ret->hitBone( ) = StringPool::Get(xorstr_("head"));
								else if (plusminus::ui::get_combobox(xorstr_("hitbox override")) == 3) {

									// yandere dev in this bitch
									int num = rand( ) % 100;
									if (num > 90)
										ret->hitBone( ) = StringPool::Get(xorstr_("head"));
									else if (num < 90 && num > 80)
										ret->hitBone( ) = StringPool::Get(xorstr_("neck"));
									else if (num < 80 && num > 70)
										ret->hitBone( ) = StringPool::Get(xorstr_("l_clavicle"));
									else if (num < 70 && num > 60)
										ret->hitBone( ) = StringPool::Get(xorstr_("pelvis"));
									else if (num < 60 && num > 50)
										ret->hitBone( ) = StringPool::Get(xorstr_("r_hip"));
									else if (num < 50 && num > 40)
										ret->hitBone( ) = StringPool::Get(xorstr_("r_foot"));
									else if (num < 40 && num > 30)
										ret->hitBone( ) = StringPool::Get(xorstr_("spine1"));
									else if (num < 30 && num > 20)
										ret->hitBone( ) = StringPool::Get(xorstr_("l_hand"));
									else if (num < 20 && num > 10)
										ret->hitBone( ) = StringPool::Get(xorstr_("r_upperarm"));
									else if (num < 10)
										ret->hitBone( ) = StringPool::Get(xorstr_("l_knee"));
									else
										ret->hitBone( ) = StringPool::Get(xorstr_("spine4"));
								}
								else if (plusminus::ui::get_combobox(xorstr_("hitbox override")) == 4) {
									int yeet = rand( ) % 100;
									if (yeet > 50)
										ret->hitBone( ) = StringPool::Get(xorstr_("head"));
									else
										ret->hitBone( ) = StringPool::Get(xorstr_("spine4"));
								}
							}
						}
					}
				}
			}
		}
	}

	return ret;
}
void DoAttack_hk(FlintStrikeWeapon* weapon) {
	if (plusminus::ui::get_bool(xorstr_("insta eoka")))
		weapon->_didSparkThisFrame( ) = true;

	return weapon->DoAttack( );
}
Vector3 BodyLeanOffset_hk(PlayerEyes* a1) {
	if (plusminus::ui::get_bool(xorstr_("manipulator")) && get_key(plusminus::ui::get_keybind(xorstr_("manipulator bind")))) {
		if (target_ply != nullptr) {
			if (other::m_manipulate.empty( ) || !LocalPlayer::Entity( )->GetHeldEntity( ))
				return a1->BodyLeanOffset( );

			return other::m_manipulate;
		}
	}

	return a1->BodyLeanOffset( );
}
void DoFirstPersonCamera_hk(PlayerEyes* a1, Component* cam) {
	a1->DoFirstPersonCamera(cam);
	if (plusminus::ui::get_bool(xorstr_("manipulator"))) {
		Vector3 re_p = LocalPlayer::Entity( )->transform( )->position( ) + LocalPlayer::Entity( )->transform( )->up( ) * (PlayerEyes::EyeOffset( ).y + LocalPlayer::Entity( )->eyes( )->viewOffset( ).y);
		cam->transform( )->set_position(re_p);
	}
}
bool CanAttack_hk(BasePlayer* self) {
	if (plusminus::ui::get_bool(xorstr_("no attack restriction")))
		return true;

	return self->CanAttack( );
}
void UpdateVelocity_hk(PlayerWalkMovement* self) {
	if (!self->flying( )) {
		Vector3 vel = self->TargetMovement( );
		if (plusminus::ui::get_bool(xorstr_("no sprinting restriction"))) {
			float max_speed = (self->swimming( ) || self->Ducking( ) > 0.5) ? 1.7f : 5.5f;
			if (vel.length( ) > 0.f) {
				Vector3 target_vel = Vector3(vel.x / vel.length( ) * max_speed, vel.y, vel.z / vel.length( ) * max_speed);
				self->TargetMovement( ) = target_vel;
			}
		}
		if (plusminus::ui::get_bool(xorstr_("manipulator")) && get_key(plusminus::ui::get_keybind(xorstr_("manipulator bind")))) {
			float max_speed = (self->swimming( ) || self->Ducking( ) > 0.5) ? 1.7f : 5.5f;
			if (vel.length( ) > 0.f) {
				self->TargetMovement( ) = Vector3::Zero( );
			}
		}
	}

	return self->UpdateVelocity( );
}
Vector3 EyePositionForPlayer_hk(BaseMountable* mount, BasePlayer* player, Quaternion lookRot) {
	if (player->userID( ) == LocalPlayer::Entity( )->userID( )) {
		if (plusminus::ui::get_bool(xorstr_("manipulator")) && get_key(plusminus::ui::get_keybind(xorstr_("manipulator bind")))) {
			return mount->EyePositionForPlayer(player, lookRot) + other::m_manipulate;
		}
	}

	return mount->EyePositionForPlayer(player, lookRot);
}
void HandleJumping_hk(PlayerWalkMovement* a1, ModelState* state, bool wantsJump, bool jumpInDirection = false) {
	if (plusminus::ui::get_bool(xorstr_("no jumping restriction"))) {
		if (!wantsJump)
			return;

		a1->grounded( ) = (a1->climbing( ) = (a1->sliding( ) = false));
		state->set_ducked(false);
		a1->jumping( ) = true;
		state->set_jumped(true);
		a1->jumpTime( ) = Time::time( );
		a1->ladder( ) = nullptr;

		Vector3 curVel = a1->body( )->velocity( );
		a1->body( )->set_velocity({ curVel.x, 10, curVel.z });
		return;
	}

	return a1->HandleJumping(state, wantsJump, jumpInDirection);
}
void OnLand_hk(BasePlayer* ply, float vel) {
	if (!plusminus::ui::get_bool(xorstr_("no fall damage")))
		ply->OnLand(vel);
}
bool IsDown_hk(InputState* self, BUTTON btn) {
	if (plusminus::ui::get_bool(xorstr_("autoshoot")) || (plusminus::ui::get_bool(xorstr_("manipulator")) && get_key(plusminus::ui::get_keybind(xorstr_("manipulator bind"))))) {
		if (btn == BUTTON::FIRE_PRIMARY) {
			auto held = LocalPlayer::Entity( )->GetHeldEntity<BaseProjectile>( );
			if (held && !held->Empty( ) && held->class_name_hash( ) == STATIC_CRC32("BaseProjectile")) {
				if (target_ply != nullptr && target_ply->isCached( )) {
					auto mpv = target_ply->find_mpv_bone( );
					Vector3 target;
					if (mpv != nullptr)
						target = mpv->position;
					else
						target = target_ply->bones( )->head->position;

					if (LineOfSight(target, LocalPlayer::Entity( )->eyes( )->position( )))
						return true;
				}
			}
		}
	}

	return self->IsDown(btn);
}
void OnAttacked_hk(BaseCombatEntity* self, HitInfo* info) {
	self->OnAttacked(info);

	/*if (self->class_name_hash( ) == STATIC_CRC32("BasePlayer")) {
		if (info->Initiator( ) == LocalPlayer::Entity( )) {
			auto entity = reinterpret_cast<BasePlayer*>(self);

			if(std::filesystem::exists(settings::data_dir + xorstr_("\\hs.wav")))
				play_sound((settings::data_dir + xorstr_("\\hs.wav")).c_str());
		}
	}*/

}
void ClientInput_hk(BasePlayer* plly, uintptr_t state) {
	if (!plly)
		return plly->ClientInput(state);

	if (plly->userID( ) == LocalPlayer::Entity( )->userID( )) {
		if (plusminus::ui::get_bool(xorstr_("manipulator")) && target_ply != nullptr && target_ply->isCached( ) && get_key(plusminus::ui::get_keybind(xorstr_("manipulator bind"))))
			plly->clientTickInterval( ) = 0.95f;
		else
			plly->clientTickInterval( ) = 0.05f;

		auto held = plly->GetHeldEntity<BaseProjectile>( );
		if (held) {
			if (plusminus::ui::get_bool(xorstr_("no sway")) & held->class_name_hash() != STATIC_CRC32("BaseMelee")) {
				held->aimSway( ) = 0.f;
				held->aimSwaySpeed( ) = 0.f;
			}

			if (plusminus::ui::get_bool(xorstr_("automatic")))
				held->automatic( ) = true;

			if (plusminus::ui::get_bool(xorstr_("fake shots")) && get_key(plusminus::ui::get_keybind(xorstr_("fake shots bind"))))
				held->SendSignalBroadcast(BaseEntity::Signal::Attack, xorstr_(""));

			if (plusminus::ui::get_bool(xorstr_("autoshoot")) || (plusminus::ui::get_bool(xorstr_("manipulator")) && get_key(plusminus::ui::get_keybind(xorstr_("manipulator bind"))))) {
				if (!held->Empty( ) && held->class_name_hash( ) == STATIC_CRC32("BaseProjectile")) {
					if (target_ply != nullptr && target_ply->isCached( )) {
						auto mpv = target_ply->find_mpv_bone( );
						Vector3 target;
						if (mpv != nullptr)
							target = mpv->position;
						else
							target = target_ply->bones( )->head->position;

						if (LineOfSight(target, plly->eyes( )->position( )))
							held->DoAttack( );
					}
				}
			}
		}

		settings::tr::manipulated = plusminus::ui::get_bool(xorstr_("manipulator")) && get_key(plusminus::ui::get_keybind(xorstr_("manipulator bind")));

		/*if (settings::desync && get_key(settings::desync_key)) {
			float desyncTime = (Time::realtimeSinceStartup( ) - plly->lastSentTickTime( )) - 0.03125 * 3;
			float max_eye_value = (0.1f + ((desyncTime + 2.f / 60.f + 0.125f) * 1.5f) * plly->MaxVelocity( )) - 0.05f;

			plly->eyes( )->viewOffset( ) = Vector3(0, max_eye_value, 0);
		}*/

		GLOBAL_TIME = Time::time( );

		if (plusminus::ui::get_bool(xorstr_("manipulator")) && target_ply != nullptr && target_ply->isCached( ) && get_key(plusminus::ui::get_keybind(xorstr_("manipulator bind"))))
			other::find_manipulate_angle( );
		else
			if (!other::m_manipulate.empty( ))
				other::m_manipulate = Vector3::Zero( );

		Physics::IgnoreLayerCollision(4, 12, !plusminus::ui::get_bool(xorstr_("walk on water")));
		Physics::IgnoreLayerCollision(30, 12, plusminus::ui::get_bool(xorstr_("walk on water")));
		Physics::IgnoreLayerCollision(11, 12, plusminus::ui::get_bool(xorstr_("walk on water")));

		if (get_key(plusminus::ui::get_keybind(xorstr_("zoom bind"))))
			ConVar::Graphics::_fov( ) = 15.f;
		else
			ConVar::Graphics::_fov( ) = plusminus::ui::get_float(xorstr_("camera fov"));

		if (plusminus::ui::get_bool(xorstr_("fake admin")))
			plly->playerFlags( ) |= PlayerFlags::IsAdmin;

		if (plusminus::ui::get_bool(xorstr_("no attack restriction")))
			if (plly->mounted( ))
				plly->mounted( )->canWieldItems( ) = true;

		if (plusminus::ui::get_combobox(xorstr_("lightning")) != 0) {
			auto list = TOD_Sky::instances( );
			if (list) {
				for (int j = 0; j < list->size; j++) {
					auto sky = (TOD_Sky*)list->get(j);
					if (!sky)
						continue;

					float amb = 1.f;
					if (plusminus::ui::get_combobox(xorstr_("lightning")) == 1)
						amb = 4.f;
					else if (plusminus::ui::get_combobox(xorstr_("lightning")) == 2)
						amb = 6.f;

					sky->Day( )->AmbientMultiplier( ) = amb == 4.f ? 0.2f : 1.f;
					sky->Night( )->AmbientMultiplier( ) = amb;
				}
			}
		}
	}

	plly->ClientInput(state);

	// before network 

	if (plusminus::ui::get_bool(xorstr_("no sprinting restriction")))
		LocalPlayer::Entity( )->add_modelstate_flag(ModelState::Flags::Sprinting);
}

void DoMovement_hk(Projectile* pr, float deltaTime) {
	if (pr->isAuthoritative( ))
		if (plusminus::ui::get_bool(xorstr_("big bullets")))
			pr->thickness( ) = 1.f;
		else
			pr->thickness( ) = 0.1f;

	return pr->DoMovement(deltaTime);
}
float GetRandomVelocity_hk(ItemModProjectile* self) {
	float modifier = 1.f;

	if (plusminus::ui::get_bool(xorstr_("fast bullets")))
		modifier += 0.4f;

	return self->GetRandomVelocity( ) * modifier;
}
void ProcessAttack_hk(BaseMelee* self, HitTest* hit) {
	auto entity = hit->HitEntity( );

	//if (target_ply != nullptr) {
	//	auto l1 = target_ply->playerModel( )->_multiMesh( )->Renderers( );
	//	if (l1) {
	//		for (int i = 0; i < l1->size; i++) {
	//			auto rend = (Renderer_*)l1->get(i);
	//			if (!rend)
	//				continue;

	//			std::cout << *reinterpret_cast<uintptr_t*>(rend->material( ) + 0xB0) << std::endl;
	//			std::cout << reinterpret_cast<uintptr_t>(rend->material( )->shader( )) << std::endl << std::endl;

	//			if (plusminus::ui::get_float(xorstr_("targeting fov")) > 1000) {
	//				auto list = *reinterpret_cast<Array<Material*>**>(rend + 0x140);
	//				if (list) {
	//					for (int j = 0; j < list->size( ); j++) {
	//						auto g = list->get(j);
	//						if (!g)
	//							continue;

	//						*reinterpret_cast<Material**>(g) = nullptr;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	if (!plusminus::ui::get_bool(xorstr_("farm assist")) || !entity)
		return self->ProcessAttack(hit);

	if (entity->class_name_hash( ) == STATIC_CRC32("OreResourceEntity")) {
		BaseNetworkable* marker = BaseNetworkable::clientEntities( )->FindClosest(STATIC_CRC32("OreHotSpot"), entity, 5.0f);
		if (marker) {
			entity = marker;
			hit->HitTransform( ) = marker->transform( );
			hit->HitPoint( ) = marker->transform( )->InverseTransformPoint(marker->transform( )->position( ));
			hit->HitMaterial( ) = String::New(xorstr_("MetalOre"));
		}
	}
	else if (entity->class_name_hash( ) == STATIC_CRC32("TreeEntity")) {
		BaseNetworkable* marker = BaseNetworkable::clientEntities( )->FindClosest(STATIC_CRC32("TreeMarker"), entity, 5.0f);
		if (marker) {
			hit->HitTransform( ) = marker->transform( );
			hit->HitPoint( ) = marker->transform( )->InverseTransformPoint(marker->transform( )->position( ));
			hit->HitMaterial( ) = String::New(xorstr_("Wood"));
		}
	}

	return self->ProcessAttack(hit);
}
void AddPunch_hk(HeldEntity* attackEntity, Vector3 amount, float duration) {
	amount *= plusminus::ui::get_float(xorstr_("recoil %")) / 100.0f;

	attackEntity->AddPunch(amount, duration);
}

Vector3 MoveTowards_hk(Vector3 current, Vector3 target, float maxDelta) {
	static auto ptr = METHOD("Assembly-CSharp::BaseProjectile::SimulateAimcone(): Void");
	if (CALLED_BY(ptr, 0x800)) {
		target *= plusminus::ui::get_float(xorstr_("recoil %")) / 100.0f;
		maxDelta *= plusminus::ui::get_float(xorstr_("recoil %")) / 100.0f;
	}

	return Vector3_::MoveTowards(current, target, maxDelta);
}
bool DoHit_hk(Projectile* prj, HitTest* test, Vector3 point, Vector3 normal) {
	if (plusminus::ui::get_bool(xorstr_("pierce"))) {
		if (prj->isAuthoritative( )) {
			auto lol = test->HitEntity( );
			auto go = test->gameObject( );

			if (go && !lol) {
				if (go->layer( ) == 0 || go->layer( ) == 24) {
					return false;
				}
			}
			if (lol) {
				if (lol->class_name_hash( ) == STATIC_CRC32("CargoShip") || lol->class_name_hash( ) == STATIC_CRC32("BaseOven")
					|| lol->class_name_hash( ) == STATIC_CRC32("TreeEntity") || lol->class_name_hash( ) == STATIC_CRC32("OreResourceEntity")
					|| lol->class_name_hash( ) == STATIC_CRC32("CH47HelicopterAIController") || lol->class_name_hash( ) == STATIC_CRC32("MiniCopter")
					|| lol->class_name_hash( ) == STATIC_CRC32("BoxStorage") || lol->class_name_hash( ) == STATIC_CRC32("Workbench")
					|| lol->class_name_hash( ) == STATIC_CRC32("VendingMachine") || lol->class_name_hash( ) == STATIC_CRC32("Barricade")
					|| lol->class_name_hash( ) == STATIC_CRC32("BuildingPrivlidge") || lol->class_name_hash( ) == STATIC_CRC32("LootContainer")
					|| lol->class_name_hash( ) == STATIC_CRC32("HackableLockedCrate") || lol->class_name_hash( ) == STATIC_CRC32("ResourceEntity")
					|| lol->class_name_hash( ) == STATIC_CRC32("RidableHorse") || lol->class_name_hash( ) == STATIC_CRC32("MotorRowboat")
					|| lol->class_name_hash( ) == STATIC_CRC32("ScrapTransportHelicopter") || lol->class_name_hash( ) == STATIC_CRC32("JunkPile")
					|| lol->class_name_hash( ) == STATIC_CRC32("MiningQuarry") || lol->class_name_hash( ) == STATIC_CRC32("WaterCatcher")
					|| lol->class_name_hash( ) == STATIC_CRC32("RHIB")) {
					return false;
				}
			}
		}
	}
	return prj->DoHit(test, point, normal);
}
void SetEffectScale_hk(Projectile* self, float eScale) {
	return self->SetEffectScale((plusminus::ui::get_bool(xorstr_("psilent")) && self->isAuthoritative( )) ? 1.5f : eScale);
}
System::Object_* StartCoroutine_hk(MonoBehaviour* a1, System::Object_* un2) {
	if (plusminus::ui::get_bool(xorstr_("fast loot"))) {
		static auto v = METHOD("Assembly-CSharp::ItemIcon::SetTimedLootAction(UInt32,Action): Void");
		if (CALLED_BY(v, 0x656)) {
			*reinterpret_cast<float*>(un2 + 0x28) = -0.2f;
		}
	}

	return a1->StartCoroutine(un2);
}
void BobApply_hk(ViewmodelBob* self, uintptr_t vm, float fov) {
	if (!plusminus::ui::get_bool(xorstr_("no sprinting restriction")))
		self->Apply(vm, fov);
}
void SwayApply_hk(ViewmodelSway* self, uintptr_t vm) {
	if (!plusminus::ui::get_bool(xorstr_("no sprinting restriction")))
		self->Apply(vm);
}
void LowerApply_hk(ViewmodelLower* self, uintptr_t vm) {
	if (!plusminus::ui::get_bool(xorstr_("no sprinting restriction")))
		self->Apply(vm);
}
String* ConsoleRun_hk(ConsoleSystem::Option* optiom, String* str, Array<System::Object_*>* args) {
	if (optiom->IsFromServer( )) {
		if (str->buffer) {
			auto string = std::wstring(str->buffer);
			if (string.find(wxorstr_(L"noclip")) != std::wstring::npos ||
				string.find(wxorstr_(L"debugcamera")) != std::wstring::npos ||
				string.find(wxorstr_(L"admintime")) != std::wstring::npos ||
				string.find(wxorstr_(L"camlerp")) != std::wstring::npos ||
				string.find(wxorstr_(L"camspeed")) != std::wstring::npos) {

				str = String::New(xorstr_(""));
			}
		}
	}

	return ConsoleSystem::Run(optiom, str, args);
}
void set_flying_hk(ModelState* modelState, bool state) {
	modelState->set_flying(false);
}
void do_hooks( ) {
	VM_DOLPHIN_BLACK_START
	hookengine::hook(BasePlayer::ClientUpdate_, ClientUpdate_hk);
	hookengine::hook(BasePlayer::ClientUpdate_Sleeping_, ClientUpdate_Sleeping_hk);
	hookengine::hook(PlayerWalkMovement::UpdateVelocity_, UpdateVelocity_hk);
	hookengine::hook(PlayerWalkMovement::HandleJumping_, HandleJumping_hk);
	hookengine::hook(BasePlayer::CanAttack_, CanAttack_hk);
	hookengine::hook(BasePlayer::OnLand_, OnLand_hk);
	hookengine::hook(Projectile::DoMovement_, DoMovement_hk);
	hookengine::hook(FlintStrikeWeapon::DoAttack_, DoAttack_hk);
	hookengine::hook(ViewmodelBob::Apply_, BobApply_hk);
	hookengine::hook(ViewmodelSway::Apply_, SwayApply_hk);
	hookengine::hook(InputState::IsDown_, IsDown_hk);
	hookengine::hook(BaseCombatEntity::OnAttacked_, OnAttacked_hk);
	hookengine::hook(ConsoleSystem::Run_, ConsoleRun_hk);
	hookengine::hook(ViewmodelLower::Apply_, LowerApply_hk);
	hookengine::hook(ModelState::set_flying_, set_flying_hk);
	hookengine::hook(HitTest::BuildAttackMessage_, BuildAttackMessage_hk);
	hookengine::hook(BaseMelee::ProcessAttack_, ProcessAttack_hk);
	hookengine::hook(Projectile::DoHit_, DoHit_hk);
	hookengine::hook(BaseMountable::EyePositionForPlayer_, EyePositionForPlayer_hk);
	hookengine::hook(MonoBehaviour::StartCoroutine_, StartCoroutine_hk);
	hookengine::hook(Projectile::SetEffectScale_, SetEffectScale_hk);
	hookengine::hook(BasePlayer::ClientInput_, ClientInput_hk);
	hookengine::hook(ItemModProjectile::GetRandomVelocity_, GetRandomVelocity_hk);
	hookengine::hook(PlayerEyes::BodyLeanOffset_, BodyLeanOffset_hk);
	hookengine::hook(AimConeUtil::GetModifiedAimConeDirection_, GetModifiedAimConeDirection_hk);
	hookengine::hook(PlayerEyes::DoFirstPersonCamera_, DoFirstPersonCamera_hk);
	hookengine::hook(Vector3_::MoveTowards_, MoveTowards_hk);
	hookengine::hook(HeldEntity::AddPunch_, AddPunch_hk);
	VM_DOLPHIN_BLACK_END
}
void undo_hooks( ) {
	VM_DOLPHIN_BLACK_START
	hookengine::unhook(BasePlayer::ClientUpdate_, ClientUpdate_hk);
	hookengine::unhook(PlayerWalkMovement::UpdateVelocity_, UpdateVelocity_hk);
	hookengine::unhook(PlayerWalkMovement::HandleJumping_, HandleJumping_hk);
	hookengine::unhook(BasePlayer::CanAttack_, CanAttack_hk);
	hookengine::unhook(BasePlayer::OnLand_, OnLand_hk);
	hookengine::unhook(Projectile::DoMovement_, DoMovement_hk);
	hookengine::unhook(FlintStrikeWeapon::DoAttack_, DoAttack_hk);
	hookengine::unhook(ViewmodelBob::Apply_, BobApply_hk);
	hookengine::unhook(ViewmodelSway::Apply_, SwayApply_hk);
	hookengine::unhook(InputState::IsDown_, IsDown_hk);
	hookengine::unhook(BaseCombatEntity::OnAttacked_, OnAttacked_hk);
	hookengine::unhook(ConsoleSystem::Run_, ConsoleRun_hk);
	hookengine::unhook(ViewmodelLower::Apply_, LowerApply_hk);
	hookengine::unhook(ModelState::set_flying_, set_flying_hk);
	hookengine::unhook(HitTest::BuildAttackMessage_, BuildAttackMessage_hk);
	hookengine::unhook(BaseMelee::ProcessAttack_, ProcessAttack_hk);
	hookengine::unhook(Projectile::DoHit_, DoHit_hk);
	hookengine::unhook(BaseMountable::EyePositionForPlayer_, EyePositionForPlayer_hk);
	hookengine::unhook(MonoBehaviour::StartCoroutine_, StartCoroutine_hk);
	hookengine::unhook(Projectile::SetEffectScale_, SetEffectScale_hk);
	hookengine::unhook(BasePlayer::ClientInput_, ClientInput_hk);
	hookengine::unhook(ItemModProjectile::GetRandomVelocity_, GetRandomVelocity_hk);
	hookengine::unhook(PlayerEyes::BodyLeanOffset_, BodyLeanOffset_hk);
	hookengine::unhook(AimConeUtil::GetModifiedAimConeDirection_, GetModifiedAimConeDirection_hk);
	hookengine::unhook(PlayerEyes::DoFirstPersonCamera_, DoFirstPersonCamera_hk);
	hookengine::unhook(Vector3_::MoveTowards_, MoveTowards_hk);
	hookengine::unhook(HeldEntity::AddPunch_, AddPunch_hk);
	VM_DOLPHIN_BLACK_END
}