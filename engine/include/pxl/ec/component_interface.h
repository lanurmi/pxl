#pragma once

#include <pxl/types.h>
#include <algorithm>    // std::sort
#include <pxl/containers/string.h>
#include <pxl/containers/vector.h>

namespace pxl
{
	class Batch;
	class IComponent
	{
	public:
		virtual ~IComponent() {}
		virtual void awake() {}
		virtual void destroy() {};
		virtual u32 id() const = 0;
		virtual pxl::String typeName() const = 0;
		virtual bool enabled() const = 0;

		virtual void update() {}
		virtual void draw(Batch &batch){}

		void setOrder(u16 order) { _order = order; }
		u16 order() const { return _order; }

		void setLayer(u16 layer) { _layer = layer; }
		u16 layer() const { return _layer; }
	private:
		u16 _order = 0;
		u16 _layer = 0;
	};

	class IComponentList
	{
	public:
		virtual void remove(IComponent* component) = 0;
	};

	using ComponentListRef = std::shared_ptr<IComponentList>;

	template <typename T>
	class ComponentList : public IComponentList
	{
	public:
		void remove(IComponent* component) override;
		void add(T* component);
		const pxl::Vector<T*> & all() const;
	private:
		pxl::Vector<T*> components;
	};

	template <typename T>
	void ComponentList<T>::remove(IComponent* component)
	{
		if (auto c = dynamic_cast<T*>(component))
		{
			components.erase(std::remove(components.begin(), components.end(), c), components.end());
		}
	}

	template <typename T>
	void ComponentList<T>::add(T* component)
	{
		components.push_back(component);
	}

	template <typename T>
	const pxl::Vector<T*>& ComponentList<T>::all() const
	{
		return components;
	}


	class IResettable
	{
	public:
		virtual void reset() = 0;
	};

	// Not sure if i like "marker" interfaces
	class IUpdateable
	{
	};


	class IDrawable
	{
	};


	template<typename T>
	class Numbers
	{
	public:
		Numbers() : counter(0)
		{

		}
		static T next()
		{
			return counter++;
		}
	private:
		inline static T counter;
	};

	template<class T>
	class ComponentInfo
	{
	public:
		static constexpr pxl::u16 updateOrder = 0;
		static constexpr pxl::u16 drawOrder = 0;
		static constexpr pxl::u32 id = 0;
		static constexpr const char* type = "";
	};
}

constexpr pxl::u32 constHash(const char* str)
{
	pxl::u32 hash = 0x811c9dc5;
	for (int i = 0; i < 20; i++)
	{
		if (str[i] != '\0')
		{
			hash ^= static_cast<pxl::u32>(str[i]);
			hash *= 0x01000193;
		}
		else
		{
				return hash;
		}
	}
	return hash;
}

#define COMPONENT(_name) \
namespace pxl \
{\
	template <> class ComponentInfo<_name> \
	{\
	public: \
		static constexpr pxl::u32 id = constHash(#_name); \
		static constexpr const char*  type = #_name; \
	}; \
}
