#pragma once

#include <pxl/types.h>
#include <pxl/containers/map.h>
#include <pxl/containers/string.h>
#include <pxl/utils/image_packer.h>
#include <pxl/graphics/texture.h>
#include <pxl/assets/aseprite.h>
#include <pxl/graphics/sprite.h>
#include <pxl/graphics/tileset.h>

namespace pxl
{
	class Atlas
	{
	public:
		Atlas();
		~Atlas();
		void addSprite(const String &name, const Aseprite &ase);
		void addTileset(const String& name, const Aseprite& ase, int tilesize);
		void build();
		const Sprite *sprite(const String &name) const;
		const Tileset* tileset(const String& name) const;
	private:
		struct Info
		{
			Info(const String &name, u32 packIndex, const Aseprite &ase);
			Info(const String& name, u32 packIndex, int size, const Aseprite& ase);
			String name;
			u32 pack_index;
			Aseprite aseprite;
			Image image;
			int size;
		};

		Vector<Info> _spriteInfos;
		Vector<Info> _tilesetInfos;
		ImagePacker _packer;
		TextureRef _page;
		u32 _packIndex;
		Map<String, Sprite> _sprites;
		Map<String, Tileset> _tilesets;
	};
}

