#include <pxl/graphics/batch.h>
#include <pxl/backends/graphics_backend.h>
#include <pxl/math/calc.h>
#include <assert.h>

pxl::BatchInfo::BatchInfo() :
	offset(0), elements(0), blend(pxl::BlendState::Normal), material(nullptr), texture(nullptr), sampler(pxl::TextureSampler::NearestClamp) {
}


pxl::Batch::Batch() : _textureUniform("u_texture"), _samplerUniform("u_texture_sampler") {
	clear();
}

pxl::Batch::~Batch() {
}

void pxl::Batch::draw(RenderTargetRef target) {
	pxl::Vec2 size = pxl::Vec2(target->width(), target->height());;
	auto mat = Mat4x4::createOrthoOffcenter(0, size.x, size.y, 0, 0.01f, 1000.0f);
	draw(target, mat);
}
void pxl::Batch::clear() {
	_vertices.clear();
	_indices.clear();
	_batches.clear();
	_matrix_stack.clear();
	_material_stack.clear();
	_sampler_stack.clear();
	_blend_stack.clear();


	_sampler_stack.clear();
	_blend_stack.clear();

	_current_blend = pxl::BlendState::Normal;
	_current_sampler = pxl::TextureSampler::NearestClamp;

	_current_matrix = Mat3x2::identity;

	newBatch();
}

void pxl::Batch::pushMatrix(const Mat3x2& matrix)
{
	_matrix_stack.push_back(_current_matrix);
	_current_matrix = matrix * _current_matrix;
}

void pxl::Batch::popMatrix()
{
	assert(_matrix_stack.size() >= 1U);
	_current_matrix = _matrix_stack.back();
	_matrix_stack.pop_back();
}

void pxl::Batch::pushMaterial(const MaterialRef& material)
{
	{
		_material_stack.push_back(_current_material);
		_current_material = material;
		auto& cBatch = currentBatch();
		if (cBatch.elements > 0 && cBatch.material != _current_material)
		{
			newBatch();
		}
	}
	currentBatch().material = _current_material;
}

void pxl::Batch::pushBlend(const BlendState& blend) {
	{
		_blend_stack.push_back(_current_blend);
		_current_blend = blend;
		auto& cBatch = currentBatch();
		if (cBatch.elements > 0 && cBatch.blend != _current_blend)
		{
			newBatch();
		}
	}

	currentBatch().blend = _current_blend;
}

void pxl::Batch::popBlend() {
	{
		auto& cBatch = currentBatch();
		_current_blend = _blend_stack.back();
		_blend_stack.pop_back();
		if (cBatch.elements > 0 && cBatch.blend != _current_blend)
		{
			newBatch();
		}
	}

	currentBatch().blend = _current_blend;
}

void pxl::Batch::popMaterial()
{
	_current_material = _material_stack.back();
	_material_stack.pop_back();
	auto& cBatch = currentBatch();
	if (cBatch.elements > 0 && cBatch.material != _current_material)
	{
		newBatch();
	}
	currentBatch().material = _current_material;
}

void pxl::Batch::pushSampler(const pxl::TextureSampler& sampler) {
	{
		_sampler_stack.push_back(_current_sampler);
		_current_sampler = sampler;
		auto& cBatch = currentBatch();
		if (cBatch.elements > 0 && cBatch.sampler != _current_sampler)
		{
			newBatch();
		}
	}
	currentBatch().sampler = _current_sampler;
}

void pxl::Batch::popSampler()
{
	{
		_current_sampler = _sampler_stack.back();
		_sampler_stack.pop_back();
		auto& cBatch = currentBatch();
		if (cBatch.elements > 0 && cBatch.sampler != _current_sampler)
		{
			newBatch();
		}
	}
	currentBatch().sampler = _current_sampler;
}

void pxl::Batch::pushScissors(const Rect& scissors) {
	{
		_scissor_stack.push_back(_currentScissors);
		_currentScissors = scissors;
		auto& cBatch = currentBatch();
		if (cBatch.elements > 0 && cBatch.scissors != _currentScissors)
		{
			newBatch();
		}
	}
	currentBatch().scissors = _currentScissors;
}

void pxl::Batch::popScissors()
{
	{
		_currentScissors = _scissor_stack.back();
		_scissor_stack.pop_back();
		auto& cBatch = currentBatch();
		if (cBatch.elements > 0 && cBatch.scissors != _currentScissors)
		{
			newBatch();
		}
	}
	currentBatch().scissors = _currentScissors;
}

