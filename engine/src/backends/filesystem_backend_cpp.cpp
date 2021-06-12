#include <pxl/types.h>
#include <pxl/utils/filestream.h>
#include <filesystem>
#include <cstdio>


using namespace pxl;

string path::extension(const string& path)
{
	std::filesystem::path filePath(path);
	return filePath.extension().u8string();
}

string path::combine(const string lowerPath, const string upperPath)
{
	std::filesystem::path path(lowerPath);
	path.append(upperPath);
	return path.u8string();
}

string path::withoutFile(const string& path)
{
	std::filesystem::path filePath(path);
	return filePath.parent_path().u8string();
}

string path::filename(const string& path)
{
	std::filesystem::path filePath(path);
	return filePath.stem().u8string();
}

class STDIOFile : public file::File {
public:
	STDIOFile(FILE* file) : m_file(file) 	{

	}
	~STDIOFile() {
		fclose(m_file);
	}
	size_t length() const override 	{

		auto curr = stdioSeek(m_file, 0, SEEK_CUR);
		if (curr < 0) {
			assert(0);
		}
		auto size = stdioSeek(m_file,0, SEEK_END);
		stdioSeek(m_file, curr, SEEK_SET);
		return size;
	}
	size_t position() const override {
		return stdioSeek(m_file, 0, SEEK_CUR);
	}
	size_t seek(size_t position) override 	{
		return stdioSeek(m_file, position, SEEK_SET);
	}
	size_t read(u8* buffer, size_t length) const override 	{
		return fread(buffer, sizeof(u8), length, m_file);
	}
	size_t write(const u8* buffer, size_t length) override 	{
		return fwrite(buffer, sizeof(u8), length, m_file);
	}
private:
	static size_t stdioSeek(FILE* file, size_t offset, int whence) {
		if (fseek(file, (long)offset, whence) == 0) {
			return ftell(file);
		}
		assert(0);
		return 0;
	}

	FILE* m_file;
};

bool file::exists(const string& path)
{
	return std::filesystem::is_regular_file(path);
}

file::FileRef file::File::open(const string& file, file::FileMode mode)
{
	string smode = "";
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
	FILE *ptr = fopen(file.c_str(), smode.c_str());
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

bool directory::exists(const string& path)
{
	return std::filesystem::is_directory(path);
}

vector<string> directory::files(const string& path, const string& extension)
{
	vector<string> result;
	for (const auto& e : std::filesystem::directory_iterator(path))
	{
		auto p = e.path();
		if (extension.empty())
		{
			result.push_back(p.u8string());
		}
		else
		{
			auto ext = p.extension();
			if (ext.u8string() == extension)
			{
				result.push_back(p.u8string());
			}
		}
	}
	return result;
}
