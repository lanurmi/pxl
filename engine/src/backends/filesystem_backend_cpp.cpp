#include <pxl/types.h>
#include <pxl/utils/filestream.h>
#include <filesystem>
#include <cstdio>


using namespace pxl;

String path::extension(const String& path)
{
	std::filesystem::path filePath(path.cstr());
	auto str = filePath.extension().u8string();
	return String(str.c_str(), str.size());
}

String path::combine(const String lowerPath, const String upperPath)
{
	std::filesystem::path path(lowerPath.cstr());
	path.append(upperPath.cstr());
	auto str = path.u8string();
	return String(str.c_str(), str.size());
}

String path::withoutFile(const String& path)
{
	std::filesystem::path filePath(path.cstr());
	auto str = filePath.parent_path().u8string();
	return String(str.c_str(), str.size());
}

String path::filename(const String& path)
{
	std::filesystem::path filePath(path.cstr());
	auto str = filePath.stem().u8string();
	return String(str.c_str(), str.size());
}

class STDIOFile : public file::File 
{
public:
	STDIOFile(FILE* file) : m_file(file)
	{

	}
	~STDIOFile()
	{
		fclose(m_file);
	}
	size_t length() const override
	{

		auto curr = stdioSeek(m_file, 0, SEEK_CUR);
		if (curr < 0) {
			assert(0);
		}
		auto size = stdioSeek(m_file,0, SEEK_END);
		stdioSeek(m_file, curr, SEEK_SET);
		return size;
	}
	size_t position() const override
	{
		return stdioSeek(m_file, 0, SEEK_CUR);
	}
	size_t seek(size_t position) override 	{
		return stdioSeek(m_file, position, SEEK_SET);
	}
	size_t read(u8* buffer, size_t length) const override
	{
		return fread(buffer, sizeof(u8), length, m_file);
	}
	size_t write(const u8* buffer, size_t length) override 
	{
		return fwrite(buffer, sizeof(u8), length, m_file);
	}
private:
	static size_t stdioSeek(FILE* file, size_t offset, int whence)
	{
		if (fseek(file, (long)offset, whence) == 0) 
		{
			return ftell(file);
		}
		assert(0);
		return 0;
	}

	FILE* m_file;
};

bool file::exists(const String& path)
{
	return std::filesystem::is_regular_file(path.cstr());
}

file::FileRef file::File::open(const String& file, file::FileMode mode)
{
	String smode = "";
	switch (mode)
	{
		case file::FileMode::ReadBinary:
		{
			smode = "rb";
			break;
		}
		case file::FileMode::Read:
		{
			smode = "r";
			break;
		}
	}
	FILE *ptr = fopen(file.cstr(), smode.cstr());
	if (ptr == nullptr)
	{
		return file::FileRef();
	}
	else
	{
		return file::FileRef(new STDIOFile(ptr));
	}
}

// directory

bool directory::exists(const String& path)
{
	return std::filesystem::is_directory(path.cstr());
}

Vector<String> directory::files(const String& path, const String& extension)
{
	Vector<String> result;
	for (const auto& e : std::filesystem::directory_iterator(path.cstr()))
	{
		auto p = e.path();
		if (extension.empty())
		{
			auto str = p.u8string();
			result.emplace_back(String(str.c_str(), str.length()));
		}
		else
		{
			auto ext = p.extension();
			if (ext.u8string().c_str() == extension)
			{
				auto str = p.u8string();
				result.push_back(String(str.c_str(), str.size()));
			}
		}
	}
	return result;
}
