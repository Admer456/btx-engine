// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

struct AxisHandlerFlags
{
	enum Type
	{
		// Generates device IDs if this belongs to a controller
		// One AxisHandler can thus be used for multiple controller devices
		GenerateDeviceIds = 1 << 0,
		// Responds to 'up' and 'down' events
		Binary = 1 << 1
	};
};

// I found this to be the most elegant solution for now
// We do not have many axes, so this is fine and manageable
using AxisHandlerFn = float( const SDL_Event& e, const int& deviceId );
struct AxisHandler
{
	// Our ID for this axis
	InputAxisCode::Enum axisCode{ InputAxisCode::None };
	// Which event type to respond to
	// Is 0 for auto-handled axes
	int sdlEventCode{ 0 };
	// How to handle the event
	// Is null for auto-handled axes
	AxisHandlerFn* handlerFunction{ nullptr };
	// Look at AxisHandlerFlags
	int flags{ 0 };
	
	template<InputAxisCode::Enum code, int eventType>
	static constexpr AxisHandler Generic( AxisHandlerFn* handler )
	{
		return
		{
			code, eventType, handler
		};
	}

	static constexpr AxisHandler Automatic( InputAxisCode::Enum code )
	{
		return { code };
	}

	template<InputAxisCode::Enum code, int sdlButton>
	static constexpr AxisHandler ForMouseButton()
	{
		return
		{
			code, SDL_MOUSEBUTTONDOWN, []( const SDL_Event& e, const int& deviceId )
			{
				if ( e.button.button != sdlButton )
				{
					return InputAxis::InvalidValue;
				}

				return e.type == SDL_MOUSEBUTTONDOWN ? 1.0f : 0.0f;
			},
			AxisHandlerFlags::Binary
		};
	}

	// Shorten typing here
	using IAC = InputAxisCode;

	template<typename T>
	struct AxisMapping
	{
		IAC::Enum code;
		T mapped;
	};

	static constexpr inline AxisMapping<SDL_GameControllerAxis> ControllerAxisMappings[] =
	{
		{ IAC::ControllerLeftStickHorizontal, SDL_CONTROLLER_AXIS_LEFTX },
		{ IAC::ControllerLeftStickVertical, SDL_CONTROLLER_AXIS_LEFTY },
		{ IAC::ControllerRightStickHorizontal, SDL_CONTROLLER_AXIS_RIGHTX },
		{ IAC::ControllerRightStickVertical, SDL_CONTROLLER_AXIS_RIGHTY },
		{ IAC::ControllerLeftTrigger, SDL_CONTROLLER_AXIS_TRIGGERLEFT },
		{ IAC::ControllerRightTrigger, SDL_CONTROLLER_AXIS_TRIGGERRIGHT }
	};

	template<InputAxisCode::Enum code>
	static constexpr AxisHandler ForControllerAxis()
	{
		static_assert(code >= IAC::ControllerLeftStickHorizontal && code <= IAC::ControllerRightTrigger,
			"The input axis must be between ControllerLeftStickHorizontal and ControllerRightTrigger to avoid going out of bounds!");

		return
		{
			code, SDL_CONTROLLERAXISMOTION, []( const SDL_Event& e, const int& deviceId )
			{
				constexpr int index = code - IAC::ControllerLeftStickHorizontal;
				constexpr SDL_GameControllerAxis controllerAxis = ControllerAxisMappings[index].mapped;

				if ( e.caxis.axis != controllerAxis )
				{
					return InputAxis::InvalidValue;
				}

				return e.caxis.value / 32767.0f;
			},
			AxisHandlerFlags::GenerateDeviceIds
		};
	}

