#include <pxl/utils/scene.h>
#include <pxl/utils/entity.h>
#include <pxl/graphics/batch.h>

using namespace pxl;

Scene::Scene() : _current_max_component_type_id(0)
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

}

void Scene::update()
{
	//Clean up removed components
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
	}
	for (auto it : _updateable_components)
	{
		it->update();
	}
}

void Scene::draw(Batch &batch)
{
	for (auto it : _drawable_components)
	{
		it->draw(batch);
	}
}