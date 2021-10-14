#pragma once

#include <pxl/containers/map.h>
#include <pxl/ec/entity.h>
#include <pxl/ec/component.h>
#include <pxl/math/vec2.h>
#include <pxl/containers/vector.h>
#include <pxl/types.h>

namespace pxl
{
	class Entity;

	constexpr pxl::u16 s_max_entities = 1000;

	class World
	{
	public:
		World();
		~World();

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
	private:
		pxl::Vector<Entity> entities;
		pxl::Vector<Entity*> entitiesToBeDestroyed;

		pxl::Map < u16, pxl::Vector<IComponent*>> componentsByType;
	};


	template<typename T>
	T* World::add(Entity* entity, T&& component)
	{
		assert(entity->get<T>() == nullptr);

		auto type = ComponentId::get<T>();

		auto it = componentsByType.find(type);
		if (it == componentsByType.end())
		{
			componentsByType[type] = pxl::Vector<IComponent*>(32);
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
		it->second.add(c);
		entity->_components.add(c);

		c->awake();
		return c;
	}

	template<typename T>
	const T* World::first() const
	{
		auto type = pxl::ComponentId::get<T>();
		auto it = componentsByType.find(type);
		if (it == componentsByType.end()) return nullptr;

		return (T*)it->second[0];
	}

	template<typename T>
	T* World::first()
	{
		auto type = pxl::ComponentId::get<T>();
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