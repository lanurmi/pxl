#pragma once
#include <cassert>

namespace pxl
{
	template<class T, unsigned C>
	class Array
	{
	public:
		Array();
		~Array();

		T& operator[](unsigned index);
		const T& operator[](unsigned index) const;

		void add(const T& item);
		void add(T&& item);

		T* data();
		const T* data() const;	
		unsigned size() const;

		T* begin();
		const T* begin() const;
		T* end();
		const T* end() const;
		T& front();
		const T& front() const;
		T& back();
		const T& back() const;

	private:
		char _data[sizeof(T) * C];
		unsigned _size;
	};

	template<class T, unsigned C>
	Array<T, C>::Array() : _size(0)
	{
	}

	template<class T, unsigned C>
	Array<T, C>::~Array()
	{
		
	}

	template<class T, unsigned C>
	T& Array<T, C>::operator[](unsigned index)
	{
		auto d = data();
		return d[index];
	}

	template<class T, unsigned C>
	const T& Array<T, C>::operator[](unsigned index) const
	{
		auto d = data();
		return d[index];
	}

	template<class T, unsigned C>
	void Array<T, C>::add(const T& item)
	{
		assert(_size < C);
		new (data() + _size) T(item);
		_size++;
	}

	template<class T, unsigned C>
	void Array<T, C>::add(T&& item)
	{
		assert(_size < C);
		new (data() + _size) T( std::move(item) );
		_size++;
	}

	template<class T, unsigned C>
	T* Array<T, C>::data()
	{
		return (T*)_data;
	}

	template<class T, unsigned C>
	const T* Array<T, C>::data() const
	{
		return (T*)_data;
	}

	template<class T, unsigned C>
	unsigned Array<T, C>::size() const
	{
		return _size;
	}

	template<class T, unsigned C>
	T* Array<T, C>::begin() 
	{
		return (T*)_data;
	}

	template<class T, unsigned C>
	const T* Array<T, C>::begin() const
	{
		return (T*)_data;
	}

	template<class T, unsigned C>
	T* Array<T, C>::end()
	{
		return (T*)_data + _size;
	}

	template<class T, unsigned C>
	const T* Array<T, C>::end() const
	{
		return (T*)_data + _size;
	}

	template<class T, unsigned C>
	T& Array<T, C>::front()
	{
		auto d = data();
		return d[0];
	}

	template<class T, unsigned C>
	const T& Array<T, C>::front() const
	{
		auto d = data();
		return d[0];
	}

	template<class T, unsigned C>
	T& Array<T, C>::back()
	{
		auto d = data();
		return d[_size - 1];
	}

	template<class T, unsigned C>
	const T& Array<T, C>::back() const
	{
		auto d = data();
		return d[_size - 1];
	}
}
