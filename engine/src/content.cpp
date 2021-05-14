#include <pxl/content.h>
#include <pxl/types.h>
#include <pxl/utils/image_packer.h>
#include <pxl/filesystem.h>
#include <pxl/engine.h>
#include <pxl/assets/aseprite.h>
#include <pxl/time.h>

using namespace pxl;

void ContentPack::addSprite(const string& path)
{
	auto applicationPath = pxl::engine().applicationPath();
	_sprites.push_back(path::combine(applicationPath, path));
}

void ContentPack::addSpriteDirectory(const string &path)
{
	auto applicationPath = pxl::engine().applicationPath();
	_sprite_directories.push_back(path::combine(applicationPath, path));
}

void Content::load(const ContentPack& content)
{
	time::Timer timer;
	loadSprites(content);
	auto elapsed = timer.ms();
	log().message(string_format("Content pack loaded in %llums", elapsed));
}

void Content::loadSprites(const ContentPack& pack)
{

	struct Info
	{
		string name;
		u32 pack_index;
		Aseprite asesprite;
	};

	ImagePacker packer(2048, 2048);
	std::vector<Info> spriteinfos;
	std::vector<string> asefiles;
	for (auto& dir : pack._sprite_directories)
	{
		auto files = directory::files(dir, ".ase");
		for (auto& file : files)
		{
			asefiles.push_back(file);
		}
	}
	for (auto file : pack._sprites)
	{
		if (path::extension(file) == ".ase" && file::exists(file))
		{
			asefiles.push_back(file);
		}
	}

	i32 packIndex = 0;
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

	vector<Sprite> sprites;
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

void Content::unload()
{
	_sprites.clear();
}