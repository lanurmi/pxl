#pragma once
#include <pxl/types.h>

namespace pxl
{
	namespace path
	{
		string extension(const string& path);
		string combine(const string lowerPath, const string upperPath);
		string withoutFile(const string& path);
		string filename(const string& path);
	}
	namespace file
	{
		bool exists(const string& path);

		class File;
		using FileRef = std::shared_ptr<File>;
		FileRef open(const string& path);

		enum class FileMode
		{
			ReadBinary,
			Read
		};
		class File
		{
		public:

			static FileRef open(const string& file, FileMode mode);
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

		class FileStream
		{
		public:
			FileStream(const string& path, FileMode mode);
			bool eof() const;
			string line();
			size_t position() const;
			size_t seek(size_t to) const;
			size_t read(u8* ptr, size_t length);
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
	namespace directory
	{
		vector<string> files(const string& path, const string& extension = "");
		bool exists(const string& path);
	}
}