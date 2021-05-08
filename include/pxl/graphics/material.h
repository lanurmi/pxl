#pragma once

#include <pxl/types.h>
#include <pxl/graphics/shader.h>
#include <pxl/graphics/texture.h>
#include <pxl/graphics/sampler.h>
#include <memory>

namespace pxl
{
	class Material;
	using MaterialRef = std::shared_ptr<Material>;

	class Material
	{
	public:
		static MaterialRef create(const ShaderRef& shader);
		Material(const Material&) = delete;
		Material(Material&&) = delete;
		Material& operator=(const Material&) = delete;
		Material& operator=(Material&&) = delete;
		~Material() = default;
		void setTexture(string name, const TextureRef& texture);
		void setTexture(int slot, const TextureRef& texture);
		void setSampler(string name, const TextureSampler& sampler);
		void setSampler(int slot, const TextureSampler& sampler);
		void setFloat(string name, const float* values, i64 length);
		ShaderRef getShader() const { return _shader; }
		TextureRef getTexture(int slot, int index) const;
		TextureSampler getSampler(int slot, int index) const;
		const float* getFloats() { return _floats.data(); }
	private:
		Material(const ShaderRef& shader);
		ShaderRef _shader;
		vector<TextureRef> _textures;
		vector<TextureSampler> _samplers;
		vector<float> _floats;
	};
}