	static constexpr inline AxisMapping<SDL_GameControllerButton> ControllerButtonBindings[] =
	{
		{ IAC::ControllerLeftStickClick, SDL_CONTROLLER_BUTTON_LEFTSTICK },
		{ IAC::ControllerRightStickClick, SDL_CONTROLLER_BUTTON_RIGHTSTICK },
		{ IAC::ControllerDpadUp, SDL_CONTROLLER_BUTTON_DPAD_UP },
		{ IAC::ControllerDpadDown, SDL_CONTROLLER_BUTTON_DPAD_DOWN },
		{ IAC::ControllerDpadLeft, SDL_CONTROLLER_BUTTON_DPAD_LEFT },
		{ IAC::ControllerDpadRight, SDL_CONTROLLER_BUTTON_DPAD_RIGHT },
		{ IAC::ControllerPaddle1, SDL_CONTROLLER_BUTTON_PADDLE1 },
		{ IAC::ControllerPaddle2, SDL_CONTROLLER_BUTTON_PADDLE2 },
		{ IAC::ControllerPaddle3, SDL_CONTROLLER_BUTTON_PADDLE3 },
		{ IAC::ControllerPaddle4, SDL_CONTROLLER_BUTTON_PADDLE4 },
		{ IAC::ControllerButtonStart, SDL_CONTROLLER_BUTTON_START },
		{ IAC::ControllerButtonBack, SDL_CONTROLLER_BUTTON_BACK },
		{ IAC::ControllerButtonGuide, SDL_CONTROLLER_BUTTON_GUIDE },
		{ IAC::ControllerButtonTouchpad, SDL_CONTROLLER_BUTTON_TOUCHPAD },
		{ IAC::ControllerButtonMisc, SDL_CONTROLLER_BUTTON_MISC1 },
		{ IAC::ControllerButtonA, SDL_CONTROLLER_BUTTON_A },
		{ IAC::ControllerButtonB, SDL_CONTROLLER_BUTTON_B },
		{ IAC::ControllerButtonX, SDL_CONTROLLER_BUTTON_X },
		{ IAC::ControllerButtonY, SDL_CONTROLLER_BUTTON_Y },
		{ IAC::ControllerLeftShoulder, SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
		{ IAC::ControllerRightShoulder, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER }
	};

	template<InputAxisCode::Enum code>
	static constexpr AxisHandler ForControllerButton()
	{
		static_assert(code >= IAC::ControllerLeftStickClick && code <= IAC::ControllerRightShoulder,
			"The input axis must be between ControllerLeftStickClick and ControllerRightShoulder to avoid going out of bounds!");

		return
		{
			code, SDL_CONTROLLERBUTTONDOWN, []( const SDL_Event& e, const int& deviceId )
			{
				constexpr int index = code - IAC::ControllerLeftStickClick;
				constexpr SDL_GameControllerButton controllerButton = ControllerButtonBindings[index].mapped;

				if ( e.cbutton.button != controllerButton )
				{
					return InputAxis::InvalidValue;
				}

				return e.cbutton.state == SDL_PRESSED ? 1.0f : 0.0f;
			},
			AxisHandlerFlags::GenerateDeviceIds | AxisHandlerFlags::Binary
		};
	}
};

constexpr AxisHandler AxisHandlers[] =
{
	// Mouse coordinates are automatically handled
	// They're just here for sake of being registered
	AxisHandler::Automatic( InputAxisCode::MouseX ),
	AxisHandler::Automatic( InputAxisCode::MouseY ),
	AxisHandler::Automatic( InputAxisCode::MouseXRelative ),
	AxisHandler::Automatic( InputAxisCode::MouseYRelative ),

	// Everything else is more or less handled automatically thanks to templates
	AxisHandler::ForMouseButton<InputAxisCode::MouseButtonLeft, SDL_BUTTON_LEFT>(),
	AxisHandler::ForMouseButton<InputAxisCode::MouseButtonMiddle, SDL_BUTTON_MIDDLE>(),
	AxisHandler::ForMouseButton<InputAxisCode::MouseButtonRight, SDL_BUTTON_RIGHT>(),
	AxisHandler::ForMouseButton<InputAxisCode::MouseButtonAux1, SDL_BUTTON_X1>(),
	AxisHandler::ForMouseButton<InputAxisCode::MouseButtonAux2, SDL_BUTTON_X2>(),
	AxisHandler::Generic<InputAxisCode::MouseWheel, SDL_MOUSEWHEEL>( []( const SDL_Event& e, const int& deviceId )
	{
		return static_cast<float>(e.wheel.y);
	} ),

	AxisHandler::ForControllerAxis<InputAxisCode::ControllerLeftStickHorizontal>(),
	AxisHandler::ForControllerAxis<InputAxisCode::ControllerLeftStickVertical>(),
	AxisHandler::ForControllerAxis<InputAxisCode::ControllerRightStickHorizontal>(),
	AxisHandler::ForControllerAxis<InputAxisCode::ControllerRightStickVertical>(),
	AxisHandler::ForControllerAxis<InputAxisCode::ControllerLeftTrigger>(),
	AxisHandler::ForControllerAxis<InputAxisCode::ControllerRightTrigger>(),

	AxisHandler::ForControllerButton<InputAxisCode::ControllerLeftStickClick>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerRightStickClick>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerDpadUp>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerDpadDown>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerDpadLeft>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerDpadRight>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerPaddle1>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerPaddle2>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerPaddle3>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerPaddle4>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerButtonStart>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerButtonBack>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerButtonGuide>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerButtonTouchpad>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerButtonMisc>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerButtonA>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerButtonB>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerButtonX>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerButtonY>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerLeftShoulder>(),
	AxisHandler::ForControllerButton<InputAxisCode::ControllerRightShoulder>()
};

constexpr size_t NumAxisHandlers = sizeof( AxisHandlers ) / sizeof( AxisHandler );
