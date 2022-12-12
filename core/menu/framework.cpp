#include "framework.hpp"
#include "variables.hpp"

//credits to harcuz for menu framework (https://www.unknowncheats.me/forum/members/2669363.html),
POINT cursor;
POINT cursor_corrected;

cursor_coords cursor1;
cursor_coords cursor_corrected1;


#define DRAW_GROUPBOX_LABELS false

#pragma region menu_framework EXTERNS
int menu_framework::vars::column_number = 0;			// Current column. Will change when calling add_column()

// o_* variables are the original ones with 0 columns (all menu width)
int menu_framework::vars::o_container_left_pos = variables::ui::menu::x + container_margin;		// Will change when adding more columns
int menu_framework::vars::o_container_width = variables::ui::menu::w - container_margin * 2;	// Will get divided when adding more columns
int menu_framework::vars::o_item_left_pos = o_container_left_pos + container_padding;			// Base top left pos for all items (label text position)

int menu_framework::vars::o_item_combo_pos = variables::ui::menu::x + o_container_width - container_margin;		// Max right pos
int menu_framework::vars::o_item_checkbox_pos = o_item_combo_pos - item_checkbox_length;
int menu_framework::vars::o_item_slider_pos = o_item_combo_pos - item_slider_length;				// Top left corner of the actual slider
int menu_framework::vars::o_item_hotkey_w = o_container_width - container_padding * 2;

// Actual vars for items and containers. Updated in init_tab() and add_column()
int menu_framework::vars::container_left_pos = o_container_left_pos;
int menu_framework::vars::container_width = o_container_width;
int menu_framework::vars::item_left_pos = o_item_left_pos;
int menu_framework::vars::item_combo_pos = o_item_combo_pos;
int menu_framework::vars::item_checkbox_pos = o_item_checkbox_pos;
int menu_framework::vars::item_slider_pos = o_item_slider_pos;
int menu_framework::vars::item_hotkey_w = o_item_hotkey_w;

// Vars for groupbox
int menu_framework::vars::o_cur_part_y = variables::ui::menu::y + vars::top_margin_with_tabs + vars::container_margin;
int menu_framework::vars::o_cur_base_item_y = o_cur_part_y + container_padding;		// Base y position of the items (position of the first item of the groupbox)

int menu_framework::vars::cur_part_items = 0;			// Will be changed when adding groupbox
int menu_framework::vars::cur_part_y = o_cur_part_y;
int menu_framework::vars::cur_base_item_y = o_cur_base_item_y;
int menu_framework::vars::cur_part_h = 0;			// Will update with each item added

int menu_framework::vars::button_part_item = 0;
int menu_framework::vars::button_part_h = 0;			// Need to get h first to subtract it from bottom to get top pos
int menu_framework::vars::button_part_y = variables::ui::menu::y + variables::ui::menu::h - container_margin;
int menu_framework::vars::button_base_item_y = button_part_y + container_padding;
#pragma endregion

void menu_framework::group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label) {
	//groupbox background
	render::draw_filled_rect(x, y, w, h, color(25, 25, 25, 255));

	//groupbox outline
	render::draw_rect(x, y, w, h, color(45, 45, 45, 255));

	//groupbox label
	if (show_label)
		render::text(x + 2, y - 12, font, string, false, color::white());
}

void menu_framework::update_positions() {
	vars::o_container_left_pos = variables::ui::menu::x + vars::container_margin;			// Will change when adding more columns
	vars::o_container_width = variables::ui::menu::w - vars::container_margin * 2;		// Will get divided when adding more columns
	vars::o_item_left_pos = vars::o_container_left_pos + vars::container_padding;		// Base top left pos for all items (label text position)

	vars::o_item_combo_pos = variables::ui::menu::x + vars::o_container_width - vars::container_margin;		// Max right pos
	vars::o_item_checkbox_pos = vars::o_item_combo_pos - vars::item_checkbox_length;
	vars::o_item_slider_pos = vars::o_item_combo_pos - vars::item_slider_length;				// Top left corner of the actual slider
	vars::o_item_hotkey_w = vars::o_container_width - vars::container_padding * 2;

	// Goupbox vars
	vars::cur_part_items = 0;		// Will get updated on the add_group_box() calls anyway
	vars::cur_part_h = 0;
	vars::o_cur_part_y = variables::ui::menu::y + vars::top_margin_with_tabs;		// No container margin because we will add it in add_groupbox()
	vars::o_cur_base_item_y = vars::o_cur_part_y + vars::container_padding;

	vars::button_part_item = 0;
	vars::button_part_h = 0;			// Need to get h first to subtract it from bottom to get top pos
	vars::button_part_y = variables::ui::menu::y + variables::ui::menu::h - vars::container_margin;
	vars::button_base_item_y = vars::button_part_y + vars::container_padding;
}

