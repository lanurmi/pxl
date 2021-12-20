#pragma once
#include <pxl/types.h>
#include <pxl/math/color.h>

namespace pxl
{
	enum class BlendOperation
	{
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max
	};

	enum class BlendFactor
	{
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DstAlpha,
		OneMinusDstAlpha,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,
		SrcAlphaSaturate,
		Src1Color,
		OneMinusSrc1Color,
		Src1Alpha,
		OneMinusSrc1Alpha
	};
	enum class BlendMask
	{
		Empty = 0,
		Red = 1,
		Green = 2,
		Blue = 4,
		Alpha = 8,
		RGB = Red | Green | Blue,
		RGBA = Red | Green | Blue | Alpha,
	};

	struct BlendState
	{
		static const BlendState Normal;
		static const BlendState Subtract;
		static const BlendState Multiply;
		static const BlendState NormalNonPremultiplied;

		BlendOperation color_operation;
		BlendFactor color_source;
		BlendFactor color_destination;

		BlendOperation alpha_operation;
		BlendFactor alpha_source;
		BlendFactor alpha_destination;
		BlendMask mask;
		Color color;

		BlendState() = default;

		BlendState(BlendOperation op, BlendFactor src, BlendFactor dst);
		BlendState(
			BlendOperation color_op, BlendFactor color_src, BlendFactor color_dst,
			BlendOperation alpha_op, BlendFactor alpha_src, BlendFactor alpha_dst,
			BlendMask blend_mask, Color blend_rgba);

		bool operator==(const BlendState& rhs) const;
		bool operator!=(const BlendState& rhs) const;
	};
}

