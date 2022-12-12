
#define INPUT_KEY_WAITING -1
#define INPUT_KEY_NONE -2

enum analog_code_t {
	ANALOG_CODE_INVALID = -1,
	MOUSE_X = 0,
	MOUSE_Y,
	MOUSE_XY, // invoked when either x or y changes
	MOUSE_WHEEL,
	ANALOG_CODE_LAST = 10,
};

struct KeyStateInfo {
	/*
	 * .pressed will store:
	 *   true when keydown and it was not held before (1st time),
	 *   false when keyup or when keydown + being held
	 * .held will store:
	 *   true if the key is currently being held down
	 *   false if the key is released
	 */
	bool pressed;
	bool held;
};

enum button_code_t{ 
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,				// A fake button which is 'pressed' and 'released' when the wheel is moved up
	MOUSE_WHEEL_DOWN,			// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,

};

class hotkey_t {
public:
	int key;
	bool reading_this;

	hotkey_t(const int key, const bool reading_this = false) {
		this->key = key;
		this->reading_this = reading_this;
	}

	operator int() { return key; }
	operator bool() { return reading_this; }
};
#pragma endregion




class i_inputsytem {
public:

	bool reading_textbox = false;
	std::string wndproc_textbox_buffer = "";
	bool reading_hotkey = false;
	int latest_hotkey = INPUT_KEY_NONE;

	void enable_input(bool bEnable) {
		using original_fn = void(__thiscall*)(void*, bool);
		return (*(original_fn * *)this)[11](this, bEnable);
	}

	void reset_input_state() {
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn * *)this)[39](this);
	}

	bool is_button_down(button_code_t code) {
		using original_fn = bool(__thiscall*)(void*, button_code_t);
		return (*(original_fn * *)this)[15](this, code);
	}

	int get_analog_value(analog_code_t code) {
		using original_fn = int(__thiscall*)(void*, analog_code_t);
		return (*(original_fn * *)this)[18](this, code);

	}

	inline bool is_held(const int vKey) const {
		if (vKey == INPUT_KEY_NONE) return true;        // If a hotkey is set to "None" is the same as always on

		if (vKey < 0) return false;                     // Should not happen
		if (vKey == latest_hotkey) return false;        // Avoid toggling the key when assigning

		return key_states[vKey].held;
	}

	inline bool is_pressed(const int vKey) const {
		if (vKey < 0) return false;                                         // Keys like HOTKEY_WAITING or HOTKEY_NONE should be checked with IsPressed()
		if (vKey == latest_hotkey) return false;                            // Avoid toggling the key when assigning
		if (reading_hotkey) return false;

		return key_states[vKey].pressed;                                    // See comment on GlobalInput::WndProcUpdate()
	}

	int get_analog_delta(analog_code_t code) {
		using original_fn = int(__thiscall*)(void*, analog_code_t);
		return (*(original_fn * *)this)[19](this, code);
	}

	const char* button_code_to_string(int code) {
		using original_fn = const char* (__thiscall*)(void*, int);
		return (*(original_fn * *)this)[40](this, code);
	}
private:
	KeyStateInfo key_states[256];       // Will be updated by WndProc
	KeyStateInfo key_states_old[256];   // Will be updated every paint_traverse iteration with the old key_states[]
};