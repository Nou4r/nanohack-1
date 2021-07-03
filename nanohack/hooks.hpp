#include <intrin.h>
#define CALLED_BY(func,off) (reinterpret_cast<std::uint64_t>(_ReturnAddress()) > func && reinterpret_cast<std::uint64_t>(_ReturnAddress()) < func + off)

Shader* shader;
int propertyF;

void ClientUpdate_hk(BasePlayer* player) {
	auto local = LocalPlayer::Entity( );
	if (local) {
		if (settings::players) {
			bonecache::cachePlayer(player);
		}
		if (player->userID( ) == local->userID( )) {
			GLOBAL_TIME = Time::time( );
			viewMatrix = Camera::getViewMatrix( );

			if (settings::manipulator && target_ply != nullptr)
				other::find_manipulate_angle( );
			else
				other::m_manipulate = Vector3::Zero( );

			Physics::IgnoreLayerCollision(4, 12, !settings::walkonwater);
			Physics::IgnoreLayerCollision(30, 12, settings::walkonwater);
			Physics::IgnoreLayerCollision(11, 12, settings::walkonwater);

			/*auto held = local->GetHeldItem( );
			if (held) {
				std::cout << "held" << std::endl;
				auto wearable = held->info( )->itemModWearable( )->targetWearable( );
				if (wearable) {
					std::cout << "wearable" << std::endl;
					auto nigga = wearable->renderers( );
					if (nigga) {
						std::cout << "renderers: " << (int)nigga->size << std::endl;
						for (int i = 0; i < nigga->size; i++) {
							auto renderer = reinterpret_cast<Renderer_*>(nigga->get(i));
							if (!renderer)
								continue;

							if (renderer->material( )->shader( ) != nullptr)
								renderer->material( )->set_shader(nullptr);
						}
					}
					auto nigga2 = wearable->skinnedRenderers( );
					if (nigga2) {
						std::cout << "skinnedrenderers: " << (int)nigga2->size << std::endl;
						for (int i = 0; i < nigga2->size; i++) {
							auto renderer = reinterpret_cast<Renderer_*>(nigga2->get(i));
							if (!renderer)
								continue;

							if (renderer->material( )->shader( ) != nullptr)
								renderer->material( )->set_shader(nullptr);
						}
					}
				}
			}*/
			/*static bool once = false;
			if (target_ply != nullptr && !once) {

				auto arr = target_ply->model( )->boneTransforms( );
				auto arr2 = target_ply->model( )->boneNames( );
				if (arr) {
					for (int i = 0; i < arr->size( ); i++) {
						auto transform = reinterpret_cast<Transform*>(arr->get(i));
						if (!transform)
							return;

						printf("%ls || idx: %d\n", arr2->get(i)->buffer, i);
					}
				}

				once = true;
			}*/
			players::gamethread_loop( );

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
	}
	return player->ClientUpdate( );
}
Vector3 GetModifiedAimConeDirection_hk(float aimCone, Vector3 inputVec, bool anywhereInside = true) {
	if (settings::psilent && target_ply != nullptr && target_ply->isCached( )) {
		return (aimutils::get_prediction( ) - LocalPlayer::Entity( )->eyes( )->position_e( )).normalized( );
	}

	return AimConeUtil::GetModifiedAimConeDirection(aimCone, inputVec, anywhereInside);
}
Attack* BuildAttackMessage_hk(HitTest* self) {
    auto ret = self->BuildAttackMessage( );

	if (settings::bullet_tracers)
		if (reinterpret_cast<BasePlayer*>(self->ignoreEntity( ))->userID( ) == LocalPlayer::Entity( )->userID( ))
			DDraw::Line(LocalPlayer::Entity( )->eyes( )->position_e( ), ret->pointEnd( ), Color(1, 0, 0, 1), 1.5f, false, true);

	auto entity = BaseNetworkable::clientEntities( )->Find<BasePlayer*>(ret->hitID( ));

	if (settings::h_override == 0 || !entity || !entity->IsPlayer( ))
		return ret;

	if (!LocalPlayer::Entity( )->isCached( ) || !entity->isCached( ))
		return ret;

	if (settings::h_override == 1) {
		ret->hitBone( ) = StringPool::Get(xorstr_("spine4"));
		ret->hitPositionWorld( ) = entity->bones( )->spine4->position;
	}
	else if (settings::h_override == 2) {
		ret->hitBone( ) = StringPool::Get(xorstr_("head"));
		ret->hitPositionWorld( ) = entity->bones( )->head->position;
	}

	return ret;
}
void DoAttack_hk(FlintStrikeWeapon* weapon) {
	if (settings::always_eoka)
		weapon->_didSparkThisFrame( ) = true;

	return weapon->DoAttack( );
}
Vector3 BodyLeanOffset_hk(PlayerEyes* a1) {
	if (settings::manipulator) {
		if (target_ply != nullptr) {
			if (other::m_manipulate.empty( ))
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
	if (settings::omnisprint) {
		Vector3 vel = self->TargetMovement( );

		float max_speed = (self->swimming() || self->Ducking( ) > 0.5) ? 1.7f : 5.6f;
		float target_speed = std::max(max_speed, vel.length( ));
		if (vel.length( ) > 0.f) {
			Vector3 target_vel = Vector3(vel.x / vel.length( ) * target_speed, vel.y, vel.z / vel.length( ) * target_speed);
			self->TargetMovement( ) = target_vel;
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
		a1->ladder( ) = NULL;

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
void ClientInput_hk(BasePlayer* plly, uintptr_t state) {
	plly->ClientInput(state);

	if (settings::omnisprint)
		LocalPlayer::Entity( )->add_modelstate_flag(ModelState::Flags::Sprinting);
}
void DoMovement_hk(Projectile* pr, float deltaTime) {
	if (pr->isAuthoritative( ))
		if (settings::bigger_bullets)
			pr->thickness( ) = 1.0f;
		else
			pr->thickness( ) = 0.1f;

	return pr->DoMovement(deltaTime);
}
void ProjectileUpdate_hk(Projectile* pr) {
	if (target_ply != nullptr) {
		if (!target_ply->is_visible())
			if (GetAsyncKeyState(0x43))
				return;
	}

	pr->Update( );
}
float GetRandomVelocity_hk(ItemModProjectile* self) {
	float modifier = 1.f;

	if (settings::faster_bullets)
		modifier += 0.4f;

	return self->GetRandomVelocity( ) * modifier;
}
bool DoHit_hk(Projectile* prj, HitTest* test, Vector3 point, Vector3 normal) {
	if (prj->isAuthoritative( )) {
		auto lol = test->HitEntity( );
		auto go = test->gameObject( );

		if (settings::penetrate) {
			if (go) {
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
	return self->SetEffectScale(settings::psilent ? settings::test1 : eScale);
}
System::Object* StartCoroutine_hk(MonoBehaviour* a1, System::Object* un2) {
	if (settings::fastloot) {
		static auto v = METHOD("Assembly-CSharp::ItemIcon::SetTimedLootAction(UInt32,Action): Void");
		if (CALLED_BY(v, 0x656))
			*reinterpret_cast<float*>(un2 + 0x28) = -0.2f;
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
void do_hooks( ) {
	hookengine::hook(BasePlayer::ClientUpdate_, ClientUpdate_hk);
	hookengine::hook(PlayerWalkMovement::UpdateVelocity_, UpdateVelocity_hk);
	hookengine::hook(PlayerWalkMovement::HandleJumping_, HandleJumping_hk);
	hookengine::hook(BasePlayer::CanAttack_, CanAttack_hk);
	hookengine::hook(BasePlayer::OnLand_, OnLand_hk);
	hookengine::hook(Projectile::DoMovement_, DoMovement_hk);
	hookengine::hook(Projectile::Update_, ProjectileUpdate_hk); 
	hookengine::hook(FlintStrikeWeapon::DoAttack_, DoAttack_hk);
	hookengine::hook(ViewmodelBob::Apply_, BobApply_hk);
	hookengine::hook(ViewmodelSway::Apply_, SwayApply_hk);
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
