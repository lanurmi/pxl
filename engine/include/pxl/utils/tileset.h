#pragma once
#include <pxl/graphics/Subtexture.h>

namespace pxl
{
	class Tileset
	{
	public:
		Tileset() = default;
		Tileset(const TextureRef& texture, int tilesize);
		Tileset(const Subtexture& texture, int tilesize);
		const Subtexture &tile(i16 index) const;
		int size() const;
		const Vector<Subtexture> &tiles() const { return _tiles;}
	private:
		void createTiles(const TextureRef &texture, const Rect& area, int tilesize);
		Vector<Subtexture> _tiles;
		int _size;
	};
}