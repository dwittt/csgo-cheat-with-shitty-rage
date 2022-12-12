#pragma once

namespace hooks {
	bool initialize();
	void release();

	
	


	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __stdcall hook(float input_sample_frametime, c_usercmd* cmd);
			
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace override_view {
		using fn = void(__thiscall*)(view_setup_t* setup);
		void __fastcall hook(uintptr_t,  view_setup_t* setup);
	}



	namespace frame_stage_notify {
		using fn = void(__thiscall*)(void*, client_frame_stage_t);
		void __stdcall hook(client_frame_stage_t frame_stage);
	}

	namespace should_skip_animation_frame {
		using fn = void* (__thiscall*)(void* this_pointer, void* edx);
		bool __fastcall hook(void* this_pointer, void* edx);
	}

	namespace do_procedural_foot_plant {
		using fn = void* (__thiscall*)(void* this_pointer, void* edx, void* bone_to_world, void* left_foot_chain, void* right_foot_chain, void* pos);
		void __fastcall hook(void* this_pointer, void* edx, void* bone_to_world, void* left_foot_chain, void* right_foot_chain, void* pos);
	}

	namespace build_transformations {
		using fn = void* (__thiscall*)( void* this_pointer, void* edx, void* hdr, void* pos, void* q, const void* camera_transform, int bone_mask, void* bone_computed );
		void __fastcall hook( void* this_pointer, void* edx, void* hdr, void* pos, void* q, const void* camera_transform, int bone_mask, void* bone_computed );
	}

	namespace update_client_side_animations {
		using fn = void* (__thiscall*)(void* this_pointer, void* edx);
		void __fastcall hook(void* this_pointer, void* edx);
	}
}