void menu_framework::init_tab() {
	vars::column_number = 0;

	vars::container_left_pos = vars::o_container_left_pos;
	vars::container_width = (vars::o_container_width / vars::columns) - (vars::container_margin / vars::columns);

	// We need to assign them again instead of using the originals because the container widht changed
	vars::item_left_pos = vars::container_left_pos + vars::container_padding;
	vars::item_combo_pos = variables::ui::menu::x + vars::container_width - vars::container_margin;
	vars::item_checkbox_pos = vars::item_combo_pos - vars::item_checkbox_length;
	vars::item_slider_pos = vars::item_combo_pos - vars::item_slider_length;
	vars::item_hotkey_w = vars::container_width - vars::container_padding * 2;

	vars::cur_part_y = vars::o_cur_part_y;
	vars::cur_base_item_y = vars::o_cur_base_item_y;
}


// Same as the other hotkey but using struct


void menu_framework::add_column() {
	vars::column_number++;

	if (vars::column_number > 0) {		// Only if we are not on the first col
		vars::container_width;
		vars::cur_part_h = 0;

		vars::container_left_pos = vars::container_left_pos + (vars::container_width * vars::column_number) + vars::container_margin;
		vars::item_left_pos = vars::item_left_pos + (vars::container_width * vars::column_number) + vars::container_margin;
		vars::item_checkbox_pos = vars::item_checkbox_pos + (vars::container_width * vars::column_number) + vars::container_margin;
		vars::item_slider_pos = vars::item_slider_pos + (vars::container_width * vars::column_number) + vars::container_margin;
		vars::item_combo_pos = vars::item_checkbox_pos + vars::item_checkbox_length;

		vars::cur_part_y = vars::o_cur_part_y;				// We reset the y positions of the goupbox so they start on top of the second column
		vars::cur_base_item_y = vars::o_cur_base_item_y;
	}
}

void menu_framework::add_groupbox1(int item_number) {
	vars::cur_part_items = item_number;
	vars::cur_part_y += vars::cur_part_h + vars::container_margin;		// cur_part_h is the "previous" part h
	vars::cur_base_item_y = vars::cur_part_y + vars::container_padding;
	vars::cur_part_h = (15 * vars::cur_part_items) + (vars::container_padding * 2) - 4;		// This for now, but should be increased with the items added
}

void menu_framework::add_groupbox1(std::string name, int item_number) {
	vars::cur_part_items = item_number;
	vars::cur_part_y += vars::cur_part_h + vars::container_margin;		// cur_part_h is the "previous" part h
	vars::cur_base_item_y = vars::cur_part_y + vars::container_padding;
	vars::cur_part_h = (15 * vars::cur_part_items) + (vars::container_padding * 2) - 4;		// This for now, but should be increased with the items added

	menu_framework::group_box(menu_framework::vars::container_left_pos, menu_framework::vars::cur_part_y, menu_framework::vars::container_width, menu_framework::vars::cur_part_h, render::fonts::watermark_font, name, DRAW_GROUPBOX_LABELS);
}

void menu_framework::add_bottom_groupbox1(int item_number) {
	vars::button_part_item = item_number;
	vars::button_part_h = (vars::button_part_item * 15) + (vars::container_padding * 2) - 4;
	vars::button_part_y = variables::ui::menu::y + variables::ui::menu::h - vars::container_margin - vars::button_part_h;		// Get the top left corner based on the margin pos and the height (start from bottom)
	vars::button_base_item_y = vars::button_part_y + vars::container_padding;			// Same as other containers
}

void menu_framework::tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, std::int32_t& tab, std::int32_t count, bool show_outline) {
	GetCursorPos(&cursor);

	if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h) && (GetAsyncKeyState(VK_LBUTTON) & 1))
		tab = count;
	
	//tab background
	if (show_outline)
		render::draw_rect(x, y, w, h, tab == count ? color(52, 134, 235, 255) : color(25, 25, 25, 255));

	//tab label
	render::text(x - render::get_text_size(font, string).x / 2 + 50, y + h / 2 - 8, font, string, false, show_outline ? color::white() : tab == count ? color(52, 134, 235, 255) : color::white());
}