void pxl::Batch::newBatch()
{
	if (_batches.empty())
	{
		BatchInfo newBatch;
		newBatch.offset = 0;
		newBatch.elements = 0;
		newBatch.blend = _current_blend;
		newBatch.sampler = _current_sampler;
		_batches.push_back(newBatch);
	}
	else
	{
		auto& cBatch = currentBatch();
		BatchInfo newBatch;
		newBatch.offset = (cBatch.elements + cBatch.offset);
		newBatch.elements = 0;
		newBatch.blend = _current_blend;
		newBatch.sampler = _current_sampler;
		newBatch.flip_vertically = cBatch.flip_vertically;
		_batches.push_back(newBatch);
	}
}

pxl::BatchInfo& pxl::Batch::currentBatch() {
	return _batches.back();
}

const pxl::Mat3x2& pxl::Batch::currentMatrix() {
	return _current_matrix;
}

void pxl::Batch::setTexture(const TextureRef& texture) {
	{
		auto& cBatch = currentBatch();
		if (cBatch.elements > 0 && cBatch.texture != texture)
		{
			newBatch();
		}
	}
	auto& batch = currentBatch();
	batch.texture = texture;
	batch.flip_vertically = pxl::graphics::features().origin_bottom_left && texture && texture->isRenderTarget();
}

void pxl::Batch::triangle(const pxl::Vec2& p0, const pxl::Vec2& p1, const pxl::Vec2& p2, const pxl::Color& color) {
	auto& mat = currentMatrix();
	auto idx = _vertices.size();
	_indices.push_back(idx);
	_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p0, mat), pxl::Vec2::zero, color));

	_indices.push_back(idx + 1);
	_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p1, mat), pxl::Vec2::zero, color));

	_indices.push_back(idx + 2);
	_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p2, mat), pxl::Vec2::zero, color));

	auto& cBatch = currentBatch();
	cBatch.elements++;
}

void pxl::Batch::rectangle(const Rect& rect, const Color& color) {
	setTexture(nullptr);
	auto position = rect.topLeft();
	auto size = rect.size();
	pushQuad(position, pxl::Vec2(position.x + size.x, position.y), pxl::Vec2(position.x + size.x, position.y + size.y), pxl::Vec2(position.x, position.y + size.y),
		pxl::Vec2::zero, pxl::Vec2(1.0f, 0.0f), pxl::Vec2(1.0f, 1.0f), pxl::Vec2(0.0f, 1.0f), color);
}

void pxl::Batch::hollowRectangle(const Rect& rect, const Color& color, unsigned borderSize) {
	setTexture(nullptr);
	//tl tr

	pushQuad(Rect(rect.left(), rect.top(), borderSize,  rect.height), pxl::Rect(0, 0, 1, 1), color);
	pushQuad(Rect(rect.right() - borderSize , rect.top(), borderSize, rect.height), pxl::Rect(0, 0, 1, 1), color);
	pushQuad(Rect(rect.left(), rect.top(), rect.width, borderSize), pxl::Rect(0, 0, 1, 1), color);
	pushQuad(Rect(rect.left(), rect.bottom() - borderSize, rect.width, borderSize), pxl::Rect(0, 0, 1, 1), color);
}

void pxl::Batch::pushQuad(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3,
	const Vec2& t0, const Vec2& t1, const Vec2& t2, const Vec2& t3, const Color& color) {
	auto& mat = currentMatrix();
	auto idx = _vertices.size();
	_indices.push_back(idx + 0);
	_indices.push_back(idx + 1);
	_indices.push_back(idx + 2);

	_indices.push_back(idx + 0);
	_indices.push_back(idx + 2);
	_indices.push_back(idx + 3);

	if (currentBatch().flip_vertically)
	{
		pxl::Vec2 tt0(t0.x, 1.0f - t0.y);
		pxl::Vec2 tt1(t1.x, 1.0f - t1.y);
		pxl::Vec2 tt2(t2.x, 1.0f - t2.y);
		pxl::Vec2 tt3(t3.x, 1.0f - t3.y);

		_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p0, mat), tt0, color));
		_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p1, mat), tt1, color));
		_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p2, mat), tt2, color));
		_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p3, mat), tt3, color));
	} 	else
	{
		_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p0, mat), t0, color));
		_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p1, mat), t1, color));
		_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p2, mat), t2, color));
		_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p3, mat), t3, color));
	}

	auto& cBatch = currentBatch();
	cBatch.elements += 2;
}

