#pragma once
#include <pxl/math/color.h>
#include <pxl/graphics/texture.h>
#include <pxl/graphics/shader.h>
#include <pxl/graphics/mesh.h>
#include <pxl/graphics/drawcall.h>
#include <pxl/graphics/rendertarget.h>

namespace pxl
{
	class PlatformBackend;

	namespace graphics
	{
		enum class Api
		{
			OpenGL
		};

		struct GraphicsFeatures
		{
			bool origin_bottom_left = false;
			int max_texture_size = 0;
		};
		Api api();
		void init();
		void awake();
		void destroy();
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