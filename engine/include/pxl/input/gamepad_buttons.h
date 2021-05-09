#pragma once

namespace pxl
{
	enum class Button
	{
		Invalid = -1,
		A = 0,
		B = 1,
		X = 2,
		Y = 3,
		Back = 4,
		Select = 5,
		Start = 6,
		LeftStick = 7,
		RightStick = 8,
		LeftShoulder = 9,
		RightShoulder = 10,
		Up = 11,
		Down = 12,
		Left = 13,
		Right = 14
	};

	enum class Axis
	{
		Invalid = -1,
		LeftX = 0,
		LeftY = 1,
		RightX = 2,
		RightY = 3,
		LeftTrigger = 4,
		RightTrigger = 5
	};
}