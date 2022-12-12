#include "../../../dependencies/utilities/csgo.hpp"
#include "../features.hpp"
#include "../misc/engine_prediction.hpp"







// Used in create_move hook. Inside prediction
void antiaim::basics::enable(c_usercmd* cmd, bool& send_packet) {
	if (!variables::enableantiaim) 
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive()) 
		return;

	const int move_type = csgo::local_player->move_type();
	weapon_t* active_weapon = csgo::local_player->active_weapon();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) 
		return;  
	
	if (!active_weapon) 
		return;

	if ((ragebot::can_fire(csgo::local_player) && cmd->buttons & in_attack)	

		|| (active_weapon->is_knife() && (cmd->buttons & in_attack || cmd->buttons & in_attack2)) 

		|| (active_weapon->is_bomb() && cmd->buttons & in_attack)                                   // Planting bomb
		|| cmd->buttons & in_use) return;															// Interacting with door, weapon, bomb, etc.

	float postpone_fire_ready = active_weapon->postpone_fire_ready_time();
	if (active_weapon->item_definition_index() == WEAPON_REVOLVER
		&& postpone_fire_ready > 0
		&& postpone_fire_ready < interfaces::globals->cur_time
		&& cmd->buttons & (in_attack | in_attack2))
		return;


	// @note: https://github.com/LWSS/Fuzion/blob/master/src/Hacks/grenadeprediction.cpp
	if (active_weapon->is_grenade() && !active_weapon->pin_pulled()) {
		float throw_time = active_weapon->throw_time();
		if (throw_time > 0) {
			send_packet = false;
			return;
		}
	}


	


	// Pitch
	cmd->viewangles.x = variables::pitch;

	// Use different var for yaw in case we are spinning or with peek aa
	static bool peek_right = true;
	constexpr float right_peek_yaw = 140.f;
	constexpr float left_peek_yaw = -140.f;
	static float yaw = 0.f;

	
	

	// Yaw
			// Add desync
	cmd->viewangles.y -= yaw;			// Real

	cmd->viewangles.clamp();

	/*
	 * Micromovement
	 *   Definition: the idea of micromovement is it literally "micromoves" so the server is constantly updating your LBY
	 *   to your real LBY and its not rotating towards your fake one.
	 */
	if (fabsf(cmd->sidemove) < 5.0f) {
		// If we are ducking make the movement bigger
		if (cmd->buttons & in_duck)
			cmd->sidemove = cmd->tick_count & 1 ? 3.25f : -3.25f;
		// Else just make it normal
		else
			cmd->sidemove = cmd->tick_count & 1 ? 1.1f : -1.1f;
	}

	// @todo: lby and fakelag
	// @note: https://github.com/LWSS/Fuzion/blob/0a4d775e17aba7a723aadce5b80898705e0bd6ff/src/Hacks/antiaim.cpp#L240
	// @note: https://github.com/LWSS/Fuzion/blob/master/src/Hacks/fakelag.cpp
}