#pragma once
#include "component_interface.h"
#include "entity.h"
#include <pxl/types.h>


namespace pxl
{
	class Entity;


	template<typename T>
	class Component : public IComponent
	{
	public:
		Component();
		virtual ~Component();
		u32 id() const override;
		pxl::String typeName() const override;
		bool enabled() const override;
		const Entity* entity() const;
		Entity* entity();

		template<typename C>
		C* get() const;

		template<typename C>
		C* get();

		const World* world() const;
		World* world();

	private:
		Entity* _entity;
		friend class World;
	};

	template<typename T>
	Component<T>::Component() : _entity(nullptr) {}

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

	template<class T>
	u32 Component<T>::id() const
	{
		return ComponentInfo<T>::id;
	}

	template<typename T>
	pxl::String Component<T>::typeName() const
	{
		return pxl::String(ComponentInfo<T>::type);
	}

	template<typename T>
	bool Component<T>::enabled() const
	{
		return entity()->enabled;
	}
}

