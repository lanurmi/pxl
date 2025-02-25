#pragma once

#include <functional>

using namespace std;

namespace pxl
{
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

		bool operator<(const String& str) const;
		bool operator<(const char* str) const;

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

		String& push_back(const String& str);
		String& push_back(const char* str);
		String& push_back(char c);

		String& append(const char* str, int size);
		String& append(const char* str);
		String& append(char c);
		String& append(const String& str);

		char& back();
		const char& back() const;

		bool startsWith(const pxl::String& test);

		static String format(const char* fmt, ...);
		static String fromInt(int num);
		static int toInt(const String& string);

		using value_type = char;
		using size_type = unsigned;

		static unsigned utf8Size(const pxl::String& str, unsigned index);

	private:
		void set(const char* from, unsigned size);
		void set(const char* from);
		char* _data = nullptr;
		unsigned _size = 0;
		unsigned _capasity = 0;
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
