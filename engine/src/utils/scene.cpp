#include <pxl/utils/scene.h>
#include <pxl/utils/entity.h>
#include <pxl/graphics/batch.h>

using namespace pxl;

Scene::Scene() : clear_color(pxl::Color::black), _current_max_component_type_id(0)
{

}

Scene::~Scene()
{

}

Entity* Scene::createEntity(const Vec2& position)
{
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
		_remove_entities.insert(entity);
	}
}

void Scene::clearRemoveSets()
{
	// delete components
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
		if (auto updateable = dynamic_cast<IUpdateable*>(it))
		{
			remove_all(_updateable_components, updateable);
		}
		delete it;
	}

	// delete entities
	for (auto it : _remove_entities)
	{
		remove_all(_entities, it);
		delete it;
	}

}

void Scene::end()
{
	for (auto it : _entities)
	{
		destroy(it);
	}
	clearRemoveSets();
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
	batch.end();
}