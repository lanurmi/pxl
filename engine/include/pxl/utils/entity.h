#pragma once

#include <pxl/utils/component.h>
#include <pxl/math/vec2.h>
#include <pxl/types.h>
#include <assert.h>

namespace pxl
{
	constexpr int s_max_component_types = 256;

	class Scene;
	class Component;

	class Entity
	{
	public:
		Entity() : _scene(nullptr) {};

		Vec2 position;

		template<class T>
		T* get();

		template<class T>
		const T* get() const;

		template<class T>
		T* add(T&& component);

		template<class T, class ...Args>
		T* emplace(Args && ...args);

		Scene* scene();

	private:
		Scene* _scene;
		Vector<Component*> _components;

		friend class Scene;
	};


	// Entity
	template<class T>
	T* Entity::get()
	{
		int type = ComponentId::get<T>();
		for (auto it : _components)
		{
			if (it->typeId() == type)
			{
				return dynamic_cast<T*>(it);
			}
		}
		return nullptr;
	}

	template<class T>
	const T* Entity::get() const
	{
		int type = ComponentId::get<T>();
		for (auto it : _components)
		{
			if (it->typeId() == type)
			{
				return dynamic_cast<T*>(it);
			}
		}
		return nullptr;
	}

	template<class T>
	T* Entity::add(T&& component)
	{
		return _scene->add(this, std::forward<T>(component));
	}
}


