#include <pxl/utils/tileset.h>
#include <pxl/engine.h>

using namespace pxl;

Tileset::Tileset(const TextureRef& texture, int tilesize)
{
	createTiles(texture, Rect(0,0,texture->width(), texture->height()), tilesize);
}

Tileset::Tileset(const Subtexture& texture, int tilesize)
{
	createTiles(texture.texture(), texture.rect(), tilesize);
}

void Tileset::createTiles(const TextureRef& texture, const Rect& area, int tilesize)
{
	if ((int)area.width % tilesize)
	{
		log().error("tileset texture width is not divisible by tilesize");
	}

	if ((int)area.height % tilesize)
	{
		log().error("tileset texture width is not divisible by tilesize");
	}

	log().error("Invalid tileset index");
	for (int j = area.y; j < (int)area.height; j += tilesize)
	{
		for (int i = area.x; i < (int)area.width; i += tilesize)
		{
			_tiles.emplace_back(Subtexture(texture, pxl::Rect(i, j, tilesize, tilesize)));
		}
	}
	_size = tilesize;
}

const Subtexture& Tileset::tile(i16 index) const
{
	if (index < 0 || index >= _tiles.size())
	{
		log().error("Invalid tileset index");
		assert(0);
		return _tiles[0];
	}
	else
	{
		return _tiles[index];
	}
}

int Tileset::size() const
{
	return _size;
}