void menu_framework::check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value) {
	GetCursorPos(&cursor);

	int w = 10, h = 10;

	if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_LBUTTON) & 1)
		value = !value;

	//checkbox background
	render::draw_filled_rect(position, y, w, h, value ? color(52, 134, 235, 255) : color(36, 36, 36, 255));

	//checkbox label
	render::text(x + 2, y - 1, font, string, false, color::white());
}

void menu_framework::slider(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, float& value, float min_value, float max_value) {
	GetCursorPos(&cursor);
	
	int ix = x + 140;
	int yi = y + 4;
	
	if ((cursor.x > ix) && (cursor.x < ix + position) && (cursor.y > yi) && (cursor.y < yi + 6) && (GetAsyncKeyState(VK_LBUTTON)))
		value = (cursor.x - ix) / (float(position) / float(max_value));

	//slider background
	render::draw_filled_rect(ix, yi, position, 6, color(36, 36, 36, 255));
	render::draw_filled_rect(ix, yi, value * (float(position) / float(max_value)), 6, color(52, 134, 235, 255));

	//slider label
	render::text(x + 2, y - 1, font, (std::stringstream{ } << string << ": " <<  std::setprecision(3) << value).str(), false, color::white());
}

void menu_framework::multicombobox(std::int32_t x, std::int32_t y, std::int32_t combo_right_pos, unsigned long font, const std::string label, multicombobox_toggle_t& target) {
	interfaces::surface->surface_get_cursor_pos(cursor1.x, cursor1.y);

	const int x_margin = popup_system::combo_win_padding;
	const int h = 11;
	const int arrow_w = 7, arrow_h = 4;
	const int arrow_x = combo_right_pos - x_margin - arrow_w, arrow_y = y + 4;	// h/2 is not reliable
	const int max_button_len = 100;
	std::string button_str = "";
	bool has_enabled = false;	// Will be false if all options are disabled
	for (multicombo_opt_t item : target.vector) {
		std::string temp_str = button_str;
		if (item.state) {
			if (has_enabled) temp_str += ", ";		// The check makes it not add a comma before the first item
			temp_str += item.text;
			if (render::get_text_size(render::fonts::watermark_font, temp_str).x < max_button_len) {
				if (has_enabled) button_str += ", ";
				button_str += item.text;
			}
			else {
				button_str += ", ...";
				break;
			}
			has_enabled = true;		// We found an enabled item
		}
	}
	if (!has_enabled) button_str = "None";
	const int item_w = render::get_text_size(render::fonts::watermark_font, button_str).x;	// Need to get text first (selected options)
	int w = x_margin + item_w + x_margin + arrow_w + x_margin;
	if (target.toggle) {		// Stores the px width of the biggest text in the vector if popup is active
		for (multicombo_opt_t item : target.vector) {
			int text_w = render::get_text_size(render::fonts::watermark_font, item.text).x + x_margin * 2;
			if (text_w > w)
				w = text_w;
		}
	}
	const int position = combo_right_pos - w;		// Get top left corner of current item

	// The bad thing about mouse_in_popup is that you can only check for popups after they are generated (You pop the items when rendering from the vector)
	if (!popup_system::mouse_in_popup(cursor.x, cursor.y) && interfaces::inputsystem->is_pressed(VK_LBUTTON)) {
		if ((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h))
			target.toggle = !target.toggle;			// If in checkbox and clicked
		// See color picker comment
		else if (!((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h + target.vector.size() * 15)))
			target.toggle = false;					// Close popup if user clicks outside
	}

	// Combobox "button"
	render::draw_filled_rect(position, y - 1, w, h + 2, color(36, 36, 36, 255));
	render::text(position + x_margin, y - 1, render::fonts::watermark_font, button_str, false, color::white());
	render::draw_rect(position, y - 1, w, h + 2, color(45, 45, 45, 255));

	// Draw arrow
	for (int n = 0; n < arrow_h; n++) {
		render::draw_filled_rect(arrow_x + n, arrow_y + n, arrow_w - n * 2, 1, color::white());
	}

	// Combobox label
	render::text(x + 2, y - 1, font, label, false, color::white());

	// Push to vector to render after menu
	if (target.toggle)
		popup_system::active_multicombo_popups.push_back(multicombo_popup_info{ position, y + h + 2, w, target.vector.size() * 15, target.vector, target.toggle });
}

