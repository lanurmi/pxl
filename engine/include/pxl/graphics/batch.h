#pragma once

#include <pxl/types.h>
#include <pxl/math/mat3x2.h>
#include <pxl/math/mat4x4.h>
#include <pxl/math/rect.h>
#include <pxl/graphics/vertex.h>
#include <pxl/graphics/sampler.h>
#include <pxl/graphics/shader.h>
#include <pxl/graphics/texture.h>
#include <pxl/graphics/mesh.h>
#include <pxl/graphics/material.h>
#include <pxl/graphics/blend.h>
#include <pxl/graphics/rendertarget.h>
#include <pxl/graphics/spritefont.h>

namespace pxl
{
	class BatchInfo
	{
	public:
		BatchInfo();
		int offset;
		int elements;
		BlendState blend;
		MaterialRef material;
		TextureRef texture;
		TextureSampler sampler;
		bool flip_vertically;
	};

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
		void draw();
	};

	struct BatchStatistics {
		int draw_calls;
		int triangles;
	};
	class Batch
	{
	public:
		Batch();
		Batch(const Batch& other) = delete;
	//	Batch& operator=(const Batch& other) = delete;
		~Batch();
		void begin(const RenderTargetRef& target, const pxl::Color &clearColor);
		void end();
		void clear();
		void pushMatrix(const Mat3x2& matrix);
		void popMatrix();
		void pushMaterial(const MaterialRef& material);
		void pushBlend(const BlendState& blend);
		void pushSampler(const TextureSampler& sampler);
		void popSampler();
		void popBlend();
		void popMaterial();
		//draw
		void triangle(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Color& color);
		void rectangle(const Rect& rect, const Color& color);
		void hollowRectangle(const Rect& rect, const Color& color);
		void line(const Vec2& from, const Vec2& to, int lineSize, const Color& color);
		void texture(const pxl::TextureRef& texture, const pxl::Vec2& pos, const pxl::Vec2& origin, const pxl::Vec2& scale, float rotation, const pxl::Color& color);
		void texture(const pxl::TextureRef& texture, const pxl::Vec2& pos, const pxl::Color& color);
		void texture(const pxl::TextureRef& texture, const Rect& dstRect, const Rect &srcrect, const pxl::Color& color);
		void texture(const pxl::Subtexture& texture, const pxl::Vec2& pos, const pxl::Vec2& origin, const pxl::Vec2& scale, float rotation, const pxl::Color& color);
		void texture(const pxl::Subtexture& texture, const pxl::Vec2& pos, const pxl::Color& color);
		void text(const pxl::SpriteFont &font, const String &text, const pxl::Vec2 &pos, const pxl::Color &color);
		//
		const BatchStatistics &stats() const;
	private:
		void pushQuad(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3,
			const Vec2& t0, const Vec2& t1, const Vec2& t2, const Vec2& t3, const Color& color);
		void pushQuad(const Rect& rect, const Rect &texcoords, const Color& color);
		void setTexture(const TextureRef& texture);
		void newBatch();
		void draw(const RenderTargetRef& renderTarget, const Mat4x4& matrix);
		void drawBatch(const RenderTargetRef& renderTarget, const pxl::Mat4x4& matrix, const BatchInfo& batch);

		const Mat3x2& currentMatrix();
		BatchInfo& currentBatch();

		Mat3x2 _current_matrix;
		Vector<Mat3x2> _matrix_stack;
		Vector<MaterialRef> _material_stack;
		Vector<BlendState> _blend_stack;
		Vector<BatchInfo> _batches;
		Vector<TextureSampler> _samplerStack;

		Vector<Vertex> _vertices;
		Vector<pxl::u32> _indices;
		MeshRef _mesh;
		ShaderRef m_defaultShader;
		MaterialRef m_defaultMaterial;
		TextureRef m_defaultTexture;

		RenderTargetRef _target;

		BatchStatistics _stats;

		pxl::String _textureUniform;
		pxl::String _samplerUniform;
	};
}