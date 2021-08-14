#include <pxl/graphics/shader.h>
#include <pxl/engine.h>

pxl::ShaderRef pxl::Shader::create(const ShaderData& data)
{
	return pxl::graphics::createShader(data);
}

pxl::ShaderRef pxl::Shader::create()
{
	return pxl::graphics::createDefaultShader();
}

void pxl::Shader::addUniformInfo(const pxl::UniformInfo& info)
{
	_uniforms.push_back(info);
}

const pxl::Vector<pxl::UniformInfo>& pxl::Shader::uniforms() const
{
	return _uniforms;
}