void menu_framework::add_multicombobox(std::string label, multicombobox_toggle_t& target, unsigned long font) {
	menu_framework::multicombobox(menu_framework::vars::item_left_pos, menu_framework::vars::cur_base_item_y, menu_framework::vars::item_combo_pos, font, label, target);
	menu_framework::vars::cur_base_item_y += 15;
}

void menu_framework::add_multicombobox(std::string label, multicombobox_toggle_t& target) {
	add_multicombobox(label, target, render::fonts::watermark_font);
}

void menu_framework::menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
	GetCursorPos(&cursor);
	
	if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
		should_drag = true;

		if (!should_move) {
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move = true;
		}
	}
	
	if (should_drag) {
		x = cursor.x - cursor_corrected.x;
		y = cursor.y - cursor_corrected.y;
	}
	
	if (GetAsyncKeyState(VK_LBUTTON) == 0) {
		should_drag = false;
		should_move = false;
	}
}

void popup_system::render_popups() {
	
	check_combo_popups();
	check_multicombo_popups();
}

// Checks if the mouse is in an active popup
bool popup_system::mouse_in_popup(int x, int y) {
	// For each color popup in the active_color_popups vector


	for (const combo_popup_info& pinfo : active_combo_popups) {
		if (pinfo.popup_toggle && (x >= pinfo.x) && (x <= pinfo.x + pinfo.w) && (y >= pinfo.y) && (y <= pinfo.y + pinfo.h))
			return true;
	}

	for (const multicombo_popup_info& pinfo : active_multicombo_popups) {
		if (pinfo.popup_toggle && (x >= pinfo.x) && (x <= pinfo.x + pinfo.w) && (y >= pinfo.y) && (y <= pinfo.y + pinfo.h))
			return true;
	}

	return false;
}


void popup_system::check_combo_popups() {
	// Render each active popup and pop from vector until there are no popups left
	while (!active_combo_popups.empty()) {
		combobox_popup(active_combo_popups.back());			// Render
		active_combo_popups.pop_back();						// Remove
	}
}

// Will check for popups in the active_combo_popups vector
void popup_system::check_multicombo_popups() {
	// Render each active popup and pop from vector until there are no popups left
	while (!active_multicombo_popups.empty()) {
		multicombobox_popup(active_multicombo_popups.back());	// Render
		active_multicombo_popups.pop_back();				// Remove
	}
}

void popup_system::combobox_popup(combo_popup_info combo_p) {
	if (!combo_p.popup_toggle) return;

	interfaces::surface->surface_get_cursor_pos(cursor1.x, cursor1.y);

	// We get the largest item width when rendering the button, and we pass it to the combo_p, so we just use that
	render::draw_filled_rect(combo_p.x, combo_p.y, combo_p.w, combo_p.h, color(30, 30, 30, 255));
	render::draw_filled_rect(combo_p.x, combo_p.y + combo_p.target_idx * 15 - 1, combo_p.w, 15, color(20, 20, 20, 255));

	int item_n = 0;
	for (std::string item : combo_p.opt_vec) {
		render::text(combo_p.x + combo_win_padding, combo_p.y + (15 * item_n), render::fonts::watermark_font, item, false, color::white());
		item_n++;
	}

	if ((cursor.x >= combo_p.x) && (cursor.x <= combo_p.x + combo_p.w) && (cursor.y >= combo_p.y) && (cursor.y < combo_p.y + combo_p.h) && interfaces::inputsystem->is_held(VK_LBUTTON))
		combo_p.target_idx = (cursor.y - combo_p.y) / 15;		// Get clicked item
}

