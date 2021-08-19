#pragma once
#include <pxl/math/calc.h>
#include <pxl/utils/entity.h>
#include <pxl/utils/component.h>
#include <pxl/graphics/batch.h>
#include <pxl/types.h>

#include <new>
#include <initializer_list>

namespace pxl
{
	class Batch;
	class Camera;
	class Scene
	{
	public:
		Scene(const String &name);
		virtual ~Scene();
		Entity* createEntity(const pxl::Vec2& position);
		template<class T>
		T* add(Entity* entity, T&& component);

		template<class T, class ...Args>
		T* emplace(Entity* entity, Args && ...args);

		void destroy(Component* component);
		void destroy(Entity* entity);

		template<class T>
		Vector<Component*>& all();

		template<class T>
		T* first();

		virtual void begin();
		void trueUpdate();
		virtual void update();
		virtual void draw();
		virtual void end();

		const Vector<IDrawable*> &drawables();
		const Vector<IDebugDrawable*> &debugDrawables();
		const Vector<Entity*>& entities();
		Batch& batch();
	private:
		void sortUpdateables();
		void sortDrawables();
		void sortDebugDrawables();
		void clearRemoveSets();
		bool _sort_drawables;
		bool _sort_updateables;
		bool _sort_debug_drawables;
		String _name;
		Batch _batch;
		u16 _current_max_component_type_id;
		Vector<Entity*> _entities;
		Vector<Component*> _components[s_max_component_types];
		Vector<IDrawable*> _drawable_components;
		Vector<IDebugDrawable*> _debug_drawable_components;
		Vector<IUpdateable*> _updateable_components;
		Unordered_set<Component*> _remove_components;
		Unordered_set<Entity*> _remove_entities;
	};

	template<class T>
	T* Scene::add(Entity* entity, T&& component)
	{
		T* c = new T(std::move(component));
		c->_entity = entity;
		c->_typeId = pxl::Component::findTypeId<T>();
		_current_max_component_type_id = calc::max(_current_max_component_type_id, c->_typeId + 1);
		if (auto drawabla = dynamic_cast<IDrawable*>(c))
		{
			_drawable_components.add(drawabla);
			_sort_drawables = true;
		}
		if (auto debugDrawable = dynamic_cast<IDebugDrawable*>(c))
		{
			_debug_drawable_components.add(debugDrawable);
		}
		if (auto updateable = dynamic_cast<IUpdateable*>(c))
		{
			_updateable_components.add(updateable);
			_sort_updateables = true;
		}
		_components[c->_typeId].add((Component*)c);
		entity->_components.add((Component*)c);
		pxl::log::message(pxl::String::format("component %d added to entity", c->typeId()));
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
	Vector<Component*>& Scene::all()
	{
		auto typeId = pxl::Component::findTypeId<T>();
		return _components[typeId];
	}
}