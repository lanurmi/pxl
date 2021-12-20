#include <pxl/graphics/drawcall.h>
#include <pxl/backends/graphics_backend.h>

void pxl::DrawCall::draw() {
	pxl::Vec2 size = pxl::Vec2(target->width(), target->height());
	viewport = pxl::Rect(0.0f, 0.0f, size.x, size.y);
	assert(material);
	pxl::graphics::render(*this);
}

