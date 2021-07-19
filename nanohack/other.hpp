namespace other {
	Vector3 m_manipulate = Vector3::Zero( );
	float m_last_manipulate_time = 0.f;

	void find_manipulate_angle( ) {
		auto loco = LocalPlayer::Entity( );
		Vector3 re_p = loco->transform( )->position( ) + loco->transform( )->up( ) * (PlayerEyes::EyeOffset( ).y + loco->eyes( )->viewOffset( ).y);
		// real eye pos

		Vector3 choice = Vector3::Zero( );

		if (LineOfSight(re_p, target_ply->find_mpv_bone( )->position) || !target_ply->isCached()) {
			m_manipulate = Vector3::Zero( );
			return;
		}

		auto right = loco->eyes( )->MovementRight( );
		auto forward = loco->eyes( )->MovementForward( );

		std::array<Vector3, 10> arr = {
			Vector3(right.x * 0.7f, 0.f, right.z * 0.7f), // small right
			Vector3(right.x * 1.4f, 0.f, right.z * 1.4f), // big right

			Vector3(-(right.x * 0.7f), 0.f, -(right.z * 0.7f)), // small left
			Vector3(-(right.x * 1.4f), 0.f, -(right.z * 1.4f)), // big left

			Vector3(0.f, 0.7f, 0.f), // small up
			Vector3(0.f, 1.4f, 0.f), // big up

			Vector3(forward.x * 0.7f, 0.f, forward.z * 0.7f), // small forward
			Vector3(forward.x * 1.4f, 0.f, forward.z * 1.4f), // big forward
		}; // restoring procedure: *= 1.428571428571429 for 0.7f
		   //                      *= 0.7142857142857143 for 1.4f

		for (auto s : arr) {
			Vector3 point = re_p + s;

			if (!LineOfSight(point, re_p))
				continue;

			if (!target_ply->bones( )->head->visible_(point))
				continue;

			choice = s;
			break;
		}
		if (choice.empty( )) {
			m_manipulate = Vector3::Zero( );
			return;
		}

		m_manipulate = choice;
		m_last_manipulate_time = Time::time( );
	}
	void test_bundle(AssetBundle* bundle) {
		if (!bundle) {
			std::cout << "bundle nfound\n";
			return;
		}

		auto arr = bundle->GetAllAssetNames( );
		if (!arr) {
			std::cout << "arr nfound\n";
			return;
		}

		for (int j = 0; j < arr->size( ); j++) {
			auto name = arr->get(j);

			printf("%ls\n", name->buffer);
		}

		std::cout << "bundletest - success\n";
	}
}
void dispatch_keybind(KeyCode& s) {

}