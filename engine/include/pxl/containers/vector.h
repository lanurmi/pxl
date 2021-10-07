#pragma once

#include <new>
#include <cassert>

namespace pxl
{
	template<class T>
	class Vector {
	public:
		Vector();
		Vector(unsigned initialCapasity);
		Vector(const Vector& src);
		Vector(Vector&& src) noexcept;
		~Vector();

		Vector& operator=(const Vector& src);
		Vector& operator=(Vector&& src) noexcept;

		T& operator[](unsigned idx);
		const T& operator[](unsigned idx) const;

		int size() const;
		void reserve(unsigned newCapasity);
		void resize(unsigned newSize);

		void add(const T &item);
		void add(T &&item);

		void erase(unsigned index, unsigned amount);
		void erase(const T &item);

		void expand(unsigned amount);
		T pop();

		void clear();

		bool empty() const;

		T* data();
		const T* data() const;

		T* begin();
		const T* begin() const;
		T* end();
		const T* end() const;
		T& front();
		const T& front() const;
		T& back();
		const T& back() const;
	private:
		T *_data;
		unsigned _size;
		unsigned _capacity;
	};


	template<class T>
	Vector<T>::Vector() : _data(nullptr), _size(0), _capacity(0)
	{

	}

	template<class T>
	Vector<T>::Vector(unsigned initialCapasity) : _data(nullptr), _size(0), _capacity(0)
	{
		reserve(initialCapasity);
	}
	template<class T>
	Vector<T>::Vector(const Vector &src) : _data(nullptr), _size(0), _capacity(0)
	{
		reserve(src._capacity);
		for (auto& it : src)
		{
			add(it);
		}
	}

	template<class T>
	Vector<T>::Vector(Vector&& src) noexcept : _data(nullptr), _size(0), _capacity(0)
	{
		_data = src._data;
		_capacity = src._capacity;
		_size = src._size;

		src._data = nullptr;
		src._capacity = 0;
		src._size = 0;
	}

	template<class T>
	Vector<T>::~Vector()
	{
		clear();
		::operator delete (_data, sizeof(T) * _capacity);
		_capacity = 0;
		_data = nullptr;
	}

	template<class T>
	Vector<T>& Vector<T>::operator=(const Vector<T>& src)
	{
		clear();
		::operator delete (_data, sizeof(T) * _capacity);
		_capacity = 0;
		_data = nullptr;

		reserve(src._capacity);
		for (auto& it : src)
		{
			add(it);
		}

		return *this;

	}

	template<class T>
	Vector<T>& Vector<T>::operator=(Vector<T>&& src) noexcept
	{
		clear();
		::operator delete (_data, sizeof(T) * _capacity);
		_capacity = 0;
		_data = nullptr;

		_data = src._data;
		_capacity = src._capacity;
		_size = src._size;

		src._data = nullptr;
		src._capacity = 0;
		src._size = 0;
		return *this;
	}

	template<class T>
	T& Vector<T>::operator[](unsigned idx)
	{
		return _data[idx];
	}

	template<class T>
	const T& Vector<T>::operator[](unsigned idx) const
	{
		return _data[idx];
	}

	template<class T>
	int Vector<T>::size() const
	{
		return _size;
	}

	template<class T>
	void  Vector<T>::reserve(unsigned newCapasity)
	{
		if (newCapasity > _capacity)
		{
			auto nc = _capacity;
			if (nc <= 0)
			{
				nc = 8;
			}
			while(nc < newCapasity)
			{
				nc *= 2;
			}

			T* newData = (T*)::operator new (sizeof(T) * nc);

			for (unsigned i = 0; i < _size; i++)
			{
				if (i < nc)
				{
					new (newData + i) T(std::move(_data[i]));
				}
				_data[i].~T();
			}

			::operator delete (_data, sizeof(T) * _capacity);

			_data = newData;
			_capacity = nc;
		}
	}

	template<class T>
	void Vector<T>::resize(unsigned newSize)
	{
		if(newSize < _size) 
		{
			erase(newSize, _size - newSize);
		}
		else
		{
			expand(newSize - _size);
		}
	}

	template<class T>
	void Vector<T>::add(const T& item)
	{
		reserve(_size + 1);
		new (_data + _size) T(item);
		_size++;
	}

	template<class T>
	void Vector<T>::add(T&& item)
	{
		reserve(_size + 1);
		new (_data + _size) T(std::move(item));
		_size++;
	}

	template<class T>
	void Vector<T>::erase(unsigned index, unsigned amount)
	{
		if (amount >= 1)
		{
			for (unsigned i = index; i < _size - amount; i++) 
			{
				_data[i] = std::move(_data[i + amount]);
			}

			for (unsigned i = _size - amount; i < _size; i++)
			{
				_data[i].~T();
			}
			_size -= amount;
		}
	}

	template<class T>
	void Vector<T>::erase(const T& item) {
		for(unsigned i = 0; i < _size; i++)
		{
			if (_data[i] == item)
			{
				erase(i,1);
			}
		}
	}

	template<class T>
	void Vector<T>::expand(unsigned amount)
	{
		if (amount > 0)
		{
			int s = _size;
			reserve(s + amount);
			for (unsigned i = 0; i < amount; i++)
			{
				new (_data + s + i) T();
			}
			_size += amount;
		}
	}

	template<class T>
	T Vector<T>::pop()
	{
		assert(_size > 0);
		T r = std::move(_data[_size - 1]);
		_data[_size - 1]. ~T();
		_size--;
		return r;
	}

	template<class T>
	void Vector<T>::clear()
	{
		for (unsigned i = 0; i < _size; i++)
		{
			_data[i].~T();
		}
		_size = 0;
	}

	template<class T>
	bool Vector<T>::empty() const
	{
		return _size == 0;
	}

	template<class T>
	T* Vector<T>::data()
	{
		return _data;
	}

	template<class T>
	const T* Vector<T>::data() const
	{
		return _data;
	}

	template<class T>
	T* Vector<T>::begin()
	{
		return _data;
	}

	template<class T>
	const T* Vector<T>::begin() const 
	{
		return _data;
	}

	template<class T>
	T* Vector<T>::end() 
	{
		return _data + _size;
	}

	template<class T>
	const T* Vector<T>::end() const
	{
		return _data + _size;
	}

	template<class T>
	T& Vector<T>::front()
	{
		return _data[0];
	}

	template<class T>
	const T& Vector<T>::front() const
	{
		return _data[0];
	}

	template<class T>
	T& Vector<T>::back()
	{
		return _data[_size - 1];
	}

	template<class T>
	const T& Vector<T>::back() const
	{
		return _data[_size - 1];
	}
}