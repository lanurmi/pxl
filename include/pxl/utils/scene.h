#pragma once
#include <pxl/math/calc.h>
#include <pxl/utils/entity.h>
#include <pxl/utils/component.h>

namespace pxl
{
	class Batch;
	class Scene
	{
	public:
		Scene();

		Entity* createEntity(const pxl::Vec2& position);
		template<class T>
		T* add(Entity* entity, T&& component);
		void update();
		void draw(Batch& batch);
		void destroy(Component* component);
		void destroy(Entity* entity);
		
	private:
		u16 _current_max_component_type_id;
		vector<Entity*> _entities;
		vector<Component*> _components[s_max_component_types];
		vector<IDrawable*> _drawable_components;
		vector<IUpdateable*> _updateable_components;
		set<Component*> _remove_components;
	};

	template<class T>
	T* Scene::add(Entity* entity, T&& component)
	{
		auto c = new T();
		*c = component;
		c->_entity = entity;
		c->_type = pxl::Component::findTypeId<T>();
		_current_max_component_type_id = calc::max(_current_max_component_type_id, c->_type + 1);
		if (auto renderable = dynamic_cast<IDrawable*>(c))
		{
			_drawable_components.push_back(renderable);
		}
		if (auto updateable = dynamic_cast<IUpdateable*>(c))
		{
			_updateable_components.push_back(updateable);
		}
		return c;
	}
}