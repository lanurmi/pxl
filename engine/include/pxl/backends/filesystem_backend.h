#pragma once
#include <pxl/types.h>
#include <functional>

namespace pxl
{
	namespace path
	{
		String extension(const String& path);
		String combine(const String lowerPath, const String upperPath);
		String withoutFile(const String& path);
		String filename(const String& path);
	}
	namespace file
	{
		bool exists(const String& path);

		class File;
		using FileRef = std::shared_ptr<File>;

		enum class FileMode
		{
			ReadBinary,
			Read
		};
		class File
		{
		public:

			static FileRef open(const String& file, FileMode mode);
			File(const File&) = delete;
			File(File&&) = delete;
			File& operator=(const File&) = delete;
			File& operator=(File&&) = delete;
			virtual ~File() = default;
			virtual size_t length() const = 0;
			virtual size_t position() const = 0;
			virtual size_t seek(size_t position) = 0;
			virtual size_t read(u8* buffer, size_t length) const = 0;
			virtual size_t write(const u8* buffer, size_t length) = 0;
		protected:
			File() {}
		};
	}
	namespace directory
	{
		Vector<String> files(const String& path, const String& extension = "");
		bool exists(const String& path);
	}
}
