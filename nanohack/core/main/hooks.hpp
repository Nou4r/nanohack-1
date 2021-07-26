#include <intrin.h>
#define CALLED_BY(func,off) (reinterpret_cast<std::uint64_t>(_ReturnAddress()) > func && reinterpret_cast<std::uint64_t>(_ReturnAddress()) < func + off)

void ClientUpdate_hk(BasePlayer* player) {
	auto local = LocalPlayer::Entity( );
	if (local) {
		if (settings::players) {
			bonecache::cachePlayer(player);
		}
	}
	return player->ClientUpdate( );
}
Vector3 GetModifiedAimConeDirection_hk(float aimCone, Vector3 inputVec, bool anywhereInside = true) {
	if (settings::psilent && target_ply != nullptr && target_ply->isCached( )) {
		return (aimutils::get_prediction( ) - LocalPlayer::Entity( )->eyes( )->position( )).normalized( );
	}

	return AimConeUtil::GetModifiedAimConeDirection(aimCone, inputVec, anywhereInside);
}
Attack* BuildAttackMessage_hk(HitTest* self) {
	auto ret = self->BuildAttackMessage( );

	auto localPlayer = LocalPlayer::Entity( );
	if (localPlayer) {
		if (reinterpret_cast<BasePlayer*>(self->ignoreEntity( ))->userID( ) == localPlayer->userID( )) { // isAuthoritative
			if (settings::bullet_tracers) {
				DDraw::Line(localPlayer->eyes( )->position( ), ret->pointEnd( ), Color(1, 0, 0, 1), 1.5f, false, true);
				DDraw::Sphere(ret->pointEnd( ), 0.05f, Color(1, 0, 0, 1), 1.5f, false);
			}

			auto entity = BaseNetworkable::clientEntities( )->Find<BasePlayer*>(ret->hitID( ));
			if (entity) {
				if (entity->IsPlayer( )) {
					if (entity->isCached( )) {
						if (settings::h_override != 0) {
							if (localPlayer->isCached( )) {
								// trajectory_end = ~1 meter
								// player_distance = 0.2 meter
								// profit $$$$$$

								if (settings::bigger_bullets) {
									auto bone = entity->model( )->find_bone(ret->hitPositionWorld( ));
									if (bone.second) { // f
										if (settings::bullet_tracers)
											DDraw::Line(ret->hitPositionWorld( ), bone.first->position( ), Color(1, 0, 0, 1), 1.5f, false, true);

										ret->hitPositionWorld( ) = bone.first->position( );
									}
								}
								if (settings::h_override == 1)
									ret->hitBone( ) = StringPool::Get(xorstr_("spine4"));
								else if (settings::h_override == 2)
									ret->hitBone( ) = StringPool::Get(xorstr_("head"));
								else if (settings::h_override == 3) {
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
								else if (settings::h_override == 4) {
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
	if (settings::always_eoka)
		weapon->_didSparkThisFrame( ) = true;

	return weapon->DoAttack( );
}
Vector3 BodyLeanOffset_hk(PlayerEyes* a1) {
	if (settings::manipulator && !(settings::desync && get_key(settings::desync_key))) {
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
	if (settings::manipulator) {
		Vector3 re_p = LocalPlayer::Entity( )->transform( )->position( ) + LocalPlayer::Entity( )->transform( )->up( ) * (PlayerEyes::EyeOffset( ).y + LocalPlayer::Entity( )->eyes( )->viewOffset( ).y);
		cam->transform( )->set_position(re_p);
	}
}
bool CanAttack_hk(BasePlayer* self) {
	if (settings::freeaim)
		return true;

	return self->CanAttack( );
}
void UpdateVelocity_hk(PlayerWalkMovement* self) {
	if (!self->flying( )) {
		Vector3 vel = self->TargetMovement( );
		if (settings::omnisprint) {
			float max_speed = (self->swimming( ) || self->Ducking( ) > 0.5) ? 1.7f : 5.5f;
			if (vel.length( ) > 0.f) {
				Vector3 target_vel = Vector3(vel.x / vel.length( ) * max_speed, vel.y, vel.z / vel.length( ) * max_speed);
				self->TargetMovement( ) = target_vel;
			}
		}
		if (settings::desync && get_key(settings::desync_key)) {
			float max_speed = (self->swimming( ) || self->Ducking( ) > 0.5) ? 1.7f : 5.5f;
			if (vel.length( ) > 0.f) {
				self->TargetMovement( ) = Vector3::Zero( );
			}
		}
	}

	return self->UpdateVelocity( );
}
void HandleJumping_hk(PlayerWalkMovement* a1, ModelState* state, bool wantsJump, bool jumpInDirection = false) {
	if (settings::infinite_jump) {
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
	if (!settings::nofall)
		ply->OnLand(vel);
}
bool IsDown_hk(InputState* self, BUTTON btn) {
	if (btn == BUTTON::FIRE_PRIMARY) {
		if (settings::autoshoot) {
			auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>( );
			if (held && held->class_name_hash( ) == STATIC_CRC32("BaseProjectile")) {
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

	//if (settings::killsay != 0) {
	//	if (self->class_name_hash( ) == STATIC_CRC32("BasePlayer")) {
	//		if (info->Initiator( ) == LocalPlayer::Entity( )) {
	//			auto entity = reinterpret_cast<BasePlayer*>(self);

	//			static auto string_klass = CLASS("mscorlib::System::String");
	//			static auto arrayy = il2cpp_array_new<System::String>(string_klass, 0);

	//			std::string str;
	//			if (settings::killsay == 1) // advertise
	//				str = StringFormat::format(xorstr_("\"%s, you just got beamed by plusminus.\""), entity->_displayName( ));
	//			else if (settings::killsay == 2) // mock
	//				str = StringFormat::format(xorstr_("\"%s, you are a fucking nn dog.\""), entity->_displayName( ));

	//			arrayy->add(0, String::New(str.c_str( )));

	//			if (settings::killsay == 1)
	//				ConsoleSystem::Run(ConsoleSystem::Option::Client( ), String::New(xorstr_("chat.say")), reinterpret_cast<System::Array<System::Object_*>*>(arrayy));
	//			else if (settings::killsay == 2)
	//				ConsoleSystem::Run(ConsoleSystem::Option::Client( ), String::New(xorstr_("chat.say")), reinterpret_cast<System::Array<System::Object_*>*>(arrayy));
	//		}
	//	}
	//}
}
void ClientInput_hk(BasePlayer* plly, uintptr_t state) {
	if (!plly)
		return plly->ClientInput(state);

	if (plly->userID( ) == LocalPlayer::Entity( )->userID( )) {
		players::gamethread( );

		if (settings::manipulator && !(settings::desync && get_key(settings::desync_key)))
			plly->clientTickInterval( ) = 0.4f;
		else if (settings::desync && get_key(settings::desync_key))
			plly->clientTickInterval( ) = 0.95f;
		else
			plly->clientTickInterval( ) = 0.05f;

		settings::tr::desyncing = settings::desync && get_key(settings::desync_key);

		if (settings::desync && get_key(settings::desync_key)) {
			float desyncTime = (Time::realtimeSinceStartup( ) - plly->lastSentTickTime( )) - 0.03125 * 3;
			float max_eye_value = (0.1f + ((desyncTime + 2.f / 60.f + 0.125f) * 1.5f) * plly->GetMaxSpeed( )) - 0.05f;

			plly->eyes( )->viewOffset( ) = Vector3(0, max_eye_value, 0);
		}
		if (settings::autoshoot) {
			auto held = plly->GetHeldEntity<BaseProjectile>( );
			if (held && held->class_name_hash( ) == STATIC_CRC32("BaseProjectile")) {
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
		GLOBAL_TIME = Time::time( );
		viewMatrix = Camera::getViewMatrix( );

		if (settings::manipulator && target_ply != nullptr && target_ply->isCached( ) && !(settings::desync && get_key(settings::desync_key)))
			other::find_manipulate_angle( );
		else
			if (!other::m_manipulate.empty( ))
				other::m_manipulate = Vector3::Zero( );

		Physics::IgnoreLayerCollision(4, 12, !settings::walkonwater);
		Physics::IgnoreLayerCollision(30, 12, settings::walkonwater);
		Physics::IgnoreLayerCollision(11, 12, settings::walkonwater);

		ConVar::Graphics::_fov( ) = settings::camera_fov;

		if (settings::weapon_spam)
			if (plly->GetHeldEntity( ))
				plly->GetHeldEntity( )->SendSignalBroadcast(BaseEntity::Signal::Attack, xorstr_(""));

		if (settings::lightning != 0) {
			auto list = TOD_Sky::instances( );
			if (list) {
				for (int j = 0; j < list->size; j++) {
					auto sky = (TOD_Sky*)list->get(j);
					if (!sky)
						continue;

					float amb = 1.f;
					if (settings::lightning == 1)
						amb = 4.f;
					else if (settings::lightning == 2)
						amb = 6.f;

					sky->Day( )->AmbientMultiplier( ) = amb == 4.f ? 0.2f : 1.f;
					sky->Night( )->AmbientMultiplier( ) = amb;
				}
			}
		}
	}

	plly->ClientInput(state);

	// before network 

	if (settings::omnisprint)
		LocalPlayer::Entity( )->add_modelstate_flag(ModelState::Flags::Sprinting);
}

void DoMovement_hk(Projectile* pr, float deltaTime) {
	if (pr->isAuthoritative( ))
		if (settings::bigger_bullets)
			pr->thickness( ) = 1.f;
		else
			pr->thickness( ) = 0.1f;

	return pr->DoMovement(deltaTime);
}
float GetRandomVelocity_hk(ItemModProjectile* self) {
	float modifier = 1.f;

	if (settings::faster_bullets)
		modifier += 0.4f;

	return self->GetRandomVelocity( ) * modifier;
}
bool DoHit_hk(Projectile* prj, HitTest* test, Vector3 point, Vector3 normal) {
	if (settings::penetrate) {
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
	return self->SetEffectScale((settings::psilent && self->isAuthoritative( )) ? 1.5f : eScale);
}
System::Object_* StartCoroutine_hk(MonoBehaviour* a1, System::Object_* un2) {
	if (settings::fastloot) {
		static auto v = METHOD("Assembly-CSharp::ItemIcon::SetTimedLootAction(UInt32,Action): Void");
		if (CALLED_BY(v, 0x656)) {
			*reinterpret_cast<float*>(un2 + 0x28) = -0.2f;
		}
	}

	return a1->StartCoroutine(un2);
}
void BobApply_hk(ViewmodelBob* self, uintptr_t vm, float fov) {
	if (!settings::omnisprint)
		self->Apply(vm, fov);
}
void SwayApply_hk(ViewmodelSway* self, uintptr_t vm) {
	if (!settings::omnisprint)
		self->Apply(vm);
}
void LowerApply_hk(ViewmodelLower* self, uintptr_t vm) {
	if (!settings::omnisprint)
		self->Apply(vm);
}
String* ConsoleRun_hk(ConsoleSystem::Option* optiom, String* str, Array<System::Object_*>* args) {

	if (optiom->IsFromServer( )) {
		if (str->buffer) {
			auto string = std::wstring(str->buffer);
			if (string.find(wxorstr_(L"noclip")) != std::wstring::npos || string.find(wxorstr_(L"debugcamera")) != std::wstring::npos || string.find(wxorstr_(L"admintime")) != std::wstring::npos) {
				str = String::New(xorstr_(""));
			}
		}
	}

	return ConsoleSystem::Run(optiom, str, args);
}
void do_hooks( ) {
	hookengine::hook(BasePlayer::ClientUpdate_, ClientUpdate_hk);
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
	hookengine::hook(HitTest::BuildAttackMessage_, BuildAttackMessage_hk);
	hookengine::hook(Projectile::DoHit_, DoHit_hk);
	hookengine::hook(MonoBehaviour::StartCoroutine_, StartCoroutine_hk);
	hookengine::hook(Projectile::SetEffectScale_, SetEffectScale_hk);
	hookengine::hook(BasePlayer::ClientInput_, ClientInput_hk);
	hookengine::hook(ItemModProjectile::GetRandomVelocity_, GetRandomVelocity_hk);
	hookengine::hook(PlayerEyes::BodyLeanOffset_, BodyLeanOffset_hk);
	hookengine::hook(AimConeUtil::GetModifiedAimConeDirection_, GetModifiedAimConeDirection_hk);
	hookengine::hook(PlayerEyes::DoFirstPersonCamera_, DoFirstPersonCamera_hk);
}