#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/variables.hpp"
#include "../../dependencies/interfaces/i_physics_surface_props.hpp"

#define TICK_INTERVAL			( interfaces::globals->interval_per_tick )
#define TIME_TO_TICKS( t )		( (int)( 0.5f + (float)( t ) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL * (float)( t ) )
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * time_to_ticks( t ) )
#define TICK_NEVER_THINK		( -1 )

struct autowall_data_t {
	bool lethal;
	float damage;		// Negative damage means invalid
};

struct localdata
{
	bool visualize_lag = false;

	c_baseplayeranimationstate* prediction_animstate = nullptr;
	c_baseplayeranimationstate* animstate = nullptr;

	vec3_t stored_real_angles = ZERO;
	vec3_t real_angles = ZERO;
	vec3_t fake_angles = ZERO;
};




namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);

		void slow_walk(c_usercmd* cmd);

	};

	




	
	void thirdperson();

	void reset_thirdperson();

}

namespace animations {
	// @todo: resolver
	namespace local {
		void run_local_animations();
	}
}

namespace antiaim {
	namespace basics {
		void enable(c_usercmd* cmd, bool& send_packet);
	};

}

namespace ragebot {
	
		void triggerbot(c_usercmd* cmd);
		void run_aimbot(c_usercmd* cmd);

		void auto_revolver(c_usercmd* cmd);
		bool can_fire(player_t* target);
		bool aimbot_weapon_check(bool check_scope);
		void draw_fov();

		namespace autowall {
			float get_damage_multiplier(int hit_group, float hs_multiplier);
			bool is_armored(int hit_group, bool helmet);
			bool trace_to_exit(trace_t& enter_trace, vec3_t& start, const vec3_t& direction, vec3_t& end, trace_t& exit_trace);
			static bool handle_bullet_penetration(surface_data* enter_surface_data, trace_t& enter_trace, const vec3_t& direction, vec3_t& start, float penetration, float& damage);
			autowall_data_t handle_walls(player_t* local_player, entity_t* entity, const vec3_t& destination, const weapon_info_t* weapon_data, bool enabled_hitbox);
		}
	
}

namespace globals {
	// Set to true when pressing the full update button. If true, does full_update in fsn and reverts to false. full_update() is called in fsn so its more thread safe.
	inline bool forcing_update = false;

	// Used in fireevent hook
	inline bool round_ended = false;



}

namespace watermark {
	inline bool user_dragging_menu = false;
	inline bool should_move_menu = false;

	// Will call other 2 functions
	void draw();

	// Will draw the user name, cheat name, fps and ping stats
	void get_str_and_draw();

	// Moving the watermark 
	void movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h);
};