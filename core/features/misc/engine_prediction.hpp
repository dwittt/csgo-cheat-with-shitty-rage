#pragma once
#include "../../../dependencies/utilities/csgo.hpp"

namespace prediction {
	void start(c_usercmd* cmd);
	void post_think();
	void end();
	float get_server_time(c_usercmd* cmd);



	inline player_move_data data{};
	inline bool custom_inpred{};
	inline float old_cur_time{};
	inline float old_frame_time{};
	inline int* prediction_random_seed{};
	inline int* prediction_player{};
};
