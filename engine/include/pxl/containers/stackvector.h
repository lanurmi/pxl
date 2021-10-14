#pragma once
#include <cassert>

namespace pxl
{
	template<class T, unsigned C>
	class StackVector
	{
	public:
		StackVector();
		~StackVector();

		T& operator[](unsigned index);
		const T& operator[](unsigned index) const;

		void push_back(const T& item);
		void push_back(T&& item);

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
	StackVector<T, C>::StackVector() : _size(0)
	{
	}

	template<class T, unsigned C>
	StackVector<T, C>::~StackVector()
	{
		
	}

	template<class T, unsigned C>
	T& StackVector<T, C>::operator[](unsigned index)
	{
		auto d = data();
		return d[index];
	}

	template<class T, unsigned C>
	const T& StackVector<T, C>::operator[](unsigned index) const
	{
		auto d = data();
		return d[index];
	}

	template<class T, unsigned C>
	void StackVector<T, C>::push_back(const T& item)
	{
		assert(_size < C);
		new (data() + _size) T(item);
		_size++;
	}

	template<class T, unsigned C>
	void StackVector<T, C>::push_back(T&& item)
	{
		assert(_size < C);
		new (data() + _size) T( std::move(item) );
		_size++;
	}

	template<class T, unsigned C>
	T* StackVector<T, C>::data()
	{
		return (T*)_data;
	}

	template<class T, unsigned C>
	const T* StackVector<T, C>::data() const
	{
		return (T*)_data;
	}

	template<class T, unsigned C>
	unsigned StackVector<T, C>::size() const
	{
		return _size;
	}

	template<class T, unsigned C>
	T* StackVector<T, C>::begin()
	{
		return (T*)_data;
	}

	template<class T, unsigned C>
	const T* StackVector<T, C>::begin() const
	{
		return (T*)_data;
	}

	template<class T, unsigned C>
	T* StackVector<T, C>::end()
	{
		return (T*)_data + _size;
	}

	template<class T, unsigned C>
	const T* StackVector<T, C>::end() const
	{
		return (T*)_data + _size;
	}

	template<class T, unsigned C>
	T& StackVector<T, C>::front()
	{
		auto d = data();
		return d[0];
	}

	template<class T, unsigned C>
	const T& StackVector<T, C>::front() const
	{
		auto d = data();
		return d[0];
	}

	template<class T, unsigned C>
	T& StackVector<T, C>::back()
	{
		auto d = data();
		return d[_size - 1];
	}

	template<class T, unsigned C>
	const T& StackVector<T, C>::back() const
	{
		auto d = data();
		return d[_size - 1];
	}
}