void pxl::Batch::circle(const Vec2& center, float radius, int steps, const Color &centerColor, const Color &outerColor)
{
	Vec2 last = Vec2(center.x + radius, center.y);

	for (int i = 1; i <= steps; i++)
	{
		const auto radians = (i / (float)steps) * pxl::calc::TAU;
		const auto next = Vec2(center.x + pxl::calc::cos(radians) * radius, center.y + pxl::calc::sin(radians) * radius);

		pushTriangle(last, next, center, pxl::Vec2::zero, pxl::Vec2::zero, pxl::Vec2::zero, outerColor, outerColor, centerColor);

		last = next;
	}
}

void pxl::Batch::pushTriangle(const Vec2& p0, const Vec2& p1, const Vec2& p2,
	const Vec2& t0, const Vec2& t1, const Vec2& t2, const Color& color0, const Color& color1, const Color& color2) {
	auto& mat = currentMatrix();
	auto idx = _vertices.size();
	_indices.push_back(idx + 0);
	_indices.push_back(idx + 1);
	_indices.push_back(idx + 2);
	
	//assert(currentBatch().flip_vertically == false);

	_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p0, mat), t0, color0));
	_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p1, mat), t1, color1));
	_vertices.push_back(pxl::Vertex(pxl::Vec2::transform(p2, mat), t2, color2));

	auto& cBatch = currentBatch();
	cBatch.elements += 1;
}

void pxl::Batch::pushQuad(const Rect& rect, const Rect& texcoords, const Color& color) {
	pushQuad(rect.topLeft(), rect.topRight(), rect.bottomRight(), rect.bottomLeft(),
		texcoords.topLeft(), texcoords.topRight(), texcoords.bottomRight(), texcoords.bottomLeft(), color);
}

void pxl::Batch::line(const Vec2& from, const Vec2& to, int lineSize, const Color& color) {
	setTexture(nullptr);
	assert(from != to);

	auto normal = (to - from).normal();
	Vec2 p = Vec2(normal.y, -normal.x);

	Vec2 p0 = from + p * lineSize * 0.5f;
	Vec2 p1 = to + p * lineSize * 0.5f;
	Vec2 p2 = to - p * lineSize * 0.5f;
	Vec2 p3 = from - p * lineSize * 0.5f;

	pushQuad(p0, p1, p2, p3,
		pxl::Vec2::zero, pxl::Vec2(1.0f, 0.0f), pxl::Vec2(1.0f, 1.0f), pxl::Vec2(0.0f, 1.0f), color);
}

void pxl::Batch::texture(const pxl::TextureRef& texture, const pxl::Vec2& pos, const pxl::Vec2 &srcPos, const pxl::Vec2 &srcSize, const pxl::Color& color)
{
	setTexture(texture);

	auto tl = pos;
	auto tr = pos + pxl::Vec2(srcSize.x, 0.0f);
	auto br = pos + pxl::Vec2(srcSize.x, srcSize.y);
	auto bl = pos + pxl::Vec2(0.0f, srcSize.y);

	auto tw = 1.0f / texture->width();
	auto th = 1.0f / texture->height();

	auto t0 = pxl::Vec2(srcPos.x * tw, srcPos.y * th);

	auto t1 = pxl::Vec2((srcPos.x + srcSize.x) * tw, srcPos.y * th);
	auto t2 = pxl::Vec2((srcPos.x + srcSize.x) * tw, (srcPos.y + srcSize.y) * th);
	auto t3 = pxl::Vec2(srcPos.x * tw, (srcPos.y + srcSize.y) * th);

	pushQuad(tl, tr, br, bl, t0, t1, t2, t3, color);
}

void pxl::Batch::texture(const pxl::TextureRef& texture, const pxl::Vec2& pos, const pxl::Vec2& origin, const pxl::Vec2& scale, float rotation, const pxl::Color& color)
{
	pushMatrix(pxl::Mat3x2::createTransform(pos, origin, scale, rotation));
	setTexture(texture);
	const float width = texture->width();
	const float height = texture->height();
	pushQuad(pxl::Rect(0.0f, 0.0f, width, height), pxl::Rect(0.0f, 0.0f, 1.0f, 1.0f), color);
	popMatrix();
}

void pxl::Batch::texture(const pxl::TextureRef& texture, const pxl::Vec2& pos, const pxl::Color& color)
{
	setTexture(texture);
	const float width = texture->width();
	const float height = texture->height();
	pushQuad(pxl::Rect(pos.x, pos.y, width, height), pxl::Rect(0.0f, 0.0f, 1.0f, 1.0f), color);
}


