#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../../source-sdk/helpers.hpp"
#include "../../dependencies/utilities/renderer/renderer.hpp"

#include <vector>

#pragma region STRUCTS
struct cursor_coords {
	int x;
	int y;
};

struct multicombo_opt_t {
	std::string text;
	bool state;
};

class colorpicker_col_t {
public:
	color col;
	float_hsv f_hsv;		// Used for color pickers
	bool toggle;

	colorpicker_col_t(colorpicker_col_t& col_picker) {
		this->col = col_picker.col;
		this->f_hsv = col_picker.f_hsv;
		this->toggle = col_picker.toggle;
	}

	colorpicker_col_t(const color col, const bool toggle = false) {
		this->col = col;
		this->f_hsv = helpers::colors::color2hsv_float(col);
		this->toggle = toggle;
	}

	operator color() { return col; }
	operator bool() { return toggle; }
};

class combobox_toggle_t {
public:
	int idx;		// The selected idx of the vector
	bool toggle;	// For toggling combobox popup

	combobox_toggle_t(const int idx, const bool toggle = false) {
		this->idx = idx;
		this->toggle = toggle;
	}

	operator int() { return idx; }
	operator bool() { return toggle; }
};

class multicombobox_toggle_t {
public:
	std::vector<multicombo_opt_t> vector;		// The selected idx of the vector. We don't need an address here cuz we will pass the whole &multicombobox_toggle_t
	bool toggle;	// For toggling combobox popup

	multicombobox_toggle_t(std::vector<multicombo_opt_t> vector, const bool toggle = false) {
		this->vector = vector;
		this->toggle = toggle;
	}

	bool is_enabled(int idx) {
		if (idx >= vector.size())
			throw std::runtime_error(std::string("multicombobox_toggle_t::is_enabled() was called with a wrong idx (out of range)"));

		return vector.at(idx).state;
	}

	operator std::vector<multicombo_opt_t>() { return vector; }
	operator bool() { return toggle; }
};
#pragma endregion

namespace menu_framework {
	void group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label);
	void update_positions();
	void init_tab();
	void hotkey(std::int32_t x, std::int32_t y, std::int32_t w, unsigned long font, const std::string string, int& target_key, bool& reading_this_hotkey);
	void add_hotkey(const std::string label, int& target_key, int& target, bool& reading_this, unsigned long font);
	void add_hotkey(const std::string label, int& target_key, int& target, bool& reading_this);
	void add_hotkey(const std::string label, hotkey_t& hotkey_info, unsigned long font);
	void add_hotkey(const std::string label, hotkey_t& hotkey_info);
	void add_column();
	void add_groupbox1(int item_number);
	void add_groupbox1(std::string name, int item_number);

	void add_bottom_groupbox1(int item_number);
	
	void tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string, std::int32_t& tab, std::int32_t count, bool show_outline);
	void check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value);

	void slider(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, float& value, float min_value, float max_value);
	
	void combobox(std::int32_t x, std::int32_t y, std::int32_t combo_right_pos, unsigned long font, const std::string label, std::vector<std::string>& opt_vec, combobox_toggle_t& target);
	void check_box1(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, int click_area_id);
	void check_box1(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, colorpicker_col_t& col);
	void check_box1(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, colorpicker_col_t& col1, colorpicker_col_t& col2);
	void slider(std::int32_t x, std::int32_t y, std::int32_t slider_pos_x, std::int32_t slider_len, unsigned long font, const std::string string, float& value, float min_value, float max_value);
	void multicombobox(std::int32_t x, std::int32_t y, std::int32_t combo_right_pos, unsigned long font, const std::string label, multicombobox_toggle_t& target);
	void add_multicombobox(std::string label, multicombobox_toggle_t& target, unsigned long font);
	void add_multicombobox(std::string label, multicombobox_toggle_t& target);
	void menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h);

	inline bool should_drag = false;
	inline bool should_move = false; 


	namespace vars {
		constexpr int top_margin = 30;
		constexpr int tab_height = 24;
		constexpr int top_margin_with_tabs = top_margin + tab_height + 1;		// See first column comment (container_width--)
		constexpr int container_margin = 5;								// Empty space between containers
		constexpr int container_padding = 10;								// Space before and after item list (top and bottom)

		constexpr int item_slider_length = 80;
		constexpr int item_checkbox_length = 11;
		constexpr int item_button_w = 30;

		constexpr int columns = 2;				// Max columns per tab (Columns the menu will be divided)
		extern int column_number;				// Current column. Will change when calling add_column()

		// o_* variables are the original ones with 0 columns (all menu width)
		extern int o_container_left_pos; 		// Will change when adding more columns
		extern int o_container_width;    		// Will get divided when adding more columns
		extern int o_item_left_pos;      		// Base top left pos for all items (label text position)

		extern int o_item_combo_pos;			// Max right pos
		extern int o_item_checkbox_pos;
		extern int o_item_slider_pos;			// Top left corner of the actual slider
		extern int o_item_hotkey_w;

		// Actual vars for items and containers. Updated in init_tab() and add_column()
		extern int container_left_pos;
		extern int container_width;
		extern int item_left_pos;
		extern int item_combo_pos;
		extern int item_checkbox_pos;
		extern int item_slider_pos;
		extern int item_hotkey_w;

		// Vars for groupbox
		extern int o_cur_part_y;
		extern int o_cur_base_item_y;		// Base y position of the items (position of the first item of the groupbox)

		extern int cur_part_items;			// Will be changed when adding groupbox
		extern int cur_part_y;
		extern int cur_base_item_y;
		extern int cur_part_h;				// Will update with each item added

		extern int button_part_item;
		extern int button_part_h;			// Need to get h first to subtract it from bottom to get top pos
		extern int button_part_y;
		extern int button_base_item_y;
	}
	void add_combobox(std::string label, std::vector<std::string>& option_vector, combobox_toggle_t& target, unsigned long font);
	void add_combobox(std::string label, std::vector<std::string>& option_vector, combobox_toggle_t& target);
	void add_checkbox(std::string label, bool& target, unsigned long font);
	void add_checkbox(std::string label, bool& target);
	void add_checkbox(std::string label, bool& target, colorpicker_col_t& color, unsigned long font);
	void add_checkbox(std::string label, bool& target, colorpicker_col_t& color);
	void add_checkbox(std::string label, bool& target, colorpicker_col_t& color1, colorpicker_col_t& color2, unsigned long font);
	void add_checkbox(std::string label, bool& target, colorpicker_col_t& color1, colorpicker_col_t& color2);
};

