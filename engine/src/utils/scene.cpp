#include <pxl/engine.h>
#include <pxl/utils/scene.h>
#include <pxl/utils/entity.h>
#include <pxl/utils/components/camera.h>
#include <pxl/graphics/batch.h>
#include <pxl/math/calc.h>

using namespace pxl;

Scene::Scene(const String &name) : _current_max_component_type_id(0), _name(name)
{
}

Scene::~Scene()
{

}

Entity* Scene::createEntity(const Vec2& position)
{
	pxl::log::message(String::format("Entity created at: (%.2f, %.2f)", position.x, position.y));
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
		pxl::log::message(String::format("component %d marked for destruction", component->typeId()));
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
		pxl::log::message("entity marked for destruction");
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

		remove_all(_components[it->typeId()], it);
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
		pxl::log::message(pxl::String::format("deleted %d components", deleted_components));
		_remove_components.clear();
	}

	if (deleted_entities > 0)
	{
		pxl::log::message(pxl::String::format("deleted %d entities", deleted_entities));
		_remove_entities.clear();
	}
}

void Scene::begin()
{
	pxl::log::message("Begin scene: " + _name);
}

void Scene::end()
{
	for (auto it : _entities)
	{
		destroy(it);
	}
	clearRemoveSets();
	pxl::log::message("End scene: " + _name);
}

void Scene::trueUpdate() {
	update();
	for (auto it : _updateable_components) 	{
		it->update();
	}

	clearRemoveSets();

	if (_sort_updateables) {
		sortUpdateables();
		_sort_updateables = false;
	}
	if (_sort_drawables) {
		sortDrawables();
		_sort_drawables = false;
	}
	if (_sort_debug_drawables) {
		sortDebugDrawables();
		_sort_debug_drawables = false;
	}
}

void Scene::update()
{
}

void Scene::draw()
{
	_batch.begin(nullptr, pxl::Color::black);
	for (auto it : _drawable_components)
	{
		it->draw(_batch);
	}
	_batch.end();
}

const std::vector<IDrawable*> &Scene::drawables()
{
	return _drawable_components;
}

const std::vector<IDebugDrawable*> &Scene::debugDrawables()
{
	return _debug_drawable_components;
}

const std::vector<Entity*>& Scene::entities()
{
	return _entities;
}

Batch& Scene::batch()
{
	return _batch;
}

void Scene::sortUpdateables()
{
	std::sort(_updateable_components.begin(), _updateable_components.end(), [](IUpdateable* up1, IUpdateable* up2) -> bool {
		return up1->updateOrder() < up2->updateOrder();
	});
}

void Scene::sortDrawables()
{
	std::sort(_drawable_components.begin(), _drawable_components.end(), [](IDrawable* up1, IDrawable* up2) -> bool {
		return up1->drawOrder() < up2->drawOrder();
	});
}

void Scene::sortDebugDrawables()
{
	std::sort(_debug_drawable_components.begin(), _debug_drawable_components.end(), [](IDebugDrawable* up1, IDebugDrawable* up2) -> bool {
		return up1->drawOrder() < up2->drawOrder();
	});
}
