#pragma once

#include <pxl/types.h>

namespace pxl
{
	struct Color
	{
		Color();
		Color(u8 r, u8 g, u8 b);
		Color(u8 r, u8 g, u8 b, u8 a);
		bool operator==(const Color& color) const;
		bool operator!=(const Color& color) const;
		void preMultiply();
		u8 r;
		u8 g;
		u8 b;
		u8 a;

		static const Color transparent;
		static const Color white;
		static const Color black;
		static const Color red;
		static const Color green;
		static const Color blue;
		static const Color yellow;
		static const Color purple;
		static const Color teal;
	};
}