#pragma region POPUPS
struct color_popup_info {
	std::int32_t x;
	std::int32_t y;
	colorpicker_col_t& col_t;
};

struct combo_popup_info {
	std::int32_t x;
	std::int32_t y;
	std::int32_t w;
	std::uint32_t h;
	std::vector<std::string>& opt_vec;
	int& target_idx;
	bool& popup_toggle;
};

struct multicombo_popup_info {
	std::int32_t x;
	std::int32_t y;
	std::int32_t w;
	std::uint32_t h;
	std::vector<multicombo_opt_t>& target_vec;
	bool& popup_toggle;
};

namespace popup_system {
	// TODO: Add sub-namespaces?

	/* ------------ Functions ------------ */
	void render_popups();					// Will call each check_*_popups()
	bool mouse_in_popup(int x, int y);		// Will check if x:y is in a popup

	/* -------------- Color -------------- */
	const int win_padding = 10;
	const int slider_w = 127, slider_h = 15;					// w has to be divisible by 6 in order for the fade to be clean
	const int win_w = slider_w + win_padding * 2;
	const int win_h = slider_h * 4 + win_padding * 5;			// +1 slider and margin for each slider

	inline std::vector<color_popup_info> active_color_popups;

	/* ------------- Combobox ------------ */
	const int combo_win_padding = 4;							// For combobox and multicombobox

	inline std::vector<combo_popup_info> active_combo_popups;	// Menu combobox buttons will store here information about the popup that will be rendered
	void check_combo_popups();									// Will check for popups in the active_combo_popups vector
	void combobox_popup(combo_popup_info combo_popup);			// Will render the actual popup

	/* ---------- Multicombobox ---------- */
	inline std::vector<multicombo_popup_info> active_multicombo_popups;		// Menu multicombobox buttons will store here information about the popup that will be rendered
	void check_multicombo_popups();											// Will check for popups in the active_multicombo_popups vector
	void multicombobox_popup(multicombo_popup_info combo_popup);			// Will render the actual popup
}