void popup_system::multicombobox_popup(multicombo_popup_info combo_p) {
	if (!combo_p.popup_toggle) return;
	
	interfaces::surface->surface_get_cursor_pos(cursor1.x, cursor1.y);

	// We get the largest item width when rendering the button, and we pass it to the combo_p, so we just use that
	render::draw_filled_rect(combo_p.x, combo_p.y, combo_p.w, combo_p.h, color(30, 30, 30, 255));
	int cur_option = 0;
	for (multicombo_opt_t item : combo_p.target_vec) {
		if (item.state)
			render::draw_filled_rect(combo_p.x, combo_p.y + cur_option * 15 - 1, combo_p.w, 15, color(20, 20, 20, 255));
		cur_option++;
	}

	int item_n = 0;
	for (multicombo_opt_t item : combo_p.target_vec) {
		render::text(combo_p.x + combo_win_padding, combo_p.y + (15 * item_n), render::fonts::watermark_font, item.text, false, item.state ? color(107, 117, 255, 255) : color(255, 255, 255, 255));
		item_n++;
	}

	if ((cursor.x >= combo_p.x) && (cursor.x <= combo_p.x + combo_p.w) && (cursor.y >= combo_p.y) && (cursor.y < combo_p.y + combo_p.h) && interfaces::inputsystem->is_pressed(VK_LBUTTON)) {
		int clicked_idx = (cursor.y - combo_p.y) / 15;		// Get clicked item
		combo_p.target_vec.at(clicked_idx).state = !combo_p.target_vec.at(clicked_idx).state;
	}
}

void menu_framework::combobox(std::int32_t x, std::int32_t y, std::int32_t combo_right_pos, unsigned long font, const std::string label, std::vector<std::string>& opt_vec, combobox_toggle_t& target) {
	interfaces::surface->surface_get_cursor_pos(cursor1.x, cursor1.y);

	const int x_margin = popup_system::combo_win_padding;
	const int h = 11;
	const int arrow_w = 7, arrow_h = 4;
	const int arrow_x = combo_right_pos - x_margin - arrow_w, arrow_y = y + 4;	// h/2 is not reliable
	const int item_w = render::get_text_size(render::fonts::watermark_font, opt_vec.at(target.idx)).x;
	int w = x_margin + item_w + x_margin + arrow_w + x_margin;
	// Stores the px width of the biggest text in the vector if popup is active
	if (target.toggle) {
		for (std::string item : opt_vec) {
			int text_w = render::get_text_size(render::fonts::watermark_font, item).x + x_margin * 2;
			if (text_w > w)
				w = text_w;
		}
	}
	const int position = combo_right_pos - w;		// Get top left corner of current item

	// The bad thing about mouse_in_popup is that you can only check for popups after they are generated (You pop the items when rendering from the vector)
	if (!popup_system::mouse_in_popup(cursor.x, cursor.y) && interfaces::inputsystem->is_pressed(VK_LBUTTON)) {
		if ((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h))
			target.toggle = !target.toggle;			// If in checkbox and clicked
		// See color picker comment
		else if (!((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h + opt_vec.size() * 15)))
			target.toggle = false;			// Close popup if user clicks outside
	}

	// Combobox "button"
	render::draw_filled_rect(position, y - 1, w, h + 2, color(36, 36, 36, 255));
	render::text(position + x_margin, y - 1, render::fonts::watermark_font, opt_vec.at(target.idx), false, color::white());
	render::draw_rect(position, y - 1, w, h + 2, color(45, 45, 45, 255));		// Border

	// Draw arrow
	for (int n = 0; n < arrow_h; n++) {
		render::draw_filled_rect(arrow_x + n, arrow_y + n, arrow_w - n * 2, 1, color::white());
	}

	// Combobox label
	render::text(x + 2, y - 1, font, label, false, color::white());

	// Push to vector to render after menu
	if (target.toggle)
		popup_system::active_combo_popups.push_back(combo_popup_info{ position, y + h + 1, w, opt_vec.size() * 15, opt_vec, target.idx, target.toggle });
}


