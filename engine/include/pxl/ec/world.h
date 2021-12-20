#pragma once

#include <pxl/containers/map.h>
#include <pxl/ec/entity.h>
#include <pxl/ec/component.h>
#include <pxl/math/vec2.h>
#include <pxl/containers/vector.h>
#include <pxl/types.h>
#include <pxl/log.h>
#include <pxl/utils/action.h>

namespace pxl
{
	class World
	{
	public:
		World();
		~World();

		void awake(int bufferEntities = 1000);

		Entity* entity(const Vec2& position);
		void destroy(Entity* entity);

		template<typename T>
		T* add(Entity* entity, T&& component);

		template<typename T>
		const T* first() const;

		template<typename T>
		T* first();

		template<typename T>
		const pxl::Vector<T*>& all() const;

		template<typename T>
		const T* get(Entity* entity) const;

		template<typename T>
		T* get(Entity* entity);

		void update();

		Action<IComponent*> componentAdded;
		Action<IComponent*> componentDestroyed;

	private:
		pxl::Vector<Entity> entities;
		pxl::Vector<Entity*> entitiesToBeDestroyed;
		pxl::Map<i64, ComponentListRef> componentListsByType;
	};


	template<typename T>
	T* World::add(Entity* entity, T&& component)
	{
		assert(entity->get<T>() == nullptr);

		auto type = ComponentInfo<T>::id;

		auto itt = componentListsByType.find(type);
		if (itt == componentListsByType.end())
		{
			componentListsByType[type] = ComponentListRef(new ComponentList<T>());
			itt = componentListsByType.find(type);
		}

		auto c = new T(std::move(component));	

		c->_entity = entity;

		auto list = std::dynamic_pointer_cast<ComponentList<T>>(itt->second);
		list->add(c);

		entity->_components.push_back(c);

		componentAdded.invoke(c);

		c->awake();

		pxl::log::message(pxl::String::format("Added component %s to entity %hu", ComponentInfo<T>::type, entity->id()) );

		return c;
	}

	template<typename T>
	const T* World::first() const
	{
		auto a = all<T>();
		return a[0];
	}

	template<typename T>
	T* World::first()
	{
		auto a = all<T>();
		return a[0];
	}

	template<typename T>
	const pxl::Vector<T*>& World::all() const
	{
		auto type = pxl::ComponentInfo<T>::id;
		auto it = componentListsByType.find(type);
		auto list = std::dynamic_pointer_cast<ComponentList<T>>(it->second);
		return list->all();
	}

	template<typename T>
	const T* World::get(Entity *entity) const
	{
		return nullptr;
	}

	template<typename T>
	T* World::get(Entity* entity)
	{
		return nullptr;
	}
}

