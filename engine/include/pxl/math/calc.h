#pragma once
#include <pxl/math/vec2.h>

namespace pxl
{
	namespace calc
	{
		constexpr float PI = 3.141592653f;
		float approach(float start, float end, float shift);
		Vec2 approach(const Vec2& start, const Vec2& end, float shift);
		float round(float v);
		float floor(float v);
		float ceiling(float v);
		float sin(float v);
		float cos(float v);
		float tan(float v);
		float atan2(float y, float x);
		float sqrt(float x);
		float lerp(float from, float to, float t);
		int abs(int v);
		float abs(float v);
		int sign(int v);
		template<class T>
		T clamp(T value, T min, T max) { return value < min ? min : (value > max ? max : value); }
		template<class T, class U>
		T min(T a, U b) { return  (T)(a < b ? a : b); }
		template<class T, class U>
		T max(T a, U b) { return (T)(a > b ? a : b); }
	}
}