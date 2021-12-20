#pragma once

#include <pxl/graphics/blend.h>
#include <pxl/graphics/mesh.h>
#include <pxl/graphics/rendertarget.h>
#include <pxl/graphics/material.h>

namespace pxl
{
	class DrawCall
	{
	public:
		MeshRef mesh;
		MaterialRef material;
		RenderTargetRef target;
		BlendState blend;
		i64 indices_start;
		i64 indices_count;
		Rect viewport;
		bool useScissors = false;
		Rect scissors;
		void draw();
	};
}

