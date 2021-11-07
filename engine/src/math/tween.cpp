#include <pxl/math/tween.h>
#include <pxl/math/calc.h>

using namespace pxl;

static float linear(float t)
{
	return t;
}

static float quadIn(float t)
{
	return t * t;
}

static float quadOut(float t)
{
	return -(t * (t - 2));
}

static float quadInOut(float t)
{
	if (t < 0.5f)
		return 2 * t * t;
	else
		return (-2 * t * t) + (4 * t) - 1;
}

static float cubeIn(float t)
{
	return t * t * t;
}

static float cubeOut(float t)
{
	float f = (t - 1);
	return f * f * f + 1;
}

static float cubeInOut(float t)
{
	if (t < 0.5f)
		return 4 * t * t * t;
	else
	{
		float f = ((2 * t) - 2);
		return 0.5f * f * f * f + 1;
	}
}

static float quartIn(float t)
{
	return t * t * t * t;
}

static float quartOut(float t)
{
	float f = (t - 1);
	return f * f * f * (1 - t) + 1;
}

static float quartInOut(float t)
{
	if (t < 0.5f)
	{
		return 8 * t * t * t * t;
	}
	else
	{
		float f = (t - 1);
		return -8 * f * f * f * f + 1;
	}
}

static float quintIn(float t)
{
	return t * t * t * t * t;
}

static float quintOut(float t)
{
	float f = (t - 1);
	return f * f * f * f * f + 1;
}

static float quintInOut(float t)
{
	if (t < 0.5f)
	{
		return 16 * t * t * t * t * t;
	}
	else
	{
		float f = ((2 * t) - 2);
		return  0.5f * f * f * f * f * f + 1;
	}
}

static float sineIn(float t)
{
	return calc::sin((t - 1) * calc::PI * 0.5f) + 1;
}

static float sineOut(float t)
{
	return calc::sin(t * (calc::PI * 0.5f));
}

static float sineInOut(float t)
{
	return 0.5f * (1 - calc::cos(t * calc::PI));
}

static float circIn(float t)
{
	return 1 - calc::sqrt(1 - (t * t));
}

static float circOut(float t)
{
	return calc::sqrt((2 - t) * t);
}

static float circInOut(float t)
{
	if (t < 0.5f)
	{
		return 0.5f * (1 - calc::sqrt(1 - 4 * (t * t)));
	}
	else
	{
		return 0.5f * (calc::sqrt(-((2 * t) - 3) * ((2 * t) - 1)) + 1);
	}
}

static float expIn(float t)
{
	return (t == 0) ? 0 : calc::pow(2, 10 * (t - 1));
}

static float expOut(float t)
{
	return (t == 1) ? 1 : 1 - calc::pow(2, -10 * t);
}

static float expInOut(float t)
{
	if (t == 0 || t == 1)
	{
		return t;
	}

	if (t < 0.5f)
	{
		return 0.5f * calc::pow(2, (20 * t) - 10);
	}
	else
	{
		return -0.5f * calc::pow(2, (-20 * t) + 10) + 1;
	}
}

static float elasticIn(float t)
{
	return calc::sin(13 * (calc::PI * 0.5f) * t) * calc::pow(2, 10 * (t - 1));
}

static float elasticOut(float t)
{
	return calc::sin(-13 * (calc::PI * 0.5f) * (t + 1)) * calc::pow(2, -10 * t) + 1;
}

static float elasticInOut(float t)
{
	if (t < 0.5f)
	{
		return 0.5f * calc::sin(13 * (calc::PI * 0.5f) * (2 * t)) * calc::pow(2, 10 * ((2 * t) - 1));
	}
	else
	{
		return 0.5f * (calc::sin(-13 * (calc::PI * 0.5f) * ((2 * t - 1) + 1)) * calc::pow(2, -10 * (2 * t - 1)) + 2);
	}
}

static float backIn(float t)
{
	return t * t * t - t * calc::sin(t * calc::PI);
}

static float backOut(float t)
{
	float f = (1 - t);
	return 1 - (f * f * f - f * calc::sin(f * calc::PI));
}

static float backInOut(float t)
{
	if (t < 0.5f)
	{
		float f = 2 * t;
		return 0.5f * (f * f * f - f * calc::sin(f * calc::PI));
	}
	else
	{
		float f = (1 - (2 * t - 1));
		return 0.5f * (1 - (f * f * f - f * calc::sin(f * calc::PI))) + 0.5f;
	}
}

static float bounceOut(float t)
{
	if (t < 4 / 11.0f)
	{
		return (121 * t * t) / 16.0f;
	}
	else if (t < 8 / 11.0f)
	{
		return (363 / 40.0f * t * t) - (99 / 10.0f * t) + 17 / 5.0f;
	}
	else if (t < 9 / 10.0f)
	{
		return (4356 / 361.0f * t * t) - (35442 / 1805.0f * t) + 16061 / 1805.0f;
	}
	else
	{
		return (54 / 5.0f * t * t) - (513 / 25.0f * t) + 268 / 25.0f;
	}
}

static float bounceIn(float t)
{
	return 1 - bounceOut(1 - t);
}

static float bounceInOut(float t)
{
	if (t < 0.5f)
		return 0.5f * bounceIn(t * 2);
	else
		return 0.5f * bounceOut(t * 2 - 1) + 0.5f;
}

pxl::EasingFunc pxl::easingFunc(pxl::Easing easing)
{
	switch (easing) {
		case Easing::Linear: return &linear; break;
		case Easing::QuadIn: return &quadIn; break;
		case Easing::QuadOut: return &quadOut; break;
		case Easing::QuadInOut: return &quadInOut; break;
		case Easing::CubeIn: return &cubeIn; break;
		case Easing::CubeOut: return &cubeOut; break;
		case Easing::CubeInOut: return &cubeInOut; break;
		case Easing::QuartIn: return &quartIn; break;
		case Easing::QuartOut: return &quartOut; break;
		case Easing::QuartInOut: return &quartInOut; break;
		case Easing::QuintIn: return &quintIn; break;
		case Easing::QuintOut: return &quintOut; break;
		case Easing::QuintInOut: return &quintInOut; break;
		case Easing::SineIn: return &sineIn; break;
		case Easing::SineOut: return &sineOut; break;
		case Easing::SineInOut: return &sineInOut; break;
		case Easing::CircIn: return &circIn; break;
		case Easing::CircOut: return &circOut; break;
		case Easing::CircInOut: return &circInOut; break;
		case Easing::ExpIn: return &expIn; break;
		case Easing::ExpOut: return &expOut; break;
		case Easing::ExpInOut: return &expInOut; break;
		case Easing::ElasticIn: return &elasticIn; break;
		case Easing::ElasticOut: return &elasticOut; break;
		case Easing::ElasticInOut: return &elasticInOut; break;
		case Easing::BackIn: return &backIn; break;
		case Easing::BackOut: return &backOut; break;
		case Easing::BackInOut: return &backInOut; break;
		case Easing::BounceIn: return &bounceIn; break;
		case Easing::BounceOut: return &bounceOut; break;
		case Easing::BounceInOut: return &bounceInOut; break;
	}
	return nullptr;
}

float pxl::ease(float t, Easing ease)
{
	auto f = easingFunc(ease);
	return f(t);
}