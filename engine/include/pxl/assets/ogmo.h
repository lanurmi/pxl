#pragma once
#include <pxl/types.h>
#include <pxl/containers/string.h>
#include <pxl/containers/map.h>
#include <pxl/math/color.h>

namespace pxl
{
	struct Ogmo
	{
		struct Entity
		{
			String name;
			int id;
			String _eid;
			int x;
			int y;
			int origin_x;
			int origin_y;
			Map<String, String> string_text_enum_color_values;
			Map<String, bool> bool_values;
			Map<String, int> int_values;
			Map<String, float> float_values;

		};
		struct TileLayer
		{
			String name;
			String _eid;
			int offset_x;
			int offset_y;
			int grid_cell_width;
			int grid_cell_height;
			int grid_cells_x;
			int grid_cells_y;
			String tileset;
			Vector<int> data;
			int export_mode;
			int array_mode;
		};
		struct EntityLayer
		{
			String name;
			String _eid;
			int offset_x;
			int offset_y;
			int grid_cell_width;
			int grid_cell_height;
			int grid_cells_x;
			int grid_cells_y;
			Vector<Entity> entities;
		};

		

		Ogmo();
		Ogmo(const String& path);
		void load(const String& path);
		bool valid() const { return !ogmo_version.empty(); }
		bool checkStringValue(const String& key, const String& value);
		const TileLayer* tileLayer(const String& name) const;
		const EntityLayer* entityLayer(const String& name) const;
		String ogmo_version;
		int width;
		int height;
		int offset_x;
		int offset_y;
		std::vector<TileLayer> tile_layers;
		std::vector<EntityLayer> entity_layers;
		Map<String, String> string_text_enum_color_values;
		Map<String, bool> bool_values;
		Map<String, int> int_values;
		Map<String, float> float_values;
	};
}