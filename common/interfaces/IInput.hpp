
#pragma once

// IInput.hpp
// Input system interface

// Indicators of whether a key was released or not
struct InputKeyState
{
	enum Enum : uint8_t
	{
		// The key is currently released
		Released = 1 << 0,
		// The key was just released this frame
		BecameReleased = 1 << 1,
		// The key is currently held
		Held = 1 << 2,
		// The key was just pressed this frame
		BecameHeld = 1 << 3
	};
};

using InputKeyFlags = uint8_t;

// Identifiers for input axes, this can be anything from gyroscopes to mice
// Generally anything that isn't a PC keyboard, including mouse buttons
struct InputAxisCode
{
	enum Enum : uint8_t
	{
		None = 0,
		// Absolute mouse X in the window, 0 to windowWidth, left to right
		MouseX,
		// Absolute mouse Y in the window, 0 to windowHeight, top to bottom
		MouseY,
		MouseXRelative,
		MouseYRelative,

		MouseButtonLeft,
		MouseButtonMiddle,
		MouseButtonRight,
		// Extra mouse buttons
		MouseButtonAux1,
		MouseButtonAux2,

		MouseWheel,

		// Controller axes

		ControllerLeftStickHorizontal,
		ControllerLeftStickVertical,
		ControllerRightStickHorizontal,
		ControllerRightStickVertical,

		ControllerLeftTrigger,
		ControllerRightTrigger,

		// Controller buttons

		ControllerLeftStickClick,
		ControllerRightStickClick,

		ControllerDpadUp,
		ControllerDpadDown,
		ControllerDpadLeft,
		ControllerDpadRight,
	
		// XBox Elite paddle
		ControllerPaddle1,
		ControllerPaddle2,
		ControllerPaddle3,
		ControllerPaddle4,

		ControllerButtonStart,
		ControllerButtonBack,
		ControllerButtonGuide,
		// PS4/PS5 touchpad button
		ControllerButtonTouchpad,
		// Xbox Series X share button
		// PS5 microphone button
		ControllerButtonMisc,

		ControllerButtonA,
		ControllerButtonB,
		ControllerButtonX,
		ControllerButtonY,

		ControllerLeftShoulder,
		ControllerRightShoulder,

		// TODO: Controller sensors and touchpads

		MAX
	};
};

// Keyboard scancodes, maps directly to SDL2's scancodes
struct InputKeyCode
{
	enum Enum : uint8_t
	{
		A = 4,
		B = 5,
		C = 6,
		D = 7,
		E = 8,
		F = 9,
		G = 10,
		H = 11,
		I = 12,
		J = 13,
		K = 14,
		L = 15,
		M = 16,
		N = 17,
		O = 18,
		P = 19,
		Q = 20,
		R = 21,
		S = 22,
		T = 23,
		U = 24,
		V = 25,
		W = 26,
		X = 27,
		Y = 28,
		Z = 29,

		Number1 = 30,
		Number2 = 31,
		Number3 = 32,
		Number4 = 33,
		Number5 = 34,
		Number6 = 35,
		Number7 = 36,
		Number8 = 37,
		Number9 = 38,
		Number0 = 39,

		Return = 40,
		Escape = 41,
		Backspace = 42,
		Tab = 43,
		Space = 44,

		Minus = 45,
		Equals = 46,
		LeftBracket = 47,
		RightBracket = 48,
		Backslash = 49,
		
		Semicolon = 51,
		Apostrophe = 52,
		Grave = 53,
		Comma = 54,
		Period = 55,
		Slash = 56,
		
		CapsLock = 57,

		F1 = 58,
		F2 = 59,
		F3 = 60,
		F4 = 61,
		F5 = 62,
		F6 = 63,
		F7 = 64,
		F8 = 65,
		F9 = 66,
		F10 = 67,
		F11 = 68,
		F12 = 69,

		PrintScreen = 70,
		ScrollLock = 71,
		Pause = 72,
		Insert = 73,

		Home = 74,
		PageUp = 75,
		Delete = 76,
		End = 77,
		PageDown = 78,
		Right = 79,
		Left = 80,
		Down = 81,
		Up = 82,
		NumLock = 83,

		KeyPadDivide = 84,
		KeyPadMultiply = 85,
		KeyPadMinus = 86,
		KeyPadPlus = 87,
		KeyPadEnter = 88,
		KeyPad1 = 89,
		KeyPad2 = 90,
		KeyPad3 = 91,
		KeyPad4 = 92,
		KeyPad5 = 93,
		KeyPad6 = 94,
		KeyPad7 = 95,
		KeyPad8 = 96,
		KeyPad9 = 97,
		KeyPad0 = 98,
		KeyPadPeriod = 99,

		NonUSBackslash = 100,
		Application = 101,
		Power = 102,

