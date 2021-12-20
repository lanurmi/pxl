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
		void setTexture(String name, const TextureRef& texture);
		void setTexture(int slot, const TextureRef& texture);
		void setSampler(String name, const TextureSampler& sampler);
		void setSampler(int slot, const TextureSampler& sampler);
		void setFloat(String name, const float* values, i64 length);
		ShaderRef shader() const { return _shader; }
		TextureRef texture(int slot, int index) const;
		TextureSampler sampler(int slot, int index) const;
		const float* floats() { return _floats.data(); }
	private:
		Material(const ShaderRef& shader);
		ShaderRef _shader;
		Vector<TextureRef> _textures;
		Vector<TextureSampler> _samplers;
		Vector<float> _floats;
	};
}

