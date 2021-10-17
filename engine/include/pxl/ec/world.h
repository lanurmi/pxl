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
	class Entity;

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
		const T* get(Entity* entity) const;

		template<typename T>
		T* get(Entity* entity);

		void update();

		Action<IComponent*> componentAdded;

	private:
		pxl::Vector<Entity> entities;
		pxl::Vector<Entity*> entitiesToBeDestroyed;

		pxl::Map <i64, pxl::Vector<IComponent*>> componentsByType;
	};


	template<typename T>
	T* World::add(Entity* entity, T&& component)
	{
		assert(entity->get<T>() == nullptr);

		auto type = ComponentInfo<T>::id;

		auto it = componentsByType.find(type);
		if (it == componentsByType.end())
		{
			componentsByType[type] = pxl::Vector<IComponent*>();
			it = componentsByType.find(type);
		}

		auto c = new T(std::move(component));	
		auto back = it->second.empty() ? nullptr : (T*) it->second.data()[it->second.size() - 1];
		if (back)
		{
			back->_next = c;
			c->_prev = back;
		}
		c->_next = nullptr;
		c->_entity = entity;
		it->second.push_back(c);
		entity->_components.push_back(c);

		componentAdded.invoke(c);

		c->awake();

		pxl::log::message(pxl::String::format("Added component %s to entity %hu", ComponentInfo<T>::type, entity->id()) );

		return c;
	}

	template<typename T>
	const T* World::first() const
	{
		auto type = ComponentInfo<T>::id;
		auto it = componentsByType.find(type);
		if (it == componentsByType.end()) return nullptr;

		return (T*)it->second[0];
	}

	template<class T>
	T* World::first()
	{
		auto type = pxl::ComponentInfo<T>::id;
		auto it = componentsByType.find(type);

		if (it == componentsByType.end()) return nullptr;
		if (it->second.empty()) return nullptr;

		return (T*)it->second[0];
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