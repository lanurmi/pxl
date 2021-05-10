#include <pxl/engine.h>
#include <pxl/utils/scene.h>
#include <pxl/utils/entity.h>
#include <pxl/graphics/batch.h>

using namespace pxl;

Scene::Scene(const string &name) : clear_color(pxl::Color::black), _current_max_component_type_id(0), _name(name)
{

}

Scene::~Scene()
{

}

Entity* Scene::createEntity(const Vec2& position)
{
	pxl::log().message(string_format("Entity created at: (%.2f, %.2f)", position.x, position.y));
	auto entity = new Entity();
	_entities.push_back(entity);
	entity->position = position;
	entity->_scene = this;
	return entity;
}

void Scene::destroy(Component* component)
{
	if (component && component->_entity && component->_entity->scene() == this)
	{
		component->destroyed();
		_remove_components.insert(component);
		pxl::log().message(string_format("component %d marked for destruction", component->typeId()));
	}
}

void Scene::destroy(Entity* entity)
{
	if (entity != nullptr && entity->scene() == this)
	{
		for (auto it : entity->_components)
		{
			destroy(it);
		}
		pxl::log().message("entity marked for destruction");
		_remove_entities.insert(entity);
	}
}

void Scene::clearRemoveSets()
{
	// delete components
	int deleted_components = 0;
	for (auto it : _remove_components)
	{
		// Remove from entity
		auto entity = it->_entity;
		auto& components = it->_entity->_components;
		remove_all(components, it);

		// remove from lists
		if (auto drawable = dynamic_cast<IDrawable*>(it))
		{
			remove_all(_drawable_components, drawable);
		}
		if (auto debugDrawable = dynamic_cast<IDebugDrawable*>(it))
		{
			remove_all(_debug_drawable_components, debugDrawable);
		}
		if (auto updateable = dynamic_cast<IUpdateable*>(it))
		{
			remove_all(_updateable_components, updateable);
		}
		delete it;
		deleted_components++;
	}

	// delete entities
	int deleted_entities = 0;
	for (auto it : _remove_entities)
	{
		remove_all(_entities, it);
		delete it;
		deleted_entities++;
	}

	if (deleted_components > 0)
	{
		pxl::log().message(string_format("deleted %d components", deleted_components));
	}

	if (deleted_entities > 0)
	{
		pxl::log().message(string_format("deleted %d entities", deleted_entities));
	}
}

void Scene::begin()
{
	pxl::log().message("Begin scene: " + _name);
}

void Scene::end()
{
	for (auto it : _entities)
	{
		destroy(it);
	}
	clearRemoveSets();
	pxl::log().message("End scene: " + _name);
}

void Scene::update()
{
	clearRemoveSets();

	for (auto it : _updateable_components)
	{
		it->update();
	}
}

void Scene::draw()
{
	batch.begin(nullptr, clear_color);
	for (auto it : _drawable_components)
	{
		it->draw(batch);
	}
	for (auto it : _debug_drawable_components)
	{
		it->debugDraw(batch);
	}
	batch.end();
}