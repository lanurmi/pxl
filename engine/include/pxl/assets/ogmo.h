#pragma once
#include <pxl/types.h>
#include <pxl/math/color.h>

namespace pxl
{
	struct Ogmo
	{
		struct Entity
		{
			string name;
			int id;
			string _eid;
			int x;
			int y;
			int origin_x;
			int origin_y;
			map<string, string> string_text_enum_color_values;
			map<string, bool> bool_values;
			map<string, int> int_values;
			map<string, float> float_values;

		};
		struct TileLayer
		{
			string name;
			string _eid;
			int offset_x;
			int offset_y;
			int grid_cell_width;
			int grid_cell_height;
			int grid_cells_x;
			int grid_cells_y;
			string tileset;
			vector<int> data;
			int export_mode;
			int array_mode;
		};
		struct EntityLayer
		{
			string name;
			string _eid;
			int offset_x;
			int offset_y;
			int grid_cell_width;
			int grid_cell_height;
			int grid_cells_x;
			int grid_cells_y;
			vector<Entity> entities;
		};

		

		Ogmo();
		Ogmo(const string& path);
		void load(const string& path);
		bool valid() const { return !ogmo_version.empty(); }
		const TileLayer* tileLayer(const string& name) const;
		const EntityLayer* entityLayer(const string& name) const;
		string ogmo_version;
		int width;
		int height;
		int offset_x;
		int offset_y;
		std::vector<TileLayer> tile_layers;
		std::vector<EntityLayer> entity_layers;
		map<string, string> string_text_enum_color_values;
		map<string, bool> bool_values;
		map<string, int> int_values;
		map<string, float> float_values;
	};
}