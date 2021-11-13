#pragma once
#include <pxl/types.h>
#include <pxl/assets/image.h>
#include <memory>

namespace pxl
{
	enum class TextureFormat
	{
		None,
		RGBA
	};

	class Texture;
	using TextureRef = std::shared_ptr<Texture>;
	using TextureWeakRef = std::weak_ptr<Texture>;
	class Texture
	{
	public:
		static TextureRef create(int width, int height, TextureFormat format, u8* data);
		static TextureRef create(int width, int height, TextureFormat format);
		static TextureRef create(const pxl::Image &image);
		virtual ~Texture() = default;
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) = delete;
		virtual int width() const = 0;
		virtual int height() const = 0;
		virtual TextureFormat format() const = 0;
		virtual void setData(u8* data) = 0;
		virtual bool isRenderTarget() const = 0;
	protected:
		Texture();
	};
}