#include <pxl/utils/filestream.h>

using namespace pxl;
using namespace file;

FileStream::FileStream(const string& path, FileMode mode)
{
	_file = File::open(path, mode);
	assert(_file);
}

bool FileStream::eof() const
{
	return _file->position() >= _file->length();
}

string FileStream::line()
{
	string r;
	u8 c;
	while (read(&c, 1) && c != '\n' && c != '\0')
	{
		r.push_back(c);
	}
	return r;
}

string FileStream::all()
{
	string r;
	u8 c;
	while (read(&c, 1) && c != '\0')
	{
		r.push_back(c);
	}
	return r;
}

size_t FileStream::position() const
{
	return _file->position();
}

size_t FileStream::seek(size_t to) const
{
	return _file->seek(to);
}

size_t FileStream::read(u8* ptr, size_t length)
{
	assert(length > 0);
	return _file->read(ptr, length);
}

bool FileStream::readable() const
{
	return _file.get() != nullptr;
}