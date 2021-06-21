#include <pxl/content.h>
#include <pxl/types.h>
#include <pxl/utils/image_packer.h>
#include <pxl/utils/filestream.h>
#include <pxl/engine.h>
#include <pxl/assets/aseprite.h>
#include <pxl/time.h>

using namespace pxl;

void ContentPack::addSprite(const string& path)
{
	auto applicationPath = pxl::engine().applicationPath();
	auto file = path::combine(applicationPath, path);
	if (file::exists(file) && path::extension(file) == ".ase")
	{
		_sprites.push_back(file);
	}
}

void ContentPack::addSpriteDirectory(const string &path)
{
	auto applicationPath = pxl::engine().applicationPath();
	auto files = directory::files(path::combine(applicationPath, path));
	for (auto& it : files)
	{
		if (path::extension(it) == ".ase")
		{
			_sprites.push_back(it);
		}
	}
}

void ContentPack::addTileset(const string& path, int size)
{
	auto applicationPath = pxl::engine().applicationPath();
	auto file = path::combine(applicationPath, path);
	if (file::exists(file) && path::extension(file) == ".ase")
	{
		TilesetInfo info;
		info.path = file;
		info.tilesize = size;
		_tilesets.push_back(info);
	}
}

void ContentPack::addTilesetDirectory(const string& path, int size)
{
	auto applicationPath = pxl::engine().applicationPath();
	auto files = directory::files(path::combine(applicationPath, path));
	for (auto& it : files)
	{
		if (path::extension(it) == ".ase")
		{
			TilesetInfo info;
			info.path = it;
			info.tilesize = size;
			_tilesets.push_back(info);
		}
	}
}

void Content::load(const ContentPack& content)
{
	time::Timer timer;
	loadImages(content);
	auto elapsed = timer.ms();
	log().message(string_format("Content pack loaded in %llums", elapsed));
}

void Content::loadImages(const ContentPack& pack)
{
	struct Info
	{
		string name;
		u32 pack_index;
		int tileset_size;
		Aseprite asesprite;
	};

	ImagePacker packer(2048, 2048);
	i32 packIndex = 0;

	//Tilesets
	std::vector<Info> tilesetinfos;
	for (auto& file : pack._tilesets)
	{
		if (path::extension(file.path) == ".ase")
		{
			Info info;
			info.name = path::filename(file.path);
			info.asesprite = Aseprite(file.path);
			info.tileset_size = file.tilesize;
			info.pack_index = packIndex;
			packer.add(packIndex, info.asesprite.frames[0].image);
			packIndex++;
			tilesetinfos.emplace_back(info);
		}
	}

	//sprites
	
	std::vector<Info> spriteinfos;
	std::vector<string> asefiles;
	for (auto &file : pack._sprites)
	{
		if (path::extension(file) == ".ase" && file::exists(file))
		{
			asefiles.push_back(file);
		}
	}

	for (auto it : asefiles)
	{
		Info info;
		info.name = path::filename(it);
		info.asesprite = Aseprite(it);
		info.pack_index = packIndex;
		for (auto& frame : info.asesprite.frames)
		{
			packer.add(packIndex, frame.image);
			packIndex++;
		}
		spriteinfos.emplace_back(info);
	}

	// Pack all
	map<u32, Subtexture> subtextures;
	const auto& result = packer.pack();
	for (auto& it : result)
	{
		auto atlas = pxl::Texture::create(it.image);
		for (auto& s : it.entries)
		{
			subtextures[s.id] = Subtexture(atlas, Rect(s.x, s.y, s.width, s.height));
		}
	}
	
	// Save tilesets
	for (auto& it : tilesetinfos)
	{
		_tilesets[it.name] = Tileset(subtextures[it.pack_index], it.tileset_size);
	}

	// Save sprites
	for (auto& it : spriteinfos)
	{
		Sprite sprite;
		sprite.name = it.name;
		sprite.origin = Vec2::zero;
		if (!it.asesprite.slices.empty() && it.asesprite.slices[0].has_pivot)
		{
			sprite.origin = it.asesprite.slices[0].pivot;
		}

		for (auto& tag : it.asesprite.tags)
		{
			Sprite::Animation anim;
			anim.name = tag.name;
			for (int i = tag.from; i <= tag.to; i++)
			{
				Sprite::Frame frame;
				frame.duration = it.asesprite.frames[i].duration / 1000.0f;
				frame.texture = subtextures[it.pack_index + i];
				anim.frames.push_back(frame);
			}
			sprite.animations.push_back(anim);
		}
		_sprites[it.name] = sprite;
	}
}

const pxl::Sprite* Content::sprite(const string &name)
{
	auto sprite = _sprites.find(name);
	if (sprite == _sprites.end()) return nullptr;
	return &sprite->second;
}

const Tileset* Content::tileset(const string& name)
{
	auto tileset = _tilesets.find(name);
	if (tileset == _tilesets.end()) return nullptr;
	return &tileset->second;
}

void Content::unload()
{
	_sprites.clear();
	_tilesets.clear();
}