#include <pxl/graphics/sampler.h>

const pxl::TextureSampler pxl::TextureSampler::NearestClamp;
const pxl::TextureSampler pxl::TextureSampler::NearestRepeat = pxl::TextureSampler(pxl::TextureFilter::Nearest, pxl::TextureWrap::Repeat, pxl::TextureWrap::Repeat);
const pxl::TextureSampler pxl::TextureSampler::LinearClamp = pxl::TextureSampler(pxl::TextureFilter::Linear, pxl::TextureWrap::Repeat, pxl::TextureWrap::Repeat);

pxl::TextureSampler::TextureSampler(TextureFilter filter, TextureWrap wrap_x, TextureWrap wrap_y) : filter(filter), wrap_x(wrap_x), wrap_y(wrap_y)
{
}

pxl::TextureSampler::TextureSampler() : filter(pxl::TextureFilter::Nearest), wrap_x(pxl::TextureWrap::Clamp), wrap_y(pxl::TextureWrap::Clamp)
{
}

bool pxl::TextureSampler::operator==(const pxl::TextureSampler& sampler) const
{
	return sampler.filter == filter && sampler.wrap_x == wrap_x && sampler.wrap_y == wrap_y;
}

bool pxl::TextureSampler::operator!=(const pxl::TextureSampler& sampler) const
{
	return sampler.filter != filter || sampler.wrap_x != wrap_x || sampler.wrap_y != wrap_y;
}

