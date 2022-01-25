#pragma once

#include <functional>
#include <string>

using namespace std;

namespace pxl
{
#ifdef PXL_USE_STL_CONTAINERS
	using String = basic_string<char, char_traits<char>, allocator<char>>;
	namespace string
	{
		unsigned utf8Size(const pxl::String& str, unsigned index);
		String format(const char* fmt, ...);
		bool startsWith(const pxl::String& str, const pxl::String& test);
	}
#else
	class String {
	public:
		String();
		String(const String& str);
		String(String&& str) noexcept;
		String(const char* str);
		String(const char* str, unsigned length);
		String(unsigned length, char c);
		~String();

		char& operator[](unsigned index);
		const char& operator[](unsigned index) const;

		bool operator==(const String& str) const;
		bool operator==(const char* str) const;

		bool operator!=(const String& str) const;
		bool operator!=(const char* str) const;

		bool operator < (const String& str) const;

		String& operator=(const String& str);
		String& operator=(String&& str) noexcept;
		String& operator=(const char* str);

		String& operator+=(const String& str);
		String& operator+=(const char* str);

		String operator+(const String& str);
		String operator+(const char* str);

		operator char* () { return data(); }
		operator const char* () const { return data(); }

		char* data();
		const char* data() const;

		char* c_str();
		const char* c_str() const;



		void resize(unsigned size);
		void resize(unsigned size, char c);
		void reserve(unsigned length);

		void clear();

		bool empty() const;
		unsigned size() const;
		unsigned utf8Size(unsigned index) const;

		String& push_back(const String& str);
		String& push_back(const char* str);
		String& push_back(const char* str, int size);
		String& push_back(char c);

		String& append(const char* str, int size);

		char& back();
		const char& back() const;

		static String format(const char* fmt, ...);
		static String fromInt(int num);

		using value_type = char;

	private:
		void set(const char* from, unsigned size);
		void set(const char* from);
		char* _data = nullptr;
		unsigned _size = 0;
		unsigned _capasity = 0;
		static char s_empty_data[1];
	};

}
#endif
}

#ifndef PXL_USE_STL_CONTAINERS

pxl::String operator+(const char* str0, const pxl::String & str1);
bool operator==(const char* str0, const pxl::String& str1);

namespace std
{
	template <>
	struct hash<pxl::String>
	{
		std::size_t operator()(const pxl::String& key) const
		{
			std::size_t result = 0x811c9dc5;
			auto s = key.size();
			for (unsigned i = 0; i < s; i++)
			{
				result ^= static_cast<std::size_t>(key[i]);
				result *= 0x01000193;
			}
			return result;
		}
	};
}

#endif
