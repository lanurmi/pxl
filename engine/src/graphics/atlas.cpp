#include <pxl/graphics/atlas.h>
#include <pxl/backends/filesystem_backend.h>
#include <pxl/graphics/subtexture.h>
#include <pxl/log.h>

using namespace pxl;

Atlas::Info::Info(const String& name, u32 packIndex, const Aseprite& ase) : name(name), pack_index(packIndex), aseprite(ase), size(0)
{
}

Atlas::Info::Info(const String& name, u32 packIndex, int size, const Aseprite& ase) : name(name), pack_index(packIndex), aseprite(ase), size(size) 
{
}

Atlas::Atlas() : _packIndex(0), _packer(2048, 2048)
{
}

Atlas::~Atlas()
{
}

void Atlas::addSprite(const String& name, const Aseprite& ase)
{
	_spriteInfos.push_back(Info(name, _packIndex, ase));
	auto &sprite = _spriteInfos.back();
	if(!sprite.aseprite.frames.empty())
	{
		for (auto& frame : sprite.aseprite.frames)
		{
			_packer.add(_packIndex, frame.image);
			_packIndex++;
		}
	}
}

void Atlas::addTileset(const String& name, const Aseprite& ase, int tilesize) {
	_tilesetInfos.push_back(Info(name, _packIndex, tilesize, ase));
	auto& t = _tilesetInfos.back();
	if (!t.aseprite.frames.empty())
	{
		for (auto& frame : t.aseprite.frames)
		{
			_packer.add(_packIndex, frame.image);
			_packIndex++;
		}
	}
}

void Atlas::build()
{
	if (_tilesetInfos.empty() && _spriteInfos.empty()) return;

	Map<u32, Subtexture> subtextures;
	const auto& result = _packer.pack();
	for (auto& it : result)
	{
		_page = pxl::Texture::create(it.image);
		for (auto& s : it.entries) 		{
			subtextures[s.id] = Subtexture(_page, Rect(s.x, s.y, s.width, s.height));
		}
		break;
	}

	if (result.size() != 1U)
	{
		pxl::log::error("Atlas is too small");
		assert(result.size() == 1U);
	}

	// Save sprites
	for (auto& it : _spriteInfos)
	{
		Sprite sprite;
		sprite.name = it.name;
		sprite.origin = Vec2::zero;
		if (!it.aseprite.slices.empty() && it.aseprite.slices[0].has_pivot)
		{
			sprite.origin = it.aseprite.slices[0].pivot;
		}

		for (auto& tag : it.aseprite.tags)
		{
			Sprite::Animation anim;
			anim.name = tag.name;
			for (int i = tag.from; i <= tag.to; i++)
			{
				Sprite::Frame frame;
				frame.duration = it.aseprite.frames[i].duration / 1000.0f;
				frame.texture = subtextures[it.pack_index + i];
				anim.frames.push_back(frame);
			}
			sprite.animations.push_back(anim);
		}
		_sprites[it.name] = sprite;
	}

	// Save tilesets
	for (auto& it : _tilesetInfos)
	{
		_tilesets[it.name] = Tileset(subtextures[it.pack_index], it.size);
	}

	_packer.clear();
}

const Sprite* Atlas::sprite(const String& name) const
{
	auto it = _sprites.find(name);
	if (it != _sprites.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}

const Tileset* Atlas::tileset(const String& name) const
{
	auto it = _tilesets.find(name);
	if (it != _tilesets.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}