void menu_framework::check_box1(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, int click_area_id) {
	interfaces::surface->surface_get_cursor_pos(cursor1.x, cursor1.y);

	const int w = 11, h = 11;

	// The bad thing about mouse_in_popup is that you can only check for popups after they are generated (You pop the items when rendering from the vector)
	if (!popup_system::mouse_in_popup(cursor.x, cursor.y)) {
		switch (click_area_id) {
		default:
		case 0: {	// Only checkbox
			if ((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h) && interfaces::inputsystem->is_pressed(VK_LBUTTON))
				value = !value;		// If in checkbox and clicked
			break;
		}
		case 1: {	// Name and checkbox, not color
			if (((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h)		// Checkbox
				|| (cursor.x >= x) && (cursor.x <= position - 55) && (cursor.y >= y) && (cursor.y <= y + h))		// Name and all that. (5 + 20 + 5 + 20 + 5 for the colors)
				&& interfaces::inputsystem->is_pressed(VK_LBUTTON))
				value = !value;		// If in checkbox or text, but not color picker and clicked
			break;
		}
		case 2: {	// All width from name to checkbox
			if ((cursor.x >= x) && (cursor.x <= position + w) && (cursor.y >= y - 1) && (cursor.y <= y + h + 1) && interfaces::inputsystem->is_pressed(VK_LBUTTON)) {
				value = !value;		// If in checkbox or text and clicked
			}
			break;
		}
		}
	}

	// Checkbox itself
	render::draw_filled_rect(position, y, w, h, color(36, 36, 36, 255));		// Background
	render::draw_rect(position, y, w, h, color(45, 45, 45, 255));				// Border
	if (value) render::draw_filled_rect(position + 2, y + 2, w - 4, h - 4, color(107, 117, 255, 255));		// Color if enabled

	// Checkbox label
	render::text(x + 2, y - 1, font, string, false, color::white());
}

// Checkbox with color picker and custom region
void menu_framework::check_box1(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, colorpicker_col_t& col) {
	interfaces::surface->surface_get_cursor_pos(cursor1.x, cursor1.y);
	const int w = 11, h = 11;							// For checkbox
	const int margin = 5;								// Color "button" margin
	const int col_w = 20, col_h = 11;					// Color "button" size
	const int color_x = position - margin - col_w;		// Color "button" position

	if (!popup_system::mouse_in_popup(cursor.x, cursor.y) && interfaces::inputsystem->is_pressed(VK_LBUTTON)) {		// Check click and all that once so it doesn't freak out
		if (((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h))	// Checkbox
			|| ((cursor.x >= x) && (cursor.x <= position - 55) && (cursor.y >= y) && (cursor.y <= y + h)))		// Name and all that. (5 + 20 + 5 + 20 + 5 for the 2 colors)
			value = !value;

		// Not else if because we want to check if the cursor is in the toggle color button (open popup) or outside (close popup)
		if ((cursor.x >= color_x) && (cursor.x <= color_x + col_w) && (cursor.y >= y) && (cursor.y <= y + col_h))
			col.toggle = !col.toggle;	// Toggle the "active popup" bool
		// We need to check like this instead of using mouse_in_popup because the first check will be on a popup that is not yet in the active_color_popups vector (see bottom of this func)
		else if (!((cursor.x >= color_x) && (cursor.x <= color_x + popup_system::win_w) && (cursor.y >= y + h + margin) && (cursor.y <= y + h + margin + popup_system::win_h)))
			col.toggle = false;			// Close popup if user clicks outside
	}

	render::draw_filled_rect(position, y, w, h, color(36, 36, 36, 255));
	render::draw_rect(position, y, w, h, color(45, 45, 45, 255));
	if (value) render::draw_filled_rect(position + 2, y + 2, w - 4, h - 4, color(107, 117, 255, 255));
	render::text(x + 2, y - 1, font, string, false, color::white());	// Checkbox text

	render::draw_filled_rect(color_x, y, col_w, col_h, col.col);					// Color itself
	render::draw_rect(color_x, y, col_w, col_h, color(45, 45, 45, 255));			// Color outline

	// Push to vector to render after menu
	if (col.toggle)
		popup_system::active_color_popups.push_back(color_popup_info{ color_x, y + col_h + margin, col });
}

