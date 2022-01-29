#include <pxl/containers/string.h>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <cmath>

using namespace pxl;

char String::s_empty_data[1] = { '\0' };

String::String() : _data(s_empty_data), _size(0), _capasity(0)
{
}

String::String(const String& str) : _data(s_empty_data), _size(0), _capasity(0)
{
	set(str.data(), str.size());
}

String::String(String&& str) noexcept
{
	_data = str._data;
	_capasity = str._capasity;
	_size = str._size;

	str._data = nullptr;
	str._capasity = 0;
	str._size = 0;
}

String::String(const char* str) : _data(s_empty_data), _size(0), _capasity(0)
{
	set(str);
}

String::String(const char* str, unsigned length) : _data(s_empty_data), _size(0), _capasity(0)
{
	set(str, length);
}

String::String(unsigned length, char c) {
	resize(length, c);
}

String::~String() {
	if (_data != nullptr && _data != String::s_empty_data) {
		delete[] _data;
	}
}

char& String::operator[](unsigned index) {
	return _data[index];
}

const char& String::operator[](unsigned index) const {
	return _data[index];
}

bool String::operator==(const String& str) const {
	return strcmp(data(), str.data()) == 0;
}

bool String::operator==(const char* str) const {
	return strcmp(data(), str) == 0;
}

bool String::operator!=(const String& str) const {
	return strcmp(data(), str.data()) != 0;
}

bool String::operator!=(const char* str) const {
	return strcmp(data(), str) != 0;
}

bool String::operator<(const String& str) const {
	return strcmp(data(), str.data()) == -1;
}

bool String::operator<(const char* str) const {
	return strcmp(data(), str) == -1;
}

String& String::operator=(const String& str) {
	String copy(str);
	std::swap(*this, copy);
	return *this;
}

String& String::operator=(String&& str) noexcept
{
	if (_data != nullptr && _data != String::s_empty_data)
	{
		delete [] _data;
		_capasity = 0;
		_size = 0;
		_data = String::s_empty_data;
	}
	_data = str._data;
	_capasity = str._capasity;
	_size = str._size;

	str._data = String::s_empty_data;
	str._capasity = 0;
	str._size = 0;

	return *this;
}

String& String::operator=(const char* str) {
	set(str);
	return *this;
}

String& String::operator+=(const String& str) {
	return push_back(str);
}

String& String::operator+=(const char* str) {
	return push_back(str);
}

String String::operator+(const String& str) {
	String ns;
	ns.set(_data, _size);
	ns.push_back(str);
	return ns;
}

String String::operator+(const char* str) {
	String ns;
	ns.set(_data, _size);
	ns.push_back(str);
	return ns;
}

char* String::data() {
	return _data;
}

const char* String::data() const {
	return _data;
}

char* String::c_str() {
	return _data;
}

const char* String::c_str() const {
	return _data;
}

bool String::empty() const {
	return _size == 0;
}

unsigned String::size() const {
	return _size;
}

String& String::push_back(const String& str)
{
	return append(str.data(), str.size());
}

String& String::push_back(const char* str)
{
	return append(str);
}

String& String::push_back(char c)
{
	return append(c);
}

String& String::append(const char* str, int size)
{
	reserve(_size + size);
	memcpy(_data + _size, str, size);
	_size += size;
	_data[_size] = '\0';
	return *this;
}

String& String::append(const char* str)
{
	auto size = strlen(str);
	return append(str, size);
}

String& String::append(const String& str)
{
	return push_back(str);
}

String& String::append(char c)
{
	return append(&c, 1);
}

char& String::back()
{
	return _data[_size - 1];
}

const char& String::back() const
{
	return _data[_size - 1];
}

String String::format(const char* fmt, ...)
{
	String str;

	va_list args;
	va_start(args, fmt);
	int size = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	str.reserve(size+1);
	str._size = size;

	va_start(args, fmt);
	vsnprintf(str.data(), size + 1, fmt, args);
	va_end(args);

	str._data[size] = '\0';
	return str;
}

String String::fromInt(int num)
{
	#define MAXDIGITS 20
	char data[MAXDIGITS + 1] = {'\0'};

	itoa(num, data, 10);
	return String(data);
}

int String::toInt(const String& string)
{
	return atoi(string.data());
}

void String::resize(unsigned size)
{
	reserve(size);
	_size = size;
	_data[size] = '\0';
}

void String::resize(unsigned size, char c) {
	auto oldSize = _size;
	reserve(size);

	for (; oldSize < size; oldSize++)
	{
		_data[oldSize] = c;
	}
	_size = size;
	_data[size] = '\0';
}

void String::reserve(unsigned length)
{
	int buffer_size = length + 1;
	if (buffer_size > _capasity)
	{
		if (_capasity <= 0) {
			_capasity = 16;
		}

		while (_capasity < buffer_size)
		{
			_capasity *= 2;
		}

		if (_data == String::s_empty_data)
		{
			_data = nullptr;
			_data = new char[_capasity];
			memset(_data, 0, _capasity);
			_data[0] = '\0';
		} 		else 		{
			auto newBuffer = new char[_capasity];
			memcpy(newBuffer, _data, _size);
			newBuffer[_size] = '\0';
			delete[] _data;
			_data = newBuffer;
		}
	}
}

void String::clear()
{
	if (_capasity > 0)
	{
		data()[0] = '\0';
	}
	_size = 0;
}

void String::set(const char* from, unsigned size)
{
	reserve(size);
	_size = size;
	memcpy(_data, from, size);
	_data[size] = '\0';
}

void String::set(const char* from)
{
	auto size = strlen(from);
	set(from, size);

}

unsigned pxl::String::utf8Size(const pxl::String& str, unsigned index)
{
	auto ch = str[index];

	if ((ch & 0xFE) == 0xFC) {
		return 6;
	}
	else if ((ch & 0xFC) == 0xF8) {
		return 5;
	}
	else if ((ch & 0xF8) == 0xF0) {
		return 4;
	}
	else if ((ch & 0xF0) == 0xE0) {
		return 3;
	}
	else if ((ch & 0xE0) == 0xC0) {
		return 2;
	}
	return 1;
}

String operator+(const char* str0, const String& str1)
{
	String str(str0);
	str += str1;
	return str;
}

bool operator==(const char* str0, const pxl::String& str1)
{
	return strcmp(str0, str1.data()) == 0;
}
