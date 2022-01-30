#pragma once
#include <pxl/graphics/Subtexture.h>
#include <pxl/containers/vector.h>

namespace pxl
{
	class Tileset
	{
	public:
		struct TileRegion
		{
			TileRegion(const pxl::String& name, const pxl::Rect& rect) : name(name), rect(rect) {}
			TileRegion() {}
			pxl::Rect rect;
			pxl::String name;
		};

		Tileset();
		Tileset(const TextureRef& texture, int tilesize);
		Tileset(const Subtexture& texture, int tilesize);
		Tileset(Tileset &&other) noexcept;
		void addRegion(const pxl::String &name, const pxl::Rect &rect);
		Tileset& operator=(Tileset &&other) noexcept;
		const Subtexture &tile(i16 index) const;
		const Vector<int> tilesInRegion(const pxl::String& region) const;
		pxl::i16 tileIdInRegion(const pxl::String& region, int regiontile) const;
		const TileRegion *getRegion(const pxl::String& region) const;
		int size() const;
		const Vector<Subtexture> &tiles() const { return _tiles;}
		int widthInTiles() const;
		int heightInTiles() const;
	private:
		void createTiles(const TextureRef &texture, const Rect& area, int tilesize);
		int _width;
		int _height;
		Vector<Subtexture> _tiles;
		Vector<TileRegion> _regions;
		int _size;
	};
}

