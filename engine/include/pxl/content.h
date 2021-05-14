#pragma once
#include <pxl/utils/sprite.h>
#include <pxl/utils/tileset.h>
#include <pxl/types.h>

namespace pxl
{
	class ContentPack
	{
	public:
		void addSprite(const string& path);
		void addSpriteDirectory(const string& path);
		void addTileset(const string& path, int size);
		void addTilesetDirectory(const string& path, int size);
	private:
		struct TilesetInfo
		{
			string path;
			int tilesize;
		};
		std::vector<string> _sprites;
		std::vector<TilesetInfo> _tilesets;
		friend class Content;
	};

	class Content
	{
	public:
		void load(const ContentPack& pack);
		void unload();
		const Sprite* sprite(const string& name);
		const Tileset* tileset(const string& name);
	private:
		void loadSprites(const ContentPack& pack);
		void loadTilesets(const ContentPack& pack);
		map<string, Sprite> _sprites;
		map<string, Tileset> _tilesets;
	};
}