#pragma once

#include <pxl/ec/component_interface.h>
#include <pxl/ec/transform.h>
#include <pxl/math/vec2.h>
#include <pxl/types.h>
#include <assert.h>

namespace pxl
{

	class World;

	constexpr u16 s_unused_entity = 0;



	class Entity : IResettable
	{
	public:
		Entity() : _id(s_unused_entity), _world(nullptr) {};

		Transform transform;
		bool enabled = true;
		Entity* parent = nullptr;

		Mat3x2 matrix() const;
		Mat3x2 matrix(const pxl::Vec2 &originTrans) const;

		Vec2 position() const;
		Vec2 scale() const;

		u16 id() const;

		template<class T>
		T* get();

		template<class T>
		const T* get() const;

		template<class T>
		T* add(T&& component);


		World* world();
		const World* world() const;

		void reset() override
		{
			transform = Transform();
			_id = s_unused_entity;
			_components.clear();
			_world = nullptr;
			parent = nullptr;
		}

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
		auto type = ComponentInfo<T>::id;
		for (auto it : _components)
		{
			if (it->id() == type) {
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


