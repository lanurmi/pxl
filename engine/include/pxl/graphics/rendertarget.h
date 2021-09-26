#pragma once

#include <pxl/math/color.h>
#include <pxl/graphics/texture.h>
#include <memory>

namespace pxl
{
	class RenderTarget;
	using RenderTargetRef = std::shared_ptr<RenderTarget>;

	class RenderTarget
	{
	public:
		static RenderTargetRef create(int width, int height);
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget(RenderTarget&&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;
		RenderTarget& operator=(RenderTarget&&) = delete;
		virtual ~RenderTarget() = default;
		virtual int width() const;
		virtual int height() const;
		virtual void clear(const pxl::Color& color) = 0;
		virtual TextureRef texture() const = 0;
	protected:
		RenderTarget() = default;
	};
}