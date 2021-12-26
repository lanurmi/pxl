#include <pxl/graphics/tileset.h>
#include <pxl/engine.h>

using namespace pxl;

Tileset::Tileset()
{

}

Tileset::Tileset(const TextureRef& texture, int tilesize)
{
	createTiles(texture, Rect(0,0,texture->width(), texture->height()), tilesize);
}

Tileset::Tileset(const Subtexture& texture, int tilesize)
{
	createTiles(texture.texture(), texture.rect(), tilesize);
}

Tileset::Tileset(Tileset&& other) noexcept
{
	_tiles = std::move(other._tiles);
	_size = other._size;
	_width = other._width;
	_height = other._height;
}

Tileset& Tileset::operator=(Tileset&& other) noexcept
{
	_tiles = std::move(other._tiles);
	_size = other._size;
	_width = other._width;
	_height = other._height;
	other._size = 0;
	other._width = 0;
	other._height = 0;
	return *this;
}

void Tileset::createTiles(const TextureRef& texture, const Rect& area, int tilesize)
{
	if ((int)area.width % tilesize)
	{
		pxl::log::error("tileset texture width is not divisible by tilesize");
	}

	if ((int)area.height % tilesize)
	{
		pxl::log::error("tileset texture width is not divisible by tilesize");
	}

	for (int j = area.y; j < (int)(area.y + area.height); j += tilesize)
	{
		for (int i = area.x; i < (int)(area.x + area.width); i += tilesize)
		{
			_tiles.push_back(Subtexture(texture, pxl::Rect(i, j, tilesize, tilesize)));
		}
	}
	_size = tilesize;

	_width = area.width / tilesize;
	_height = area.height / tilesize;
}

const Subtexture& Tileset::tile(i16 index) const
{
	if (index < 0 || index >= _tiles.size())
	{
		pxl::log::error("Invalid tileset index");
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


int Tileset::widthInTiles() const
{
	return _width;
}

int Tileset::heightInTiles() const
{
	return _height;
}
