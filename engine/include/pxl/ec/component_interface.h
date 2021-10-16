#pragma once
#include <pxl/types.h>
#include <algorithm>    // std::sort

namespace pxl
{
	class Batch;
	class IComponent
	{
	public:
		virtual ~IComponent() {}
		virtual void awake() {}
		virtual void destroy() {};
		virtual i64 id() const = 0;
		virtual u16 order() const = 0;
	};

	class IResettable
	{
	public:
		virtual void reset() = 0;
	};

	class IUpdateable
	{
	public:
		virtual void update() = 0;
	private:
	};


	class IDrawable
	{
	public:
		virtual void draw(Batch &batch) = 0;
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
		static constexpr pxl::i64 id = 0;
	};
}

#define HASH(_num) \
	if (str[_num] != '\0') \
	{ \
		hash ^= static_cast<pxl::i64>(str[_num]); \
		hash *= 0x01000193; \
	} \
	else \
	{ \
		return hash; \
	}

constexpr pxl::i64 constHash(const char* str)
{
	pxl::i64 hash = 0x811c9dc5;
	HASH(0)
	HASH(1)
	HASH(2)
	HASH(3)
	HASH(4)
	HASH(5)
	HASH(6)
	HASH(7)
	HASH(8)
	HASH(9)
	HASH(10)
	HASH(11)
	HASH(12)
	HASH(13)
	HASH(14)
	HASH(15)
	HASH(16)
	HASH(17)
	HASH(18)
	HASH(19)
	HASH(20)
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
		static constexpr pxl::i64 id = constHash(#_name); \
	}; \
}


#define COMPONENT_UPDATE(_name, _update) \
	COMPONENT_UPDATE_DRAW(_name, _update, 0)

#define COMPONENT_DRAW(_name, _draw) \
	COMPONENT_UPDATE_DRAW(_name, 0, _draw)

#define COMPONENT(_name) \
	COMPONENT_UPDATE_DRAW(_name, 0, 0)