		KeyPadEquals = 103,

		F13 = 104,
		F14 = 105,
		F15 = 106,
		F16 = 107,
		F17 = 108,
		F18 = 109,
		F19 = 110,
		F20 = 111,
		F21 = 112,
		F22 = 113,
		F23 = 114,
		F24 = 115,

		Execute = 116,
		Help = 117,
		Menu = 118,
		Select = 119,
		Stop = 120,
		Redo = 121,
		Undo = 122,
		Cut = 123,
		Copy = 124,
		Paste = 125,
		Find = 126,
		Mute = 127,
		VolumeUp = 128,
		VolumeDown = 129,

		KeyPadComma = 133,
		KeyPadEqualsAs400 = 134,

		International1 = 135,
		International2 = 136,
		International3 = 137,
		International4 = 138,
		International5 = 139,
		International6 = 140,
		International7 = 141,
		International8 = 142,
		International9 = 143,

		Language1 = 144,
		Language2 = 145,
		Language3 = 146,
		Language4 = 147,
		Language5 = 148,
		Language6 = 149,
		Language7 = 150,
		Language8 = 151,
		Language9 = 152,

		SystemRequirements = 154,
		Cancel = 155,
		Clear = 156,
		Prior = 157,
		Return2 = 158,
		Separator = 159,
		Out = 160,
		Oper = 161,
		ClearAgain = 162,
		CrSel = 163,
		ExSel = 164,

		KeyPad00 = 176,
		KeyPad000 = 177,

		ThousandsSeparator = 178,
		DecimalSeparator = 179,
		CurrencyUnit = 180,
		CurencySubUnit = 181,

		KeyPadLeftParenthesis = 182,
		KeyPadRightParenthesis = 183,
		KeyPadLeftBrace = 184,
		KeyPadRightBrace = 185,
		KeyPadTab = 186,
		KeyPadBackspace = 187,
		KeyPadA = 188,
		KeyPadB = 189,
		KeyPadC = 190,
		KeyPadD = 191,
		KeyPadE = 192,
		KeyPadF = 193,
		KeyPadXor = 194,
		KeyPadPower = 195,
		KeyPadPercent = 196,
		KeyPadLess = 197,
		KeyPadGreater = 198,
		KeyPadAmpersand = 199,
		KeyPadDoubleAmpersand = 200,
		KeyPadVerticalBar = 201,
		KeyPadDoubleVerticalBar = 202,
		KeyPadColon = 203,
		KeyPadHash = 204,
		KeyPadSpace = 205,
		KeyPadAt = 206,
		KeyPadExclamation = 207,
		KeyPadMemoryStore = 208,
		KeyPadMemoryRecall = 209,
		KeyPadMemoryClear = 210,
		KeyPadMemoryAdd = 211,
		KeyPadMemorySubtract = 212,
		KeyPadMemoryMultiply = 213,
		KeyPadMemoryDivide = 214,
		KeyPadPlusMinus = 215,
		KeyPadClear = 216,
		KeyPadClearEntry = 217,
		KeyPadBinary = 218,
		KeyPadOctal = 219,
		KeyPadDecimal = 220,
		KeyPadHexadecimal = 221,

		LeftCtrl = 224,
		LeftShift = 225,
		LeftAlt = 226,
		LeftGUI = 227,
		RightCtrl = 228,
		RightShift = 229,
		RightAlt = 230,
		RightGUI = 231,
	};
};

constexpr int ScancodeUninitialized = -1;

// Input system
class IInput
{
public:
	virtual bool Init() = 0;
	virtual void Shutdown() = 0;

	// Polls for input updates and updates all registered keys & axes
	virtual void Update() = 0;

	// @param axis: InputAxisCode:: entry
	// @param deviceId: Device ID for controllers etc.
	// @returns A [-1,1] value for the given axis
	virtual float GetAxis( InputAxisCode::Enum axis, const int& deviceId = 0 ) const = 0;
	
	// Axes and buttons are the same concept here
	// @param button: InputAxisCode:: entry
	// @param deviceId: Device ID for controllers etc.
	// @returns A key state for the given axis button
	virtual InputKeyFlags GetButton( InputAxisCode::Enum button, const int& deviceId = 0 ) const = 0;

	// @returns A key state for the given scancode
	virtual InputKeyFlags GetKey( const int& key ) const = 0;

	// If the mouse is grabbed, it will become invisible and locked to the centre of the screen.
	// If it's not grabbed, it will behave like usual, suitable for main menus or editors.
	virtual void SetMouseGrab( bool grab ) = 0;

	// @returns Whether or not the mouse is grabbed, see SetMouseGrab
	virtual bool IsMouseGrabbed() const = 0;

	// Did the user attempt to close the window?
	virtual bool IsWindowClosing() const = 0;
};
