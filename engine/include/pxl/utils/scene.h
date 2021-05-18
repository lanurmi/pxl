#pragma once
#include <pxl/math/calc.h>
#include <pxl/utils/entity.h>
#include <pxl/utils/component.h>
#include <pxl/graphics/batch.h>

namespace pxl
{
	class Batch;
	class Camera;
	class Scene
	{
	public:
		Scene(const string &name);
		virtual ~Scene();
		Entity* createEntity(const pxl::Vec2& position);
		template<class T>
		T* add(Entity* entity, T&& component);
		void destroy(Component* component);
		void destroy(Entity* entity);

		template<class T>
		vector<Component*>& all();

		template<class T>
		T* first();

		virtual void begin();
		virtual void update();
		virtual void draw();
		virtual void end();

		virtual const Camera* camera() { return nullptr; }
	protected:
		const std::vector<IDrawable*> &drawables();
		const std::vector<IDebugDrawable*> &debugDrawables();
		Batch& batch();
	private:
		void clearRemoveSets();
		string _name;
		Batch _batch;
		u16 _current_max_component_type_id;
		vector<Entity*> _entities;
		vector<Component*> _components[s_max_component_types];
		vector<IDrawable*> _drawable_components;
		vector<IDebugDrawable*> _debug_drawable_components;
		vector<IUpdateable*> _updateable_components;
		set<Component*> _remove_components;
		set<Entity*> _remove_entities;
	};

	template<class T>
	T* Scene::add(Entity* entity, T&& component)
	{
		T* c = new T();
		*c = component;
		c->_entity = entity;
		c->_typeId = pxl::Component::findTypeId<T>();
		_current_max_component_type_id = calc::max(_current_max_component_type_id, c->_typeId + 1);
		if (auto drawabla = dynamic_cast<IDrawable*>(c))
		{
			_drawable_components.push_back(drawabla);
		}
		if (auto debugDrawable = dynamic_cast<IDebugDrawable*>(c))
		{
			_debug_drawable_components.push_back(debugDrawable);
		}
		if (auto updateable = dynamic_cast<IUpdateable*>(c))
		{
			_updateable_components.push_back(updateable);
		}
		_components[c->_typeId].push_back((Component*)c);
		entity->_components.push_back((Component*)c);
		pxl::log().message(string_format("component %d added to entity", c->typeId()));
		c->awake();
		return c;
	}

	template<class T>
	T* Scene::first()
	{
		auto typeId = pxl::Component::findTypeId<T>();
		if (_components[typeId].empty())
		{
			return nullptr;
		}
		{
			return dynamic_cast<T*>(_components[typeId].front());
		}
	}

	template<class T>
	vector<Component*>& Scene::all()
	{
		auto typeId = pxl::Component::findTypeId<T>();
		return _components[typeId];
	}
}