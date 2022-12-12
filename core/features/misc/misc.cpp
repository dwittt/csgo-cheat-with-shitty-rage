#include "../features.hpp"

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!variables::bhop)
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (!(csgo::local_player->flags() & fl_onground))
		cmd->buttons &= ~in_jump;
}

void multidir_rage_strafe(c_usercmd* cmd) {
	if (!variables::autostrafe) return;
	if (ragebot::can_fire(csgo::local_player) != 0 && (cmd->buttons & in_attack)) return;

	const float speed = csgo::local_player->velocity().length_2d();

	// If pressing forward or back, give a bit of starting speed, then just ignore
	if (cmd->buttons & in_forward && speed <= 30.0f) {
		cmd->forwardmove = 450.0f;
		return;
	}

	// Get best delta
	// https://git.debooger.xyz/debooger/ah3/src/branch/master/hax/misc.c#L185
	float best_delta = 0.f;
	float term = interfaces::console->get_convar("sv_air_max_wishspeed")->get_float() / interfaces::console->get_convar("sv_airaccelerate")->get_float() / interfaces::console->get_convar("sv_maxspeed")->get_float() * 100.0f / speed;
	if (term < 1 && term > -1) best_delta = acosf(term);
	else                       return;

	vec3_t viewangles;
	interfaces::engine->get_view_angles(viewangles);

	// Get our desired angles and delta
	float yaw = DEG2RAD(viewangles.y);
	float vel_dir = atan2f(csgo::local_player->velocity().y, csgo::local_player->velocity().x) - yaw;		// Get our player velocity
	float target_ang = atan2f(-cmd->sidemove, cmd->forwardmove);											// Get the desired angle
	float delta = math::angle_delta_rad(vel_dir, target_ang);												// Get delta

	// Apply delta to vel_dir (Strafe left or right)
	float movedir = delta < 0 ? vel_dir + best_delta : vel_dir - best_delta;

	cmd->forwardmove = cosf(movedir) * interfaces::console->get_convar("cl_forwardspeed")->get_float();
	cmd->sidemove = -sinf(movedir) * interfaces::console->get_convar("cl_sidespeed")->get_float();

	viewangles.normalize();
	viewangles.clamp();
	math::correct_movement(viewangles, cmd, cmd->forwardmove, cmd->sidemove);
}

void misc::movement::slow_walk(c_usercmd* cmd) {
	if (!variables::slowwalk) return;
	if (!interfaces::inputsystem->is_held(GetAsyncKeyState(VK_SHIFT))) return;
	if (!csgo::local_player) return;
	if (!(csgo::local_player->flags() & fl_onground)) return;        // We are not walking

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon) return;
	const auto weapon_info = weapon->get_weapon_data();
	if (!weapon_info) return;

	auto max_speed = 0.33f * (csgo::local_player->is_scoped() ? weapon_info->weapon_max_speed_alt : weapon_info->weapon_max_speed);     // Max slowwalk speed
	if (max_speed <= 0.f) return;

	float min_speed = (float)(std::sqrt(cmd->forwardmove * cmd->forwardmove + cmd->sidemove * cmd->sidemove + cmd->upmove * cmd->upmove));
	if (min_speed <= 0.f) return;

	if (cmd->buttons & in_duck)
		max_speed *= 2.94117647f;

	if (min_speed <= max_speed) return;

	float final_speed = max_speed / min_speed;

	cmd->forwardmove *= final_speed;
	cmd->sidemove *= final_speed;
	cmd->upmove *= final_speed;
}


