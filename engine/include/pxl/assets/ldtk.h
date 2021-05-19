#pragma once
#include <pxl/types.h>
#include <pxl/math/color.h>
namespace pxl
{
	struct LDTKLevel
	{
		struct EntityInstance
		{
			string _identifier;
			int _grid[2];
			int _pivot[2];
			int width;
			int height;
			int px[2];
		};

		struct GridTile
		{
			int px[2];
			int src[2];
			int f;
			int t;
			//int d; some ldtk internal thing
		};
		struct LayerInstance
		{
			enum class Type
			{
				Entities,
				Tiles
			};
			string _identifier;
			Type _type;
			int _c_wid;
			int _c_hei;
			int _grid_size;
			float _opacity;
			int _px_total_offset_x;
			int _py_total_offset_y;
			std::vector<GridTile> grid_tiles;
			std::vector<EntityInstance> entity_instances;
		};

		struct FieldInstance
		{
			enum class Type
			{
				Bool
			};
			string _identifier;
			Type _type;
			bool bool_value;
			int defUid;
		};

		LDTKLevel();
		LDTKLevel(const string& path);
		void load(const string& path);

		string identifier;
		int uid;
		int world_x;
		int world_y;
		int px_wid;
		int px_hei;

		std::vector<LayerInstance> layer_instances;
		std::vector<FieldInstance> field_instances;
		const LayerInstance* tileLayer(const string& name) const;
		const LayerInstance* entitiesLayer(const string& name) const;

		bool checkBool(const string& name, bool value) const;
	};
}