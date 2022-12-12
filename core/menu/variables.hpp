#pragma once
#include "framework.hpp"

namespace variables {
	//RAGE
	inline bool enableantiaim = false;
	inline float pitch = 0.f;
	inline float yaws = 0.f;
	inline bool autorevolver = false;
	inline bool bhop = false;
	inline bool thirdperson = false;
	inline float thirdpersondistance = false;
	inline bool autostrafe = false;
	inline bool slowwalk = false;

	namespace ui {
		namespace spectators {
			inline bool spectator_list = true;
			inline int x = 10, y = 560;
			inline int w = 100, h = 10;
		}

		namespace menu {
			inline bool opened = false;
			inline int x = 300, y = 200;
			inline int w = 500, h = 450;
		}

		namespace watermark {
			inline int y = 10;
			inline int x = 15;
			inline int w = 0;
			inline int h = 17;
		}
	}

	namespace aim {
		inline bool triggerbot = false;
		inline hotkey_t triggerbot_key(VK_PRIOR);
		inline float triggerbot_delay = 0.f;

		inline bool aimbot = false;
		inline bool silent = false;
		inline bool autofire = false;
		inline hotkey_t aimbot_key(VK_NEXT);

		inline bool bodyaim_if_lethal = false;
		inline bool priorize_lethal_targets = false;		// If we can kill a someone inside our fov, go for it even if it's not the closest one


		
			
		inline std::vector<std::string> autowall_settings = {
		"Only visible",
		"Autowall",
		"Ignore walls"
		};
		inline combobox_toggle_t autowall(0);		// Will store the autowall setting: 0 only visible, 1 autowall, 2 ignore walls

		inline float aimbot_fov = 20.f;
		inline bool draw_fov = false;

		inline float aimbot_smoothing = 0.f;
		inline bool target_friends = false;
		inline bool non_rifle_aimpunch = true;
		inline bool aimbot_noscope = true;
		inline bool autorevolver = false;

		inline float min_damage = 60.f;

		inline std::vector<multicombo_opt_t> hitboxes_options = {
			{ "Head",	        true },
			{ "Neck",	        true },
			{ "Chest",			false },
			{ "Arms",			false },
			{ "Legs",			false },
		};
		inline multicombobox_toggle_t hitboxes(hitboxes_options);
	}


	inline float test_float = 0.f;

	namespace menu {
		inline bool opened = false;
		inline int x = 300, y = 200;
		inline int w = 500, h = 450;
	}
}