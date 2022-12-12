#include "menu.hpp"

//todo auto elements positioning

auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color bg, color header_text, color header_line, const std::string& name) {
		render::draw_filled_rect(x, y, w, h, bg);
		render::draw_filled_rect(x, y, w, 30, header_text);
		render::draw_filled_rect(x, y + 30, w, 2, header_line);
		render::text(x + 10, y + 8, render::fonts::watermark_font, name, false, color::white());
};

void menu::render() {
	if (!variables::menu::opened)
		return;

	do_frame(variables::menu::x, variables::menu::y, variables::menu::w, variables::menu::h, color(36, 36, 36, 255), color(25, 25, 25, 255), color(36, 36, 36, 255), "b l i t z . w i n");
	
	menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, 100, 260, render::fonts::watermark_font, "tabs", false); {
		menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2) - 35, 100, 30, render::fonts::watermark_font, "rage", menu::current_tab, 0, false);
		menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2), 100, 30, render::fonts::watermark_font, "anti-aim", menu::current_tab, 1, false);
		menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2) + 35, 100, 30, render::fonts::watermark_font, "visuals", menu::current_tab, 2, false);
	}


	menu_framework::update_positions();
	switch (current_tab) {
	case 0:
		menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 285, 260, render::fonts::watermark_font, "rage", false); {
			menu_framework::init_tab();









			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font, "enable rage-bot", variables::aim::aimbot);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 60, variables::menu::x + 375, render::fonts::watermark_font, "autofire", variables::aim::autofire);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 75, variables::menu::x + 375, render::fonts::watermark_font, "silent", variables::aim::silent);
			menu_framework::combobox(variables::menu::x + 120, variables::menu::y + 90, variables::menu::x + 375, render::fonts::watermark_font, "Autowall", variables::aim::autowall_settings, variables::aim::autowall);
			menu_framework::slider(variables::menu::x + 120, variables::menu::y + 105, 125, render::fonts::watermark_font, "minimum damage", variables::aim::min_damage, 0.f, 100.f);
			menu_framework::slider(variables::menu::x + 120, variables::menu::y + 120, 125, render::fonts::watermark_font, "minimum damage", variables::aim::aimbot_fov, 0.f, 180.f);

			/* -------- Aim - Second column -------- */



		}
		break;
	case 1:
		menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 285, 260, render::fonts::watermark_font, "anti-aim", false); {
			menu_framework::init_tab();

			
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font, "enable anti-aim", variables::enableantiaim);
				menu_framework::slider(variables::menu::x + 120, variables::menu::y + 60, 125, render::fonts::watermark_font, "pitch", variables::pitch, 89.f, -89.f);
				menu_framework::slider(variables::menu::x + 120, variables::menu::y + 75, 125, render::fonts::watermark_font, "yaw", variables::yaws, 0.f, 180.f);
				



			
		}
		break;
	case 2:
		menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 285, 260, render::fonts::watermark_font, "visuals", false); {
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font, "bunny-hop", variables::bhop);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 60, variables::menu::x + 375, render::fonts::watermark_font, "thirdperson", variables::thirdperson);
			menu_framework::slider(variables::menu::x + 120, variables::menu::y + 75, 125, render::fonts::watermark_font, "thirdperson-distance", variables::thirdpersondistance, 100.f, 300.f);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 90, variables::menu::x + 375, render::fonts::watermark_font, "slowwalk", variables::slowwalk);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 105, variables::menu::x + 375, render::fonts::watermark_font, "autostrafe", variables::autostrafe);
			


		}
		break;
	}

	menu_framework::menu_movement(variables::menu::x, variables::menu::y, variables::menu::w, 30);
}

void menu::toggle() {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		variables::menu::opened = !variables::menu::opened;
}
