#pragma once

#include <pxl/types.h>
#include <pxl/containers/vector.h>

namespace pxl
{
	using ActionBindId = pxl::i64;

	template<typename... Args>
	class Action
	{
	public:
		Action() : id(0)
		{
		}
		Action& operator+=(Func<Args...> func)
		{
			bind(func);
			return *this;
		}
		void invoke(Args... args) const
		{
			for (auto b : funcs)
			{
				b.func(args...);
			}
		}
		ActionBindId bind(Func<Args...> func)
		{
			funcs.push_back(Binder(id, func));
			return id++;
		}
		void unbind(ActionBindId removeId)
		{
			funcs.erase(std::remove_if(funcs.begin(), funcs.end(), [removeId](const Binder& b)
				{
					return b.id == removeId;
				}), funcs.end());
		}
		void clear()
		{
			funcs.clear();
		}
	private:
		ActionBindId id;
		struct Binder
		{
			Binder(ActionBindId id, Func<Args...> func) : id(id), func(func) {}
			ActionBindId id;
			Func<Args...> func;
		};
		pxl::Vector<Binder> funcs;
	};
}