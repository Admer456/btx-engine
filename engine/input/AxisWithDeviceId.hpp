
#pragma once

// This was written for Input::axes
// It is essentially a combination of an input axis + a device ID that is hashable
// Before supporting multiple devices, the input system could only register a single
// InputAxis instance, which was mapped to a unique InputAxisCode.
// 
// Supporting multiple devices was implemented the easiest way by simply
// registering multiple InputAxis instances with different device IDs, but
// pointing to the same InputAxisCode.
// 
// It is now possible to do something like
//  axes[{InputAxisCode::ControllerLeftTrigger, 1}]
// in order to access the 2nd controller's left trigger, but the simple old
//  axes[InputAxisCode::ControllerLeftTrigger] 
// will work just fine too, defaulting to the 1st device.
struct AxisWithDeviceId
{
	AxisWithDeviceId( InputAxisCode::Enum code, int device = 0 )
		: axisCode( code ), deviceId( device )
	{
	}

	size_t operator()() const noexcept
	{
		return axisCode + (1 << (10 + deviceId));
	}

	bool operator==( const AxisWithDeviceId& other ) const
	{
		return axisCode == other.axisCode && deviceId == other.deviceId;
	}

	InputAxisCode::Enum axisCode;
	int deviceId;
};

template<>
struct std::hash<AxisWithDeviceId>
{
	std::size_t operator()( AxisWithDeviceId const& other ) const noexcept
	{
		return other();
	}
};
