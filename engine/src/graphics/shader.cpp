#include <pxl/graphics/shader.h>
#include <pxl/engine.h>

const pxl::ShaderData s_default_shader = {
		"#version 330\n"
		"uniform mat4 u_matrix;\n"
		"layout(location=0) in vec2 a_position;\n"
		"layout(location=1) in vec2 a_tex;\n"
		"layout(location=2) in vec4 a_color;\n"
		"out vec2 v_tex;\n"
		"out vec4 v_col;\n"
		"void main(void)\n"
		"{\n"
		"	gl_Position = u_matrix * vec4(a_position.xy, 0, 1);\n"
		"	v_tex = a_tex;\n"
		"	v_col = a_color;\n"
		"}",
		"#version 330\n"
		"uniform sampler2D u_texture;\n"
		"in vec2 v_tex;\n"
		"in vec4 v_col;\n"
		"in vec4 v_type;\n"
		"out vec4 o_color;\n"
		"void main(void)\n"
		"{\n"
		"	vec4 color = texture(u_texture, v_tex);\n"
		"	o_color = color * v_col;\n"
		"}"
};

/*
const pxl::ShaderData s_default_shader = {
		"#version 330\n"
		"uniform mat4 u_matrix;\n"
		"layout(location=0) in vec2 a_position;\n"
		"layout(location=1) in vec2 a_tex;\n"
		"layout(location=2) in vec4 a_color;\n"
		"out vec2 v_tex;\n"
		"out vec4 v_col;\n"
		"void main(void)\n"
		"{\n"
		"	gl_Position = u_matrix * vec4(a_position.xy, 0, 1);\n"
		"	v_tex = a_tex;\n"
		"	v_col = a_color;\n"
		"}",
		"#version 330\n"
		"in vec2 v_tex;\n"
		"in vec4 v_col;\n"
		"in vec4 v_type;\n"
		"out vec4 o_color;\n"
		"void main(void)\n"
		"{\n"
		"	o_color = v_col;\n"
		"}"
};
*/
pxl::ShaderRef pxl::Shader::create(const ShaderData& data)
{
	return pxl::graphics().createShader(data);
}

pxl::ShaderRef pxl::Shader::create()
{
	return pxl::Shader::create(s_default_shader);
}

void pxl::Shader::addUniformInfo(const pxl::UniformInfo& info)
{
	_uniforms.push_back(info);
}

const pxl::vector<pxl::UniformInfo>& pxl::Shader::uniforms() const
{
	return _uniforms;
}