#pragma once
#include <pxl/types.h>

namespace pxl
{
	class ComponentId {
	public:
		template<class T>
		static u16 get() {
			static u16 id = idCounter++;
			return id;
		}
		static u16 max()
		{
			return idCounter;
		}
	private:
		static u16 idCounter;
	};

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
		virtual void draw(Batch& batch) = 0;
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
}