#pragma once

#include <pxl/types.h>

namespace pxl
{
	enum class UniformType
	{
		None,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3x2,
		Mat4x4,
		Texture2D,
		Sampler2D
	};

	enum class ShaderType
	{
		None = 0,
		Vertex = 1 << 0,
		Fragment = 1 << 1
	};

	struct UniformInfo
	{
		string name;
		UniformType type;
		ShaderType shader;
		int buffer_index;
		int array_length;
	};

	class Shader;
	using ShaderRef = std::shared_ptr<Shader>;

	struct ShaderData
	{
		string vertex;
		string pixel;
	};

	class Shader
	{
	public:
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = delete;
		virtual ~Shader() = default;
		static ShaderRef create(const ShaderData& data);
		static ShaderRef create();
		const vector<UniformInfo>& uniforms() const;
	protected:
		Shader() = default;
		void addUniformInfo(const UniformInfo& info);
	private:
		vector<UniformInfo> _uniforms;
	};
}