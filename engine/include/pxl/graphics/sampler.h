#pragma once

namespace pxl
{
	enum class TextureFilter
	{
		None,
		Linear,
		Nearest
	};

	enum class TextureWrap
	{
		None,
		Clamp,
		Repeat
	};

	struct TextureSampler
	{
		TextureSampler();
		TextureSampler(TextureFilter filter, TextureWrap wrap_x, TextureWrap wrap_y);
		bool operator==(const TextureSampler& sampler) const;
		bool operator!=(const TextureSampler& sampler) const;
		TextureFilter filter;
		TextureWrap wrap_x;
		TextureWrap wrap_y;
		static const TextureSampler NearestClamp;
		static const TextureSampler LinearClamp;
		static const TextureSampler NearestRepeat;

	};
}