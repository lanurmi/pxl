#pragma once
#include <memory>
#include <pxl/types.h>
#include <pxl/containers/stackvector.h>

namespace pxl
{
	enum class VertexType
	{
		Invalid,
		Float,
		Float2,
		Float3,
		Float4,
		Byte4,
		UByte4,
		Short2,
		UShort2,
		Short4,
		UShort4
	};

	struct VertexAttribute
	{
		int index = 0;
		VertexType type = VertexType::Invalid;
		bool normalized = false;
	};
	struct VertexFormat
	{
		VertexFormat(std::initializer_list<VertexAttribute> attributes, int stride = 0);
		VertexFormat() = default;
		StackVector<VertexAttribute, 16> attributes;
		int stride = 0;
	};
	enum class IndexFormat
	{
		UInt16,
		UInt32
	};

	class Mesh;
	using MeshRef = std::shared_ptr<Mesh>;

	class Mesh
	{
	public:
		static MeshRef create();
		Mesh(const Mesh&) = delete;
		Mesh(Mesh&&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh&&) = delete;
		virtual ~Mesh() = default;
		virtual void setIndexData(IndexFormat format, const void* indices, i64 count) = 0;
		virtual void setVertexData(const VertexFormat& format, const void* vertices, i64 count) = 0;
		virtual i64 getIndexCount() const = 0;
		virtual i64 getVertexCount() const = 0;
	protected:
		Mesh() = default;
	};
}