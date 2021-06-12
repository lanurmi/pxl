#include <pxl/assets/ogmo.h>
#include <pxl/utils/filestream.h>
#include <pxl/3rdparty/json.hpp>

using namespace pxl;


Ogmo::Ogmo()
{

}

Ogmo::Ogmo(const string &path)
{
	load(path);
}

static void parseValues(std::map<string,string>& dstStrings,
	std::map<string,int>& dstInt,
	std::map<string,float>& dstFloats,
	std::map<string,bool>& dstBools,
	const nlohmann::json& valuesJson)
{
	for (auto itv = valuesJson.begin(); itv != valuesJson.end(); ++itv)
	{
		if (itv->is_string())
		{
			dstStrings[itv.key()] = itv.value();
		}
		else if (itv->is_number_float())
		{
			dstFloats[itv.key()] = itv.value();
		}
		else if (itv->is_number_integer())
		{
			dstInt[itv.key()] = itv.value();
		}
		else if (itv->is_boolean())
		{
			dstBools[itv.key()] = itv.value();
		}
	}
}

void Ogmo::load(const string& path)
{
	FileStream file(path, file::FileMode::Read);
	string data = file.all();
	nlohmann::json ogmoJson = nlohmann::json::parse(data);
	
	ogmo_version = ogmoJson["ogmoVersion"].get<string>();
	width = ogmoJson["width"].get<int>();
	height = ogmoJson["height"].get<int>();
	offset_x = ogmoJson["offsetX"].get<int>();
	offset_y = ogmoJson["offsetY"].get<int>();

	if (ogmoJson.contains("values"))
	{
		parseValues(string_text_enum_color_values, int_values, float_values, bool_values, ogmoJson["values"]);
	}
	nlohmann::json ogmoLayers = ogmoJson["layers"];
	assert(ogmoLayers.is_array());
	for (auto &it : ogmoLayers)
	{
		if (it.contains("entities"))
		{
			EntityLayer layer;
			layer.name = it["name"].get<string>();
			layer._eid = it["_eid"].get<string>();
			layer.offset_x = it["offsetX"].get<int>();
			layer.offset_y = it["offsetY"].get<int>();
			layer.grid_cell_width = it["gridCellWidth"].get<int>();
			layer.grid_cell_height = it["gridCellHeight"].get<int>();
			layer.grid_cells_x = it["gridCellsX"].get<int>();
			layer.grid_cells_y = it["gridCellsY"].get<int>();
			auto entities = it["entities"];
			for (auto& e : entities)
			{
				Entity entity;
				entity.name = e["name"].get<string>();
				entity.id = e["id"].get<int>();
				entity._eid = e["_eid"].get<string>();
				entity.x = e["x"].get<int>();
				entity.y = e["y"].get<int>();
				entity.origin_x = e["originX"].get<int>();
				entity.origin_y = e["originY"].get<int>();

				if (e.contains("values"))
				{
					parseValues(entity.string_text_enum_color_values, entity.int_values, entity.float_values, entity.bool_values, e["values"]);
				}
			}
		}
		else if(it.contains("tileset"))
		{
			TileLayer layer;
			layer.name = it["name"].get<string>();
			layer._eid = it["_eid"].get<string>();
			layer.offset_x = it["offsetX"].get<int>();
			layer.offset_y = it["offsetY"].get<int>();
			layer.grid_cell_width = it["gridCellWidth"].get<int>();
			layer.grid_cell_height = it["gridCellHeight"].get<int>();
			layer.grid_cells_x = it["gridCellsX"].get<int>();
			layer.grid_cells_y = it["gridCellsY"].get<int>();
			layer.tileset = it["tileset"].get<string>();
			auto data = it["data"];
			for (auto& d : data)
			{
				layer.data.push_back(d.get<int>());
			}
			layer.export_mode = it["exportMode"].get<int>();
			layer.array_mode = it["arrayMode"].get<int>();
			tile_layers.emplace_back(layer);
		}
	}
}

bool Ogmo::checkStringValue(const string& key, const string& value)
{
	auto it = string_text_enum_color_values.find(key);
	if (it == string_text_enum_color_values.end()) return false;
	
	return it->second == value;
}

const Ogmo::TileLayer* Ogmo::tileLayer(const string& name) const
{
	for (int i = 0; i < tile_layers.size(); i++)
	{
		if (tile_layers[i].name == name)
		{
			return &tile_layers[i];
		}
	}
	return nullptr;
}

const Ogmo::EntityLayer* Ogmo::entityLayer(const string& name) const
{
	for (int i = 0; i < entity_layers.size(); i++)
	{
		if (entity_layers[i].name == name)
		{
			return &entity_layers[i];
		}
	}
	return nullptr;
}