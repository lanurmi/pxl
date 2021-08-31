#pragma once
#include <pxl/math/vec2.h>
#include <pxl/math/color.h>
#include <pxl/math/calc.h>

namespace pxl 
{
	enum class Easing
	{
		Linear,	QuadIn, QuadOut, QuadInOut,
		CubeIn, CubeOut, CubeInOut,
		QuartIn, QuartOut, QuartInOut,
		QuintIn, QuintOut, QuintInOut,
		SineIn, SineOut, SineInOut,
		CircIn, CircOut, CircInOut,
		ExpIn, ExpOut, ExpInOut,
		ElasticIn, ElasticOut, ElasticInOut,
		BackIn, BackOut, BackInOut,
		BounceIn, BounceOut, BounceInOut
	};

	using EasingFunc = float (*)(float);
	EasingFunc easingFunc(Easing easing);

	
	template<typename T>
	class Tween
	{
	public:
		Tween() : _running(false)
		{

		}
		void start(T start, T end, float time, Easing easing)
		{
			_start = start;
			_end = end;
			_easing = easing;
			_time = time;
			_running = true;
			_current_time = 0.0f;
		}
		void update(float delta)
		{
			_current_time = calc::approach(_current_time, _time, delta);

			float p = _current_time / _time;
			auto f = easingFunc(_easing);
			_value = lerp(_start, _end, f(p));
			if (_current_time == _time)
			{
				_running = false;
			}
		}
		T value()
		{
			return _value;
		}
		bool running() const
		{
			return _running;
		}
	private:
		float lerp(float v0, float v1, float t)
		{
			if (t == 0)
			{
				return v0;
			}
			else if (t == 1)
			{
				return v1;
			}
			else
			{
				return v0 + (v1 - v0) * t;
			}
		}
		Vec2 lerp(const Vec2& v0, const Vec2& v1, float t)
		{
			return Vec2(
				lerp(v0.x, v1.x, t),
				lerp(v0.y, v1.y, t));
		}
		Color lerp(const Color& v0, const Color& v1, float t)
		{
			return Color(
				lerp(v0.r, v1.r, t),
				lerp(v0.g, v1.g, t),
				lerp(v0.b, v1.b, t),
				lerp(v0.a, v1.a, t));
		}


		T _start;
		T _end;
		T _value;
		Easing _easing;
		float _time;
		float _current_time;
		bool _running;
	};

	using Vec2Tween = Tween<Vec2>;
	using ColorTween = Tween<Color>;
	using FloatTween = Tween<float>;
}