void menu_framework::check_box1(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, colorpicker_col_t& col1, colorpicker_col_t& col2) {
	interfaces::surface->surface_get_cursor_pos(cursor1.x, cursor1.y);
	const int w = 11, h = 11;							// For checkbox
	const int margin = 5;								// Color "button" margin
	const int col_w = 20, col_h = 11;					// Color "button" size
	const int color_r_x = position - margin - col_w;	// Right color (2) "button" position
	const int color_l_x = color_r_x - margin - col_w;	// Left color (1) "button" position

	// Comments in other function
	if (!popup_system::mouse_in_popup(cursor.x, cursor.y) && interfaces::inputsystem->is_pressed(VK_LBUTTON)) {		// Check click and all that once so it doesn't freak out
		// Checkbox
		if (((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h))
			|| ((cursor.x >= x) && (cursor.x <= position - 55) && (cursor.y >= y) && (cursor.y <= y + h)))		// Name and all that. (5 + 20 + 5 + 20 + 5 for the 2 colors)
			value = !value;

		// Clicked outside popups. We need to check like this so both popups can't be open at the same time
		if (!((cursor.x >= color_l_x) && (cursor.x <= color_r_x + popup_system::win_w) && (cursor.y >= y + h + margin) && (cursor.y <= y + h + margin + popup_system::win_h))) {
			// Check if we clicked left button, then toggle. If not just turn it off
			if ((cursor.x >= color_l_x) && (cursor.x <= color_l_x + col_w) && (cursor.y >= y) && (cursor.y <= y + col_h))
				col1.toggle = !col1.toggle;
			else
				col1.toggle = false;

			// Check if we clicked right button, then toggle. If not just turn it off
			if ((cursor.x >= color_r_x) && (cursor.x <= color_r_x + col_w) && (cursor.y >= y) && (cursor.y <= y + col_h))
				col2.toggle = !col2.toggle;
			else
				col2.toggle = false;
		}
	}

	// Checkbox. Comments in first checkbox
	render::draw_filled_rect(position, y, w, h, color(36, 36, 36, 255));
	render::draw_rect(position, y, w, h, color(45, 45, 45, 255));
	if (value) render::draw_filled_rect(position + 2, y + 2, w - 4, h - 4, color(107, 117, 255, 255));

	render::text(x + 2, y - 1, font, string, false, color::white());

	// Left color
	render::draw_filled_rect(color_l_x, y, col_w, col_h, col1.col);
	render::draw_rect(color_l_x, y, col_w, col_h, color(45, 45, 45, 255));

	// Right color
	render::draw_filled_rect(color_r_x, y, col_w, col_h, col2.col);
	render::draw_rect(color_r_x, y, col_w, col_h, color(45, 45, 45, 255));

	// Push to vector to render after menu
	if (col1.toggle)
		popup_system::active_color_popups.push_back(color_popup_info{ color_l_x, y + col_h + margin, col1 });
	if (col2.toggle)
		popup_system::active_color_popups.push_back(color_popup_info{ color_r_x, y + col_h + margin, col2 });
}





void menu_framework::add_combobox(std::string label, std::vector<std::string>& option_vector, combobox_toggle_t& target, unsigned long font) {
	menu_framework::combobox(menu_framework::vars::item_left_pos, menu_framework::vars::cur_base_item_y, menu_framework::vars::item_combo_pos, font, label, option_vector, target);
	menu_framework::vars::cur_base_item_y += 15;

}

void menu_framework::add_combobox(std::string label, std::vector<std::string>& option_vector, combobox_toggle_t& target) {
	add_combobox(label, option_vector, target, render::fonts::watermark_font);
}

void menu_framework::add_checkbox(std::string label, bool& target, unsigned long font) {
	// @todo: Should update cur_part_h with += item_h
	menu_framework::check_box(menu_framework::vars::item_left_pos, menu_framework::vars::cur_base_item_y, menu_framework::vars::item_checkbox_pos, font, label, target);
	menu_framework::vars::cur_base_item_y += 15;		// Add 15 to the variable
}

void menu_framework::add_checkbox(std::string label, bool& target) {
	add_checkbox(label, target, render::fonts::watermark_font);
}

void menu_framework::add_checkbox(std::string label, bool& target, colorpicker_col_t& color, unsigned long font) {
	menu_framework::check_box1(menu_framework::vars::item_left_pos, menu_framework::vars::cur_base_item_y, menu_framework::vars::item_checkbox_pos, font, label, target, color);
	menu_framework::vars::cur_base_item_y += 15;
}

void menu_framework::add_checkbox(std::string label, bool& target, colorpicker_col_t& color) {
	add_checkbox(label, target, color, render::fonts::watermark_font);
}

void menu_framework::add_checkbox(std::string label, bool& target, colorpicker_col_t& color1, colorpicker_col_t& color2, unsigned long font) {
	menu_framework::check_box1(menu_framework::vars::item_left_pos, menu_framework::vars::cur_base_item_y, menu_framework::vars::item_checkbox_pos, font, label, target, color1, color2);
	menu_framework::vars::cur_base_item_y += 15;
}

void menu_framework::add_checkbox(std::string label, bool& target, colorpicker_col_t& color1, colorpicker_col_t& color2) {
	add_checkbox(label, target, color1, color2, render::fonts::watermark_font);
}