#pragma once
#include <pxl/types.h>


namespace pxl
{
	class Entity;

	class IUpdateable
	{
	public:
		virtual void update() = 0;
		virtual i16 updateOrder() const { return 0; }
	};
	
	class Batch;
	class IDrawable
	{
	public:
		virtual void draw(Batch &batch) = 0;
		virtual i16 drawOrder() const { return 0; }
	};

	class IDebugDrawable
	{
	public:
		virtual void debugDraw(Batch& batch) = 0;
		virtual i16 drawOrder() const { return 0; }
	};

	class IComponent
	{
	public:
		virtual ~IComponent() {}
		virtual void awake() {}
		virtual void destroy() {};
		virtual u16 typeId() const = 0;
	};

	class IResettable
	{
	public:
		virtual void reset() = 0;
	};

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

	private:
		T* _next = nullptr;
		T* _prev = nullptr;
		Entity* _entity;
		friend class World;
	};

	template<typename T>
	Component<T>::Component() {}

	template<typename T>
	Component<T>::~Component() {}

	template<typename T>
	const Entity* Component<T>::entity() const
	{
		return _entity;
	}

	template<typename T>
	Entity* Component<T>::entity()
	{
		return _entity;
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