void pxl::Batch::texture(const pxl::Subtexture& subtexture, const pxl::Vec2& pos, const pxl::Vec2& origin, const pxl::Vec2& scale, float rotation, const pxl::Color& color)
{
	pushMatrix(pxl::Mat3x2::createTransform(pos, origin, scale, rotation));
	auto texture = subtexture.texture();
	setTexture(texture);
	const float width = texture->width();
	const float height = texture->height();
	auto srcRect = subtexture.rect();
	pushQuad(pxl::Rect(0.0f, 0.0f, srcRect.width, srcRect.height), pxl::Rect(srcRect.x / width, srcRect.y / height, srcRect.width / width, srcRect.height / height), color);
	popMatrix();
}

void pxl::Batch::texture(const pxl::Subtexture& subtexture, const pxl::Vec2& pos, const pxl::Color& color)
{
	auto tex = subtexture.texture();
	texture(tex, Rect(pos.x, pos.y, (float)subtexture.width(), (float)subtexture.height()), subtexture.rect(), color);
}

void pxl::Batch::texture(const pxl::TextureRef& texture, const Rect& dstRect, const Rect& srcrect, const pxl::Color& color)
{
	setTexture(texture);
	float width = texture->width();
	float height = texture->height();
	Rect texcoords(srcrect.x / width, srcrect.y / height, srcrect.width / width, srcrect.height / height);
	pushQuad(dstRect, texcoords, color);
}


void pxl::Batch::text(const pxl::SpriteFont& font, const String& text, const pxl::Vec2& pos, const pxl::Color& color)
{
	Vec2 drawPos = pos + pxl::Vec2(0, font.ascent() - font.descent());
	u32 prev = 0;
	for (unsigned i = 0; i < text.size(); i++) 	{
		u32 glyph = static_cast<u32>(text[i]);

		if (text[i] == '\n')
		{
			drawPos.y += font.lineHeight();
			drawPos.x = pos.x;
			continue;
		}


		int kerning = i == 0 ? 0 : font.kerning(prev, glyph);
		drawPos.x += kerning;

		if (const auto character = font.character(glyph))
		{
			if (auto tex = character->subtexture.texture())
			{
				texture(character->subtexture, drawPos + character->offset, color);
			}
			drawPos.x += character->advance;
		}

		prev = glyph;
		i += text.utf8Size(i) - 1;
	}
}

const pxl::BatchStatistics& pxl::Batch::stats() const
{
	return _stats;
}

const pxl::VertexFormat format = pxl::VertexFormat(
	{
		{ 0, pxl::VertexType::Float2, false }, //position
		{ 1, pxl::VertexType::Float2, false }, // tex
		{ 2, pxl::VertexType::UByte4, true } // col
	}
);


void pxl::Batch::draw(const RenderTargetRef& renderTarget, const Mat4x4& matrix)
{
	auto& cBatch = currentBatch();
	if ((_batches.size() == 1U && _batches.back().elements == 0) || _indices.empty()) return;

	if (!_mesh)
	{
		_mesh = Mesh::create();
	}

	if (!_current_material)
	{
		_current_material = Material::create( Shader::create() );
	}

	if (!m_defaultTexture) 	{
		pxl::u8 pixel[4] = { 255,255,255,255 };
		m_defaultTexture = pxl::Texture::create(1, 1, pxl::TextureFormat::RGBA, pixel);
	}

	_mesh->setIndexData(pxl::IndexFormat::UInt32, _indices.data(), _indices.size());
	_mesh->setVertexData(format, _vertices.data(), _vertices.size());


	for (int i = 0; i < _batches.size(); i++) 	{
		drawBatch(renderTarget, matrix, _batches[i]);
	}
}

void pxl::Batch::drawBatch(const RenderTargetRef& renderTarget, const pxl::Mat4x4& matrix, const BatchInfo& batch)
{
	if (batch.elements == 0) return;

	_stats.draw_calls++;
	_stats.triangles += batch.elements;
	DrawCall drawcall;
	drawcall.mesh = _mesh;
	drawcall.useScissors = batch.scissors.width != 0 && batch.scissors.height != 0;
	drawcall.scissors = batch.scissors;

	drawcall.material = batch.material == nullptr ? _current_material : batch.material;
	if (drawcall.material == _current_material && batch.texture == nullptr) 
	{
		drawcall.material->setTexture(_textureUniform, m_defaultTexture);
		drawcall.material->setSampler(_samplerUniform, batch.sampler);
	} 	
	else
	{
		drawcall.material->setTexture(_textureUniform, batch.texture);
		drawcall.material->setSampler(_samplerUniform, batch.sampler);
	}
	drawcall.target = renderTarget;
	drawcall.material->setFloat("u_matrix", &matrix.m11, 16);
	drawcall.indices_start = batch.offset * 3;
	drawcall.indices_count = batch.elements * 3;
	drawcall.blend = batch.blend;
	drawcall.draw();
}
