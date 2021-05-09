#include <pxl/graphics/blend.h>

const pxl::BlendState pxl::BlendState::Normal = pxl::BlendState(
	pxl::BlendOperation::Add,
	pxl::BlendFactor::One,
	pxl::BlendFactor::OneMinusSrcAlpha,
	pxl::BlendOperation::Add,
	pxl::BlendFactor::One,
	pxl::BlendFactor::OneMinusSrcAlpha,
	pxl::BlendMask::RGBA,
	pxl::Color(255,255,255,255)
);

const pxl::BlendState pxl::BlendState::Subtract = pxl::BlendState(
	pxl::BlendOperation::ReverseSubtract,
	pxl::BlendFactor::One,
	pxl::BlendFactor::One,
	pxl::BlendOperation::Add,
	pxl::BlendFactor::One,
	pxl::BlendFactor::One,
	pxl::BlendMask::RGBA,
	pxl::Color(255, 255, 255, 255)
);

pxl::BlendState::BlendState(BlendOperation op, BlendFactor src, BlendFactor dst) :
	color_operation(op),
	color_source(src),
	color_destination(dst),

	alpha_operation(op),
	alpha_source(src),
	alpha_destination(dst),
	mask(BlendMask::RGBA),
	color(pxl::Color::white)
{
}

pxl::BlendState::BlendState(
	BlendOperation color_op, BlendFactor color_src, BlendFactor color_dst,
	BlendOperation alpha_op, BlendFactor alpha_src, BlendFactor alpha_dst,
	BlendMask blend_mask, Color blend_rgba) :
	color_operation(color_op),
	color_source(color_src),
	color_destination(color_dst),
	alpha_operation(alpha_op),
	alpha_source(alpha_src),
	alpha_destination(alpha_dst),
	mask(blend_mask),
	color(blend_rgba)
{
}

bool pxl::BlendState::operator==(const BlendState& rhs) const
{
	return
		color_operation == rhs.color_operation && color_source == rhs.color_source && color_destination == rhs.color_destination &&
		alpha_operation == rhs.alpha_operation && alpha_source == rhs.alpha_source && alpha_destination == rhs.alpha_destination &&
		mask == rhs.mask && color == rhs.color;
}

bool pxl::BlendState::operator!=(const BlendState& rhs) const
{
	return !(*this == rhs);
}