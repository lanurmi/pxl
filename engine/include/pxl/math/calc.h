#pragma once
#include <pxl/math/vec2.h>
#include <pxl/types.h>
#include <pxl/containers/vector.h>

#include <algorithm>
#include <random>

namespace pxl
{
	namespace calc
	{
		constexpr float DEG2RAD = 0.0174532925f;
		constexpr float PI = 3.141592653f;
		constexpr float TAU = 2 * PI;
		float approach(float start, float end, float shift);
		Vec2 approach(const Vec2& start, const Vec2& end, float shift);
		float pow(float x, float y);
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
	namespace random
	{
		using RngEngine = std::default_random_engine;

		void seed(pxl::i64 seed);
		int range(int min, int max);
		bool percent(int percent);
		RngEngine& engine();
		template<typename T>
		void shuffle(pxl::Vector<T>& v)
		{
			std::shuffle(v.begin(), v.end(), engine());
		}

		template<typename T>
		T& choose(pxl::Vector<T>& v)
		{
			return v[range(0, v.size())];
		}

		template<typename T>
		T choose(const pxl::Vector<T>& v)
		{
			return v[range(0, v.size())];
		}
	}
}

