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
		virtual u16 typeId() const = 0;
		virtual u16 order() const = 0;
		virtual void update() {}
		virtual void draw(Batch& batch) {}
		virtual void debugDraw(Batch& batch) {}
	};

	class IResettable
	{
	public:
		virtual void reset() = 0;
	};

	class Updateables
	{
	public:
		Updateables() {
			sort = false;
		}
		static void Update()
		{
			if (sort)
			{
				std::sort(components.begin(), components.end(), [](IComponent* c0, IComponent* c1) -> bool
				{
						return c0->order() < c1->order();
				});
				sort = false;
			}
			for (auto it : components)
			{
				it->update();
			}
		}
	private:
		static void reg(IComponent* component)
		{
			components.push_back(component);
			sort = true;
		}
		static void unreg(IComponent* component)
		{
#ifdef PXL_USE_STL_CONTAINERS
			components.erase(std::remove(components.begin(), components.end(), component), components.end());
#else
			components.erase(component);
#endif
		}
		friend class IUpdateable;
		static pxl::Vector<IComponent*> components;
		static bool sort;
	};

	class IUpdateable
	{
	public:
		IUpdateable()
		{
			Updateables::reg((IComponent*)this);
		}
		virtual ~IUpdateable()
		{
			Updateables::unreg((IComponent*)this);
		}
	private:
	};


	class Drawables
	{
	public:
		Drawables()
		{

		}
		static void debugDraw(bool enabled)
		{
			_debugDraw = enabled;
		}
		static void draw(Batch &batch)
		{
			if (sort)
			{
				std::sort(components.begin(), components.end(), [](IComponent* c0, IComponent* c1) -> bool
					{
						return c0->order() < c1->order();
					});
				sort = false;
			}

			for (auto it : components)
			{
				it->draw(batch);
			}

			if (_debugDraw)
			{
				for (auto it : components)
				{
					it->debugDraw(batch);
				}
			}
		}
	private:
		static void reg(IComponent* component)
		{
			components.push_back(component);
			sort = true;
		}
		static void unreg(IComponent* component)
		{
#ifdef PXL_USE_STL_CONTAINERS
			components.erase(std::remove(components.begin(), components.end(), component), components.end());
#else
			components.erase(component);
#endif
		}
		friend class IDrawable;
		static pxl::Vector<IComponent*> components;
		static bool sort;
		static bool _debugDraw;
	};


	class IDrawable
	{
	public:
		IDrawable()
		{
			Drawables::reg((IComponent*)this);
		}
		virtual ~IDrawable()
		{
			Drawables::unreg((IComponent*)this);
		}
	};


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



}