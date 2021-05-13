#pragma once
#include <pxl/utils/sprite.h>
#include <pxl/types.h>

namespace pxl
{
	class ContentPack
	{
	public:
		void addSprite(const string& path);
		void addSpriteDirectory(const string& path);
	private:
		std::vector<string> _sprites;
		std::vector<string> _sprite_directories;
		friend class Content;
	};

	class Content
	{
	public:
		void load(const ContentPack& pack);
		void unload();
		const Sprite* sprite(const string& name);
	private:
		void loadSprites(const ContentPack& pack);
		map<string, Sprite> _sprites;
	};
}