#include <pxl/utils/filestream.h>

using namespace pxl;
using namespace file;

FileStream::FileStream(const String& path, FileMode mode)
{
	_file = File::open(path, mode);
	assert(_file);
}

bool FileStream::eof() const
{
	return _file->position() >= _file->length();
}

String FileStream::line()
{
	String r;
	u8 c;
	while (read(&c, 1) && c != '\n' && c != '\0')
	{
		r.append(c);
	}
	return r;
}

String FileStream::all()
{
	String r;
	u8 c;
	while (read(&c, 1) && c != '\0')
	{
		r.append(c);
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

size_t FileStream::length() const
{
	return _file->length();
}

bool FileStream::readable() const
{
	return _file.get() != nullptr;
}

