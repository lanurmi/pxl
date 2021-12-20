#include <pxl/graphics/mesh.h>
#include <pxl/engine.h>

pxl::VertexFormat::VertexFormat(std::initializer_list<VertexAttribute> attribs, int stride)
{
	for (auto& it : attribs)
	{
		this->attributes.push_back(it);
	}

	if (stride <= 0)
	{
		stride = 0;
		for (auto& it : attributes)
		{
			switch (it.type)
			{
				case VertexType::Invalid: break;
				case VertexType::Float: stride += 4; break;
				case VertexType::Float2: stride += 8; break;
				case VertexType::Float3: stride += 12; break;
				case VertexType::Float4: stride += 16; break;
				case VertexType::Byte4: stride += 4; break;
				case VertexType::UByte4: stride += 4; break;
				case VertexType::Short2: stride += 4; break;
				case VertexType::UShort2: stride += 4; break;
				case VertexType::Short4: stride += 8; break;
				case VertexType::UShort4: stride += 8; break;
			}
		}
	}

	this->stride = stride;
}

pxl::MeshRef pxl::Mesh::create()
{
	return pxl::graphics::createMesh();
}

