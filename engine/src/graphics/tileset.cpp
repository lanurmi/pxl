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

const Vector<int> Tileset::tilesInRegion(const pxl::String& region) const
{
	pxl::Vector<int> tiles;
	const auto reg = getRegion(region);
	if (reg == nullptr) return tiles;
	assert(reg->rect.width == _width);
	for (int j = reg->rect.y; j < reg->rect.y + reg->rect.height; j++)
	{
		for (int i = reg->rect.x; i < reg->rect.x + reg->rect.width; i++)
		{
			auto idx = j * _width + i;
			tiles.push_back(idx);
		}
	}
	return tiles;
}

pxl::i16 Tileset::tileIdInRegion(const pxl::String& region, int regiontile) const
{
	int c = 0;
	const auto reg = getRegion(region);
	assert(reg->rect.width == _width);
	return reg->rect.y * _width + reg->rect.x;
}

const Tileset::TileRegion* Tileset::getRegion(const pxl::String& region) const
{
	for (auto& it : _regions)
	{
		if (it.name == region)
		{
			return &it;
		}
	}
	return nullptr;
}

void Tileset::addRegion(const pxl::String& name, const pxl::Rect& rect)
{
	_regions.push_back(TileRegion(name, pxl::Rect( (int)rect.x / _size, (int)rect.y /_size, (int)rect.width / _size, (int)rect.height / _size)) );
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
