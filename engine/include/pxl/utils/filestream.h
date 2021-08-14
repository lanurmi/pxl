#pragma once
#include <pxl/backends/filesystem_backend.h>

namespace pxl
{
	using namespace file;

	class FileStream
	{
	public:
		FileStream(const String& path, FileMode mode);
		bool eof() const;
		String line();
		String all();
		size_t position() const;
		size_t seek(size_t to) const;
		size_t read(u8* ptr, size_t length);
		size_t length() const;
		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		T read()
		{
			T r;
			read((u8*)&r, sizeof(T));
			return r;
		}
		bool readable() const;
	private:
		FileRef _file;
	};
}