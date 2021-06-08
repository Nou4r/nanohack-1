#include <intrin.h>
#define CALLED_BY(func,off) (reinterpret_cast<std::uint64_t>(_ReturnAddress()) > func && reinterpret_cast<std::uint64_t>(_ReturnAddress()) < func + off)

void ClientUpdate_hk(BasePlayer* player) {
	auto local = LocalPlayer::Entity( );
	if (local) {
		if (settings::players) {
			bonecache::cachePlayer(player);
		}
		if (player->userID( ) == local->userID( )) {
			viewMatrix = Camera::getViewMatrix( );

			if (settings::manipulator && target_ply != nullptr)
				other::find_manipulate_angle( );
			else
				other::m_manipulate = Vector3::Zero( );

			if (settings::lightning != 0) {
				auto list = TOD_Sky::instances( );
				if (list) {
					for (int j = 0; j < list->size; j++) {
						auto sky = reinterpret_cast<TOD_Sky*>(list->get(j));
						if (!sky)
							continue;

						float amb = 1.f;
						if (settings::lightning == 1)
							amb = 3.f;
						else if (settings::lightning == 2)
							amb = 6.f;

						sky->Day( )->AmbientMultiplier( ) = amb == 3.f ? 0.5f : 1.f;
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
		inputVec = (aimutils::get_prediction( ) - LocalPlayer::Entity( )->eyes( )->position_e( )).normalized( );
		aimCone = 0.f;
	}

	return AimConeUtil::GetModifiedAimConeDirection(aimCone, inputVec, anywhereInside);
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
		float speed = max(5.6, vel.length( ));
		if (vel.length( ) > 0.f) {
			Vector3 target_vel = Vector3(vel.x / vel.length( ) * speed, vel.y, vel.z / vel.length( ) * speed);
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
			pr->thickness( ) = 0.5f;

	return pr->DoMovement(deltaTime);
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
uintptr_t StartCoroutine_hk(MonoBehaviour* a1, uintptr_t un2) {
	if (settings::fastloot) {
		static auto ptr = METHOD("Assembly-CSharp::ItemIcon::SetTimedLootAction(UInt32,Action): Void");
		if (CALLED_BY(ptr, 0x656))
			*reinterpret_cast<float*>(un2 + 0x28) = -0.2f;
	}

	return a1->StartCoroutine(un2);
}
void do_hooks( ) {
	hookengine::hook(BasePlayer::ClientUpdate_, ClientUpdate_hk);
	hookengine::hook(PlayerWalkMovement::UpdateVelocity_, UpdateVelocity_hk);
	hookengine::hook(PlayerWalkMovement::HandleJumping_, HandleJumping_hk);
	hookengine::hook(BasePlayer::CanAttack_, CanAttack_hk);
	hookengine::hook(BasePlayer::OnLand_, OnLand_hk);
	hookengine::hook(Projectile::DoMovement_, DoMovement_hk);
	hookengine::hook(Projectile::DoHit_, DoHit_hk);
	hookengine::hook(MonoBehaviour::StartCoroutine_, StartCoroutine_hk);
	hookengine::hook(BasePlayer::ClientInput_, ClientInput_hk);
	hookengine::hook(ItemModProjectile::GetRandomVelocity_, GetRandomVelocity_hk);
	hookengine::hook(PlayerEyes::BodyLeanOffset_, BodyLeanOffset_hk);
	hookengine::hook(AimConeUtil::GetModifiedAimConeDirection_, GetModifiedAimConeDirection_hk);
	hookengine::hook(PlayerEyes::DoFirstPersonCamera_, DoFirstPersonCamera_hk);
}