#pragma once

#include <pxl/ec/component.h>
#include <pxl/math/vec2.h>
#include <pxl/types.h>
#include <assert.h>

namespace pxl
{

	class World;

	constexpr u16 s_unused_entity = 0;

	class Transform {
	public:
		Vec2 position = Vec2::zero;
		int facing = 1;
	};

	class Entity
	{
	public:
		Entity() : _id(s_unused_entity), _world(nullptr) {};

		Transform transform;

		u16 id() const;

		template<class T>
		T* get();

		template<class T>
		const T* get() const;

		template<class T>
		T* add(T&& component);

	private:
		pxl::Vector<pxl::IComponent*> _components;
		u16 _id;
		World* _world;
		friend class World;
	};


	// Entity
	template<class T>
	T* Entity::get()
	{
		auto type = ComponentId::get<T>();
		for (auto it : _components)
		{
			if (it->typeId() == type) {
				return  (T*)it;
			}
		}
		return nullptr;
	}

	template<class T>
	const T* Entity::get() const
	{
		auto type = ComponentId::get<T>();
		for (auto it : _components)
		{
			if (it->typeId() == type) {
				return  (T*)it;
			}
		}
		return nullptr;
	}

	template<class T>
	T* Entity::add(T&& component)
	{
		return _world->add<T>(this, std::forward<T>(component));
	}
}


