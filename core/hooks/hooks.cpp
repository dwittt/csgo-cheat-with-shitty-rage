#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../menu/menu.hpp"


hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::override_view::fn override_view_original = nullptr;
hooks::frame_stage_notify::fn frame_stage_notify_original = nullptr;
hooks::should_skip_animation_frame::fn should_skip_animation_frame_original = nullptr;
hooks::do_procedural_foot_plant::fn do_procedural_foot_plant_original = nullptr;
hooks::build_transformations::fn build_transformations_original = nullptr;
hooks::update_client_side_animations::fn update_client_side_animations_original = nullptr;



bool should_animate()
{
	static int old_tick_count;

	if (old_tick_count != interfaces::globals->tick_count)
	{
		old_tick_count = interfaces::globals->tick_count;
		return true;
	}

	return false;
}




bool hooks::initialize() {
	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto override_view_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));
	const auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	const auto should_skip_animation_frame_target = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", sig_should_skip_animation_frame));
	const auto do_procedural_foot_plant_target = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", sig_do_procedural_foot_plant));
	const auto build_transformations_target = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", sig_build_transformations));
	const auto update_client_side_animations_target = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", sig_update_client_side_animations));
	

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create move. (outdated index?)");

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint traverse. (outdated index?)");

	if (MH_CreateHook(override_view_target, &override_view::hook, reinterpret_cast<void**>(&override_view_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");

	if (MH_CreateHook(frame_stage_notify_target	, &frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify_original)) != MH_OK)
		throw std::runtime_error("failed to initialize frame_stage_notify. (outdated index?)");

	if (MH_CreateHook(should_skip_animation_frame_target, &should_skip_animation_frame::hook, reinterpret_cast<void**>(&should_skip_animation_frame_original)) != MH_OK)
		throw std::runtime_error("failed to initialize should_skip_animation_frame. (outdated sig?)");

	if (MH_CreateHook(do_procedural_foot_plant_target, &do_procedural_foot_plant::hook, reinterpret_cast<void**>(&do_procedural_foot_plant_original)) != MH_OK)
		throw std::runtime_error("failed to initialize do_procedural_foot_plant. (outdated sig?)");




	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	console::log("[setup] hooks initialized!\n");
	return true;
}

void hooks::release() {
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
}




bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return false;

	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x34);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	misc::movement::bunny_hop(cmd);
	misc::movement::slow_walk(cmd);

	
		ragebot::run_aimbot(cmd);
		
		antiaim::basics::enable(cmd, send_packet);
		
	

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return false;
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		
		render::text(10, 10, render::fonts::watermark_font, "csgo-cheat", false, color::white(255));

		menu::toggle();
		menu::render();

		break;

	case fnv::hash("FocusOverlayPanel"):
		interfaces::panel->set_keyboard_input_enabled(panel, variables::menu::opened);
		interfaces::panel->set_mouse_input_enabled(panel, variables::menu::opened);
		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __fastcall hooks::override_view::hook(uintptr_t,  view_setup_t* setup) {
	if (csgo::local_player
		&& interfaces::engine->is_connected()
		&& interfaces::engine->is_in_game()
		&& !csgo::local_player->is_scoped()
		&& !(interfaces::engine->is_taking_screenshot()));
		
	
		misc::thirdperson();
	


	override_view_original(setup);
}

void __stdcall hooks::frame_stage_notify::hook(client_frame_stage_t frame_stage) {
	switch (frame_stage) {
	case FRAME_UNDEFINED:                       break;
	case FRAME_START:                           break;
	case FRAME_NET_UPDATE_START:                break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
				// Other models like localplayer, players and hands
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END:   break;
	case FRAME_NET_UPDATE_END:
		
		break;
	case FRAME_RENDER_START:
		// Force update on thread safe manner to avoid crashes
		if (globals::forcing_update) {
			interfaces::clientstate->full_update();
			globals::forcing_update = false;
		}

		animations::local::run_local_animations();
		break;
	case FRAME_RENDER_END:                      break;
	default:                                    break;
	}

	frame_stage_notify_original(interfaces::client, frame_stage);
}

bool __fastcall hooks::should_skip_animation_frame::hook(void* this_pointer, void* edx) {

	// the function is only called by SetupBones so there is no need to check for return address
	// returning false prevents copying of cached bone data

	return false;

}

void __fastcall hooks::do_procedural_foot_plant::hook(void* this_pointer, void* edx, void* bone_to_world, void* left_foot_chain, void* right_foot_chain, void* pos) {

	// the function is only called by DoExtraBoneProcessing so there is no need to check for return address
	// returning nothing prevents the "leg shuffling"

	return;

}



void __fastcall hooks::update_client_side_animations::hook(void* this_pointer, void* edx) {

	



	// only update local animations each tick
	if (should_animate)
		update_client_side_animations_original(this_pointer, edx);

}

