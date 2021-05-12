#pragma once
#include <pxl/types.h>

namespace pxl
{
	namespace path
	{
		string extension(const string& path);
		string combine(const string lowerPath, const string upperPath);
		string withoutFile(const string& path);
	}
	namespace file
	{
		bool exists(const string& path);

		class File;
		using FileRef = std::shared_ptr<File>;
		FileRef open(const string& path);

		class File
		{
		public:
			static FileRef create(const string& file);
			File(const File&) = delete;
			File(File&&) = delete;
			File& operator=(const File&) = delete;
			File& operator=(File&&) = delete;
			virtual ~File() = default;
			virtual bool ok() const = 0;
			virtual string line() = 0;
			virtual bool eof() const = 0;
		protected:
			File() {}
		};
	}
	namespace directory
	{
		vector<string> files(const string& path, const string& extension = "");
		bool exists(const string& path);
	}
}