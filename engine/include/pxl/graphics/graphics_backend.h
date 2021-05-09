#pragma once
#include <pxl/math/color.h>
#include <pxl/graphics/texture.h>
#include <pxl/graphics/shader.h>
#include <pxl/graphics/mesh.h>
#include <pxl/graphics/batch.h>
#include <pxl/graphics/render_target.h>

namespace pxl
{
	struct GraphicsFeatures
	{
		bool origin_bottom_left = false;
		int max_texture_size = 0;
	};

	class PlatformBackend;

	class GraphicsBackend
	{
	public:
		GraphicsBackend();
		~GraphicsBackend();
		void bind(PlatformBackend& platform);
		void unbind(PlatformBackend& platform);
		void render(const DrawCall& pass);
		void clearBackbuffer(const Color& color);
		GraphicsFeatures features() const;

		TextureRef createTexture(int width, int height, TextureFormat format) const;
		ShaderRef createShader(const ShaderData& data) const;
		MeshRef createMesh() const;
		RenderTargetRef createRenderTarget(int width, int height);
	};
}