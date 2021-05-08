#include <pxl/graphics/material.h>
#include <assert.h>

static int calcUniformSize(const pxl::UniformInfo& uniform)
{
	int components = 0;
	switch (uniform.type)
	{
		case pxl::UniformType::Float: components = 1; break;
		case pxl::UniformType::Float2: components = 2; break;
		case pxl::UniformType::Float3: components = 3; break;
		case pxl::UniformType::Float4: components = 4; break;
		case pxl::UniformType::Mat3x2: components = 6; break;
		case pxl::UniformType::Mat4x4: components = 16; break;
		default:
			assert(0);
			break;
	}

	return components * uniform.array_length;
}

pxl::Material::Material(const pxl::ShaderRef& shader) : _shader(shader)
{

	auto& uniforms = _shader->getUniforms();
	int floatSize = 0;
	for (auto& u : uniforms)
	{
		if (u.type == pxl::UniformType::None) continue;

		if (u.type == pxl::UniformType::Texture2D)
		{
			for (int i = 0; i < u.array_length; i++)
			{
				_textures.push_back(TextureRef());
			}
			continue;
		}
		if (u.type == pxl::UniformType::Sampler2D)
		{
			for (int i = 0; i < u.array_length; i++)
			{
				_samplers.push_back(TextureSampler());
			}
			continue;
		}
		floatSize += calcUniformSize(u);
	}
	_floats.resize(floatSize);
}

pxl::MaterialRef pxl::Material::create(const ShaderRef& shader)
{
	assert(shader);
	return MaterialRef(new Material(shader));
}

void pxl::Material::setTexture(string name, const TextureRef& texture)
{
	int offset = 0;
	auto& uniforms = _shader->getUniforms();
	for (auto& u : uniforms)
	{
		if (u.type != pxl::UniformType::Texture2D) continue;

		if (u.name == name)
		{
			_textures[offset] = texture;
			break;
		}

		offset += u.array_length;
		if (offset >= _textures.size())
		{
			assert(0);
			break;
		}
	}
}

void pxl::Material::setTexture(int slot, const TextureRef& texture)
{
	int offset = 0;
	auto& uniforms = _shader->getUniforms();
	int s = 0;
	for (auto& u : uniforms)
	{
		if (u.type != pxl::UniformType::Texture2D) continue;

		if (slot == s)
		{
			_textures[offset] = texture;
			break;
		}

		offset += u.array_length;
		s++;
	}
}

void pxl::Material::setSampler(string name, const TextureSampler& sampler)
{
	int offset = 0;
	auto& uniforms = _shader->getUniforms();
	for (auto& u : uniforms)
	{
		if (u.type != pxl::UniformType::Sampler2D) continue;

		if (u.name == name)
		{
			_samplers[offset] = sampler;
			break;
		}

		offset += u.array_length;
		if (offset >= _textures.size())
		{
			assert(0);
			break;
		}
	}
}

void pxl::Material::setSampler(int slot, const TextureSampler& sampler)
{
	int s = 0;
	int offset = 0;
	auto& uniforms = _shader->getUniforms();
	for (auto& u : uniforms)
	{
		if (u.type != pxl::UniformType::Sampler2D) continue;

		if (s == slot)
		{
			_samplers[offset] = sampler;
			break;
		}

		offset += u.array_length;
		s++;
	}
}

void pxl::Material::setFloat(string name, const float* values, i64 length)
{
	int offset = 0;
	int index = 0;
	auto& uniforms = _shader->getUniforms();
	for (auto& u : uniforms)
	{
		if (u.type == UniformType::Texture2D ||
			u.type == UniformType::Sampler2D ||
			u.type == UniformType::None)
			continue;


		auto uniformSize = calcUniformSize(u);
		if (u.name == name)
		{
			if (length > uniformSize)
			{
				assert(0);
				length = uniformSize;
			}
			memcpy(_floats.data() + offset, values, sizeof(float) * length);
		}
		else
		{
			offset += uniformSize;
		}
	}
}

pxl::TextureRef pxl::Material::texture(int slot, int index) const
{
	int s = 0;
	int offset = 0;
	auto& uniforms = _shader->getUniforms();
	for (auto& uniform : uniforms)
	{
		if (uniform.type != UniformType::Texture2D) continue;

		if (s == slot)
		{
			if (index > uniform.array_length)
			{
				break;
			}
			return _textures[offset + index];
		}

		offset += uniform.array_length;
		if (offset + index >= _textures.size())
		{
			break;
		}

		s++;
	}
	return nullptr;
}

pxl::TextureSampler pxl::Material::getSampler(int slot, int index) const
{
	int s = 0;
	int offset = 0;
	auto& uniforms = _shader->getUniforms();
	for (auto& uniform : uniforms)
	{
		if (uniform.type != UniformType::Sampler2D)
		{
			continue;
		}

		if (s == slot)
		{
			if (index > uniform.array_length)
			{
				break;
			}

			return _samplers[offset + index];
		}

		offset += uniform.array_length;
		if (offset + index >= _samplers.size())
		{
			break;
		}

		s++;
	}

	return TextureSampler();
}