#pragma once

#include <functional>

namespace pxl
{
	class String {
	public:
		String();
		String(const String& str);
		String(String&& str) noexcept;
		String(const char* str);
		String(const char* str, int length);
		~String();

		char& operator[](int index);
		const char& operator[](int index) const;

		bool operator==(const String& str) const;
		bool operator==(const char* str) const;

		bool operator!=(const String& str) const;
		bool operator!=(const char* str) const;

		bool operator < (const String & str) const;

		String& operator=(const String& str);
		String& operator=(String&& str) noexcept;
		String& operator=(const char* str);

		String& operator+=(const String& str);
		String& operator+=(const char* str);

		String operator+(const String& str);
		String operator+(const char* str);

		operator char* () { return cstr(); }
		operator const char* () const { return cstr(); }

		char* cstr();
		const char* cstr() const;

		void resize(int size);
		void reserve(int length);

		bool empty() const;
		int size() const;
		int utf8Size(int index) const;

		String& append(const String& str);
		String& append(const char* str);
		String& append(const char* str, int size);
		String& append(char c);

		static String format(const char *fmt, ...);
		static String fromInt(int num);
	private:
		void set(const char* from, int size);
		void set(const char* from);
		char* _data;
		int _size;
		int _capasity;
		static char s_empty_data[1];
	};

}

pxl::String operator+(const char* str0, const pxl::String & str1);
bool operator==(const char* str0, const pxl::String& str1);

namespace std
{
	template <>
	struct hash<pxl::String>
	{
		std::size_t operator()(const pxl::String& key) const
		{
			std::size_t result = 2166136261U;
			auto s = key.size();
			for (int i = 0; i < s; i++)
			{
				result ^= static_cast<std::size_t>(key[i]);
				result *= 16777619U;
			}
			return result;
		}
	};
}