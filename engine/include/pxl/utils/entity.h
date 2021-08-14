#pragma once

#include <pxl/utils/entity.h>
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
		for (auto it : _components)
		{
			if (auto c = dynamic_cast<T*>(it))
			{
				return c;
			}
		}
		return nullptr;
	}

	template<class T>
	const T* Entity::get() const
	{
		for (auto it : _components)
		{
			if (auto c = dynamic_cast<T*>(it))
			{
				return c;
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


