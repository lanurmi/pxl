#pragma once
#include <pxl/math/color.h>
#include <pxl/graphics/texture.h>
#include <pxl/graphics/shader.h>
#include <pxl/graphics/mesh.h>
#include <pxl/graphics/batch.h>
#include <pxl/graphics/rendertarget.h>

namespace pxl
{
	class PlatformBackend;

	namespace graphics
	{
		struct GraphicsFeatures
		{
			bool origin_bottom_left = false;
			int max_texture_size = 0;
		};

		void init();
		void bind();
		void unbind();
		void render(const DrawCall& pass);
		void clearBackbuffer(const Color& color);
		GraphicsFeatures features();

		TextureRef createTexture(int width, int height, TextureFormat format);
		ShaderRef createShader(const ShaderData& data);
		ShaderRef createDefaultShader();
		ShaderRef createPaletteShader();
		MeshRef createMesh();
		RenderTargetRef createRenderTarget(int width, int height);
	}
}