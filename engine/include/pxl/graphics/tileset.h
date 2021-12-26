#pragma once
#include <pxl/graphics/Subtexture.h>

namespace pxl
{
	class Tileset
	{
	public:
		Tileset();
		Tileset(const TextureRef& texture, int tilesize);
		Tileset(const Subtexture& texture, int tilesize);
		Tileset(Tileset &&other) noexcept;
		Tileset& operator=(Tileset &&other) noexcept;
		const Subtexture &tile(i16 index) const;
		int size() const;
		const Vector<Subtexture> &tiles() const { return _tiles;}
		int widthInTiles() const;
		int heightInTiles() const;
	private:
		void createTiles(const TextureRef &texture, const Rect& area, int tilesize);
		int _width;
		int _height;
		Vector<Subtexture> _tiles;
		int _size;
	};
}

