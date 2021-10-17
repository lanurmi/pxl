#pragma once
#include <pxl/types.h>
#include <algorithm>    // std::sort
#include <pxl/containers/string.h>
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
		virtual u16 order() const = 0;
		virtual u16 layer() const = 0;
		virtual bool enabled() const = 0;

		virtual void update() {}
		virtual void draw(Batch &batch){}
	};

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

#define COMPONENT_UPDATE_DRAW(_name, _updateOrder, _drawOrder) \
namespace pxl \
{\
	template <> class ComponentInfo<_name> \
	{\
	public: \
		static constexpr pxl::u16 updateOrder = _updateOrder; \
		static constexpr pxl::u16 drawOrder = _drawOrder; \
		static constexpr pxl::u32 id = constHash(#_name); \
		static constexpr const char*  type = #_name; \
	}; \
}


#define COMPONENT_UPDATE(_name, _update) \
	COMPONENT_UPDATE_DRAW(_name, _update, 0)

#define COMPONENT_DRAW(_name, _draw) \
	COMPONENT_UPDATE_DRAW(_name, 0, _draw)

#define COMPONENT(_name) \
	COMPONENT_UPDATE_DRAW(_name, 0, 0)

