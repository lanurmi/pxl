#include <pxl/math/calc.h>
#include <cmath>
#include <assert.h>

float pxl::calc::approach(float start, float end, float shift)
{
	if (start < end)
	{
		return min(start + shift, end);
	}
	else
	{
		return max(start - shift, end);
	}
}

pxl::Vec2 pxl::calc::approach(const Vec2& start, const Vec2& end, float shift)
{
	assert(shift >= 0.0f);
	auto toVector = end - start;
	auto sqrdistance = toVector.lengthSquared();
	if (sqrdistance == 0 || sqrdistance <= shift * shift)
	{
		return end;
	}
	return start + toVector.normal() * shift;
}

float pxl::calc::pow(float x, float y)
{
	return powf(x, y);
}

float pxl::calc::round(float v)
{
	return roundf(v);
}

float pxl::calc::floor(float v)
{
	return floorf(v);
}

float pxl::calc::ceiling(float v)
{
	return ceilf(v);
}

float pxl::calc::sin(float v)
{
	return sinf(v);
}

float pxl::calc::cos(float v)
{
	return cosf(v);
}

float pxl::calc::tan(float v)
{
	return tanf(v);
}

float pxl::calc::atan2(float y, float x)
{
	return atan2f(y, x);
}

float pxl::calc::sqrt(float x)
{
	return sqrtf(x);
}

float pxl::calc::lerp(float from, float to, float t)
{
	if (from == to) return from;

	return from + (to - from) * t;
}

int pxl::calc::abs(int v)
{
	if (v < 0)
	{
		return -v;
	}
	else
	{
		return v;
	}
}

float pxl::calc::abs(float v)
{
	if (v < 0)
	{
		return -v;
	}
	else
	{
		return v;
	}
}

int pxl::calc::sign(int v)
{
	if (v == 0)
	{
		return 0;
	}
	else if (v < 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
	
}




namespace 
{
	pxl::random::RngEngine randomEngine;
	pxl::i64 currentSeed;
}

pxl::random::RngEngine& pxl::random::engine()
{
	return randomEngine;
}

void pxl::random::seed(pxl::i64 seed)
{
	currentSeed = seed;
	randomEngine = pxl::random::RngEngine(seed);
}

int pxl::random::range(int min, int max)
{
	assert(min < max);
	std::uniform_int_distribution<> dst(min, max - 1);
	return dst(randomEngine);
}

bool pxl::random::percent(int percent)
{
	return pxl::random::range(1, 100) <= percent;
}