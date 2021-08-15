#include <pxl/assets/ogmo.h>
#include <pxl/utils/filestream.h>
#include <pxl/3rdparty/json.hpp>

using namespace pxl;


Ogmo::Ogmo()
{

}

Ogmo::Ogmo(const String&path)
{
	load(path);
}

static void parseValues(Map<String, String>& dstStrings,
	Map<String,int>& dstInt,
	Map<String,float>& dstFloats,
	Map<String,bool>& dstBools,
	const nlohmann::json& valuesJson)
{
	for (auto itv = valuesJson.begin(); itv != valuesJson.end(); ++itv)
	{
		pxl::String key(itv.key().c_str());
		if (itv->is_string())
		{
			dstStrings[key] = itv.value().get<std::string>().c_str();
		}
		else if (itv->is_number_float())
		{
			dstFloats[key] = itv.value();
		}
		else if (itv->is_number_integer())
		{
			dstInt[key] = itv.value();
		}
		else if (itv->is_boolean())
		{
			dstBools[key] = itv.value();
		}
	}
}

void Ogmo::load(const String& path)
{
	FileStream file(path, file::FileMode::Read);
	String data = file.all();
	nlohmann::json ogmoJson = nlohmann::json::parse(data.cstr());
	
	ogmo_version = ogmoJson["ogmoVersion"].get<std::string>().c_str();
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
			layer.name = it["name"].get<std::string>().c_str();
			layer._eid = it["_eid"].get<std::string>().c_str();
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
				entity.name = e["name"].get<std::string>().c_str();;
				entity.id = e["id"].get<int>();
				entity._eid = e["_eid"].get<std::string>().c_str();;
				entity.x = e["x"].get<int>();
				entity.y = e["y"].get<int>();
				entity.origin_x = e["originX"].get<int>();
				entity.origin_y = e["originY"].get<int>();

				if (e.contains("values"))
				{
					parseValues(entity.string_text_enum_color_values, entity.int_values, entity.float_values, entity.bool_values, e["values"]);
				}
				layer.entities.add(entity);
			}
			entity_layers.emplace_back(layer);
		}
		else if(it.contains("tileset"))
		{
			TileLayer layer;
			layer.name = it["name"].get<std::string>().c_str();
			layer._eid = it["_eid"].get<std::string>().c_str();
			layer.offset_x = it["offsetX"].get<int>();
			layer.offset_y = it["offsetY"].get<int>();
			layer.grid_cell_width = it["gridCellWidth"].get<int>();
			layer.grid_cell_height = it["gridCellHeight"].get<int>();
			layer.grid_cells_x = it["gridCellsX"].get<int>();
			layer.grid_cells_y = it["gridCellsY"].get<int>();
			layer.tileset = it["tileset"].get<std::string>().c_str();
			auto data = it["data"];
			for (auto& d : data)
			{
				layer.data.add(d.get<int>());
			}
			layer.export_mode = it["exportMode"].get<int>();
			layer.array_mode = it["arrayMode"].get<int>();
			tile_layers.emplace_back(layer);
		}
	}
}

bool Ogmo::checkStringValue(const String& key, const String& value)
{
	auto it = string_text_enum_color_values.find(key);
	if (it == string_text_enum_color_values.end()) return false;
	
	return it->second == value;
}

const Ogmo::TileLayer* Ogmo::tileLayer(const String& name) const
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

const Ogmo::EntityLayer* Ogmo::entityLayer(const String& name) const
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