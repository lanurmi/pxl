#pragma once
#include "component_interface.h"
#include "entity.h"
#include <pxl/types.h>


namespace pxl
{
	class Entity;


	template<typename T>
	class Component : public IComponent, IResettable
	{
	public:
		Component();
		virtual ~Component();

		u16 typeId() const override;
		const Entity* entity() const;
		Entity* entity();

		T* next();
		const T* next() const;

		T* prev();
		const T* prev() const;

		void reset() override final
		{

			if (_prev != nullptr)
			{
				_prev->_next = _next;
			}
			if (_next != nullptr)
			{
				_next->_prev = _prev;
			}

			_next = nullptr;
			_prev = nullptr;
			_entity = nullptr;
		}

		template<typename C>
		C* get() const;

		template<typename C>
		C* get();

		const World* world() const;
		World* world();

	private:
		T* _next = nullptr;
		T* _prev = nullptr;
		Entity* _entity;
		friend class World;
	};

	template<typename T>
	Component<T>::Component() : _next(nullptr), _prev(nullptr), _entity(nullptr) {}

	template<typename T>
	Component<T>::~Component() {}

	template<typename T>
	const Entity* Component<T>::entity() const
	{
		return _entity;
	}

	template<typename T>
	template<typename C>
	C* Component<T>::get() const
	{
		return _entity->get<C>();
	}

	template<typename T>
	template<typename C>
	C* Component<T>::get()
	{
		return _entity->get<C>();
	}

	template<typename T>
	Entity* Component<T>::entity()
	{
		return _entity;
	}

	template<typename T>
	const World* Component<T>::world() const
	{
		return _entity->world();
	}

	template<typename T>
	World* Component<T>::world()
	{
		return _entity->world();
	}

	template<typename T>
	u16 Component<T>::typeId() const
	{
		return ComponentId::get<T>();
	}

	template<typename T>
	T* Component<T>::next()
	{
		return _next;
	}

	template<typename T>
	const T* Component<T>::next() const
	{
		return _next;
	}

	template<typename T>
	T* Component<T>::prev()
	{
		return _prev;
	}

	template<typename T>
	const T* Component<T>::prev() const
	{
		return _prev;
	}
}