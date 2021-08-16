#include <pxl/backends/graphics_backend.h>
#include <pxl/engine.h>
#include <pxl/graphics/sampler.h>
#include <pxl/graphics/texture.h>
#include <pxl/graphics/shader.h>
#include <pxl/graphics/mesh.h>
#include <assert.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#define APIENTRY
#endif

// OpenGL Value Types
typedef ptrdiff_t        GLintptr;
typedef ptrdiff_t        GLsizeiptr;
typedef unsigned int     GLenum;
typedef unsigned char    GLboolean;
typedef unsigned int     GLbitfield;
typedef void             GLvoid;
typedef signed char      GLbyte;      /* 1-byte signed */
typedef short            GLshort;     /* 2-byte signed */
typedef int              GLint;       /* 4-byte signed */
typedef unsigned char    GLubyte;     /* 1-byte unsigned */
typedef unsigned short   GLushort;    /* 2-byte unsigned */
typedef unsigned int     GLuint;      /* 4-byte unsigned */
typedef int              GLsizei;     /* 4-byte signed */
typedef float            GLfloat;     /* single precision float */
typedef float            GLclampf;    /* single precision float in [0,1] */
typedef double           GLdouble;    /* double precision float */
typedef double           GLclampd;    /* double precision float in [0,1] */
typedef char             GLchar;

// OpenGL Constants
#define GL_DONT_CARE 0x1100
#define GL_ZERO 0x0000
#define GL_ONE 0x0001
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_HALF_FLOAT 0x140B
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_SHORT_5_5_5_1_REV 0x8366
#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_INT_24_8 0x84FA
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_DEPTH_BUFFER_BIT 0x0100
#define GL_STENCIL_BUFFER_BIT 0x0400
#define GL_SCISSOR_TEST 0x0C11
#define GL_DEPTH_TEST 0x0B71
#define GL_STENCIL_TEST 0x0B90
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_CW 0x0900
#define GL_CCW 0x0901
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_FRONT_AND_BACK 0x0408
#define GL_CULL_FACE 0x0B44
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_3D 0x806F
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_BLEND 0x0BE2
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_CONSTANT_COLOR 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#define GL_CONSTANT_ALPHA 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#define GL_SRC1_ALPHA 0x8589
#define GL_SRC1_COLOR 0x88F9
#define GL_ONE_MINUS_SRC1_COLOR 0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA 0x88FB
#define GL_MIN 0x8007
#define GL_MAX 0x8008
#define GL_FUNC_ADD 0x8006
#define GL_FUNC_SUBTRACT 0x800A
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_INVERT 0x150A
#define GL_KEEP 0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR 0x1E02
#define GL_DECR 0x1E03
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508
#define GL_REPEAT 0x2901
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_MIRRORED_REPEAT 0x8370
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_RED 0x1903
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_LUMINANCE 0x1909
#define GL_RGB8 0x8051
#define GL_RGBA8 0x8058
#define GL_RGBA4 0x8056
#define GL_RGB5_A1 0x8057
#define GL_RGB10_A2_EXT 0x8059
#define GL_RGBA16 0x805B
#define GL_BGRA 0x80E1
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_RG 0x8227
#define GL_RG8 0x822B
#define GL_RG16 0x822C
#define GL_R16F 0x822D
#define GL_R32F 0x822E
#define GL_RG16F 0x822F
#define GL_RG32F 0x8230
#define GL_RGBA32F 0x8814
#define GL_RGBA16F 0x881A
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#define GL_DEPTH_COMPONENT 0x1902
#define GL_DEPTH_STENCIL 0x84F9
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_TEXTURE_LOD_BIAS 0x8501
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_TEXTURE0 0x84C0
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STREAM_DRAW 0x88E0
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_FRAMEBUFFER 0x8D40
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_RENDERBUFFER 0x8D41
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_QUERY_RESULT 0x8866
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#define GL_SAMPLES_PASSED 0x8914
#define GL_MULTISAMPLE 0x809D
#define GL_MAX_SAMPLES 0x8D57
#define GL_SAMPLE_MASK 0x8E51
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_SAMPLER_2D 0x8B5E
#define GL_FLOAT_MAT3x2 0x8B67
#define GL_FLOAT_MAT4 0x8B5C
#define GL_NUM_EXTENSIONS 0x821D
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#define GL_DEBUG_SOURCE_OTHER 0x824B
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#define GL_DEBUG_TYPE_POP_GROUP 0x826A
#define GL_DEBUG_TYPE_MARKER 0x8268
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_DEBUG_OUTPUT 0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242

// OpenGL Functions
#define GL_FUNCTIONS \
	GL_FUNC(DebugMessageCallback, void, DEBUGPROC callback, const void* userParam) \
	GL_FUNC(GetString, const GLubyte*, GLenum name) \
	GL_FUNC(Flush, void, void) \
	GL_FUNC(Enable, void, GLenum mode) \
	GL_FUNC(Disable, void, GLenum mode) \
	GL_FUNC(Clear, void, GLenum mask) \
	GL_FUNC(ClearColor, void, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) \
	GL_FUNC(ClearDepth, void, GLdouble depth) \
	GL_FUNC(ClearStencil, void, GLint stencil) \
	GL_FUNC(DepthMask, void, GLboolean enabled) \
	GL_FUNC(DepthFunc, void, GLenum func) \
	GL_FUNC(Viewport, void, GLint x, GLint y, GLint width, GLint height) \
	GL_FUNC(Scissor, void, GLint x, GLint y, GLint width, GLint height) \
	GL_FUNC(CullFace, void, GLenum mode) \
	GL_FUNC(BlendEquation, void, GLenum eq) \
	GL_FUNC(BlendEquationSeparate, void, GLenum modeRGB, GLenum modeAlpha) \
	GL_FUNC(BlendFunc, void, GLenum sfactor, GLenum dfactor) \
	GL_FUNC(BlendFuncSeparate, void, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) \
	GL_FUNC(BlendColor, void, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) \
	GL_FUNC(ColorMask, void, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) \
	GL_FUNC(GetIntegerv, void, GLenum name, GLint* data) \
	GL_FUNC(GenTextures, void, GLint n, void* textures) \
	GL_FUNC(GenRenderbuffers, void, GLint n, void* textures) \
	GL_FUNC(GenFramebuffers, void, GLint n, void* textures) \
	GL_FUNC(ActiveTexture, void, GLuint id) \
	GL_FUNC(BindTexture, void, GLenum target, GLuint id) \
	GL_FUNC(BindRenderbuffer, void, GLenum target, GLuint id) \
	GL_FUNC(BindFramebuffer, void, GLenum target, GLuint id) \
	GL_FUNC(TexImage2D, void, GLenum target, GLint level, GLenum internalFormat, GLint width, GLint height, GLint border, GLenum format, GLenum type, void* data) \
	GL_FUNC(FramebufferRenderbuffer, void, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) \
	GL_FUNC(FramebufferTexture2D, void, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) \
	GL_FUNC(TexParameteri, void, GLenum target, GLenum name, GLint param) \
	GL_FUNC(RenderbufferStorage, void, GLenum target, GLenum internalformat, GLint width, GLint height) \
	GL_FUNC(GetTexImage, void, GLenum target, GLint level, GLenum format, GLenum type, void* data) \
	GL_FUNC(DrawElements, void, GLenum mode, GLint count, GLenum type, void* indices) \
	GL_FUNC(DrawElementsInstanced, void, GLenum mode, GLint count, GLenum type, void* indices, GLint amount) \
	GL_FUNC(DeleteTextures, void, GLint n, GLuint* textures) \
	GL_FUNC(DeleteRenderbuffers, void, GLint n, GLuint* renderbuffers) \
	GL_FUNC(DeleteFramebuffers, void, GLint n, GLuint* textures) \
	GL_FUNC(GenVertexArrays, void, GLint n, GLuint* arrays) \
	GL_FUNC(BindVertexArray, void, GLuint id) \
	GL_FUNC(GenBuffers, void, GLint n, GLuint* arrays) \
	GL_FUNC(BindBuffer, void, GLenum target, GLuint buffer) \
	GL_FUNC(BufferData, void, GLenum target, GLsizeiptr size, const void* data, GLenum usage) \
	GL_FUNC(BufferSubData, void, GLenum target, GLintptr offset, GLsizeiptr size, const void* data) \
	GL_FUNC(DeleteBuffers, void, GLint n, GLuint* buffers) \
	GL_FUNC(DeleteVertexArrays, void, GLint n, GLuint* arrays) \
	GL_FUNC(EnableVertexAttribArray, void, GLuint location) \
	GL_FUNC(DisableVertexAttribArray, void, GLuint location) \
	GL_FUNC(VertexAttribPointer, void, GLuint index, GLint size, GLenum type, GLboolean normalized, GLint stride, const void* pointer) \
	GL_FUNC(VertexAttribDivisor, void, GLuint index, GLuint divisor) \
	GL_FUNC(CreateShader, GLuint, GLenum type) \
	GL_FUNC(AttachShader, void, GLuint program, GLuint shader) \
	GL_FUNC(DetachShader, void, GLuint program, GLuint shader) \
	GL_FUNC(DeleteShader, void, GLuint shader) \
	GL_FUNC(ShaderSource, void, GLuint shader, GLsizei count, const GLchar** string, const GLint* length) \
	GL_FUNC(CompileShader, void, GLuint shader) \
	GL_FUNC(GetShaderiv, void, GLuint shader, GLenum pname, GLint* result) \
	GL_FUNC(GetShaderInfoLog, void, GLuint shader, GLint maxLength, GLsizei* length, GLchar* infoLog) \
	GL_FUNC(CreateProgram, GLuint, ) \
	GL_FUNC(DeleteProgram, void, GLuint program) \
	GL_FUNC(LinkProgram, void, GLuint program) \
	GL_FUNC(GetProgramiv, void, GLuint program, GLenum pname, GLint* result) \
	GL_FUNC(GetProgramInfoLog, void, GLuint program, GLint maxLength, GLsizei* length, GLchar* infoLog) \
	GL_FUNC(GetActiveUniform, void, GLuint program, GLuint index, GLint bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) \
	GL_FUNC(GetActiveAttrib, void, GLuint program, GLuint index, GLint bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) \
	GL_FUNC(UseProgram, void, GLuint program) \
	GL_FUNC(GetUniformLocation, GLint, GLuint program, const GLchar* name) \
	GL_FUNC(GetAttribLocation, GLint, GLuint program, const GLchar* name) \
	GL_FUNC(Uniform1f, void, GLint location, GLfloat v0) \
	GL_FUNC(Uniform2f, void, GLint location, GLfloat v0, GLfloat v1) \
	GL_FUNC(Uniform3f, void, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) \
	GL_FUNC(Uniform4f, void, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) \
	GL_FUNC(Uniform1fv, void, GLint location, GLint count, const GLfloat* value) \
	GL_FUNC(Uniform2fv, void, GLint location, GLint count, const GLfloat* value) \
	GL_FUNC(Uniform3fv, void, GLint location, GLint count, const GLfloat* value) \
	GL_FUNC(Uniform4fv, void, GLint location, GLint count, const GLfloat* value) \
	GL_FUNC(Uniform1i, void, GLint location, GLint v0) \
	GL_FUNC(Uniform2i, void, GLint location, GLint v0, GLint v1) \
	GL_FUNC(Uniform3i, void, GLint location, GLint v0, GLint v1, GLint v2) \
	GL_FUNC(Uniform4i, void, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) \
	GL_FUNC(Uniform1iv, void, GLint location, GLint count, const GLint* value) \
	GL_FUNC(Uniform2iv, void, GLint location, GLint count, const GLint* value) \
	GL_FUNC(Uniform3iv, void, GLint location, GLint count, const GLint* value) \
	GL_FUNC(Uniform4iv, void, GLint location, GLint count, const GLint* value) \
	GL_FUNC(Uniform1ui, void, GLint location, GLuint v0) \
	GL_FUNC(Uniform2ui, void, GLint location, GLuint v0, GLuint v1) \
	GL_FUNC(Uniform3ui, void, GLint location, GLuint v0, GLuint v1, GLuint v2) \
	GL_FUNC(Uniform4ui, void, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) \
	GL_FUNC(Uniform1uiv, void, GLint location, GLint count, const GLint* value) \
	GL_FUNC(Uniform2uiv, void, GLint location, GLint count, const GLint* value) \
	GL_FUNC(Uniform3uiv, void, GLint location, GLint count, const GLint* value) \
	GL_FUNC(Uniform4uiv, void, GLint location, GLint count, const GLint* value) \
	GL_FUNC(UniformMatrix2fv, void, GLint location, GLint count, GLboolean transpose, const GLfloat* value) \
	GL_FUNC(UniformMatrix3fv, void, GLint location, GLint count, GLboolean transpose, const GLfloat* value) \
	GL_FUNC(UniformMatrix4fv, void, GLint location, GLint count, GLboolean transpose, const GLfloat* value) \
	GL_FUNC(UniformMatrix2x3fv, void, GLint location, GLint count, GLboolean transpose, const GLfloat* value) \
	GL_FUNC(UniformMatrix3x2fv, void, GLint location, GLint count, GLboolean transpose, const GLfloat* value) \
	GL_FUNC(UniformMatrix2x4fv, void, GLint location, GLint count, GLboolean transpose, const GLfloat* value) \
	GL_FUNC(UniformMatrix4x2fv, void, GLint location, GLint count, GLboolean transpose, const GLfloat* value) \
	GL_FUNC(UniformMatrix3x4fv, void, GLint location, GLint count, GLboolean transpose, const GLfloat* value) \
	GL_FUNC(UniformMatrix4x3fv, void, GLint location, GLint count, GLboolean transpose, const GLfloat* value) \
	GL_FUNC(PixelStorei, void, GLenum pname, GLint param)

typedef void (APIENTRY* DEBUGPROC)(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);

struct GLState
{
#define GL_FUNC(name, ret, ...) typedef ret (*name ## Func) (__VA_ARGS__); name ## Func name;
	GL_FUNCTIONS
#undef GL_FUNC

		void* context;

	// info
	int max_color_attachments;
	int max_element_indices;
	int max_element_vertices;
	int max_renderbuffer_size;
	int max_samples;
	int max_texture_image_units;
	int max_texture_size;

	pxl::graphics::GraphicsFeatures features;
};

GLState s_gl;

void APIENTRY gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// these are basically never useful
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION &&
		type == GL_DEBUG_TYPE_OTHER)
		return;

	const char* typeName = "";
	const char* severityName = "";

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR: typeName = "ERROR"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeName = "DEPRECATED BEHAVIOR"; break;
		case GL_DEBUG_TYPE_MARKER: typeName = "MARKER"; break;
		case GL_DEBUG_TYPE_OTHER: typeName = "OTHER"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: typeName = "PEROFRMANCE"; break;
		case GL_DEBUG_TYPE_POP_GROUP: typeName = "POP GROUP"; break;
		case GL_DEBUG_TYPE_PORTABILITY: typeName = "PORTABILITY"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP: typeName = "PUSH GROUP"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeName = "UNDEFINED BEHAVIOR"; break;
	}

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH: severityName = "HIGH"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: severityName = "MEDIUM"; break;
		case GL_DEBUG_SEVERITY_LOW: severityName = "LOW"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: severityName = "NOTIFICATION"; break;
	}

	/*
	if (type == GL_DEBUG_TYPE_ERROR)
		//Log::error("GL (%s:%s) %s", typeName, severityName, message);
	else if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
		//Log::warn("GL (%s:%s) %s", typeName, severityName, message);
	else
		//Log::info("GL (%s) %s", typeName, message);
		*/
}

GLuint gl_mesh_assign_attributes(GLuint buffer, GLenum buffer_type, const pxl::VertexFormat& format, GLint divisor)
{
	// bind
	s_gl.BindBuffer(buffer_type, buffer);

	// TODO: disable existing enabled attributes ..
	// ...
	// ...

	// enable attributes
	size_t ptr = 0;
	for (int n = 0; n < format.attributes.size(); n++)
	{
		auto& attribute = format.attributes[n];
		GLenum type = GL_UNSIGNED_BYTE;
		size_t component_size = 0;
		int components = 1;

		if (attribute.type == pxl::VertexType::Float)
		{
			type = GL_FLOAT;
			component_size = 4;
			components = 1;
		}
		else if (attribute.type == pxl::VertexType::Float2)
		{
			type = GL_FLOAT;
			component_size = 4;
			components = 2;
		}
		else if (attribute.type == pxl::VertexType::Float3)
		{
			type = GL_FLOAT;
			component_size = 4;
			components = 3;
		}
		else if (attribute.type == pxl::VertexType::Float4)
		{
			type = GL_FLOAT;
			component_size = 4;
			components = 4;
		}
		else if (attribute.type == pxl::VertexType::Byte4)
		{
			type = GL_BYTE;
			component_size = 1;
			components = 4;
		}
		else if (attribute.type == pxl::VertexType::UByte4)
		{
			type = GL_UNSIGNED_BYTE;
			component_size = 1;
			components = 4;
		}
		else if (attribute.type == pxl::VertexType::Short2)
		{
			type = GL_SHORT;
			component_size = 2;
			components = 2;
		}
		else if (attribute.type == pxl::VertexType::UShort2)
		{
			type = GL_UNSIGNED_SHORT;
			component_size = 2;
			components = 2;
		}
		else if (attribute.type == pxl::VertexType::Short4)
		{
			type = GL_SHORT;
			component_size = 2;
			components = 4;
		}
		else if (attribute.type == pxl::VertexType::UShort4)
		{
			type = GL_UNSIGNED_SHORT;
			component_size = 2;
			components = 4;
		}

		pxl::u32 location = (pxl::u32)(attribute.index);
		s_gl.EnableVertexAttribArray(location);
		s_gl.VertexAttribPointer(location, components, type, attribute.normalized, format.stride, (void*)ptr);
		s_gl.VertexAttribDivisor(location, divisor);

		ptr += components * component_size;
	}

	return format.stride;
}

static GLenum blendOperationToGl(pxl::BlendOperation operation)
{
	switch (operation)
	{
		case pxl::BlendOperation::Add:				return GL_FUNC_ADD;
		case pxl::BlendOperation::Subtract:			return GL_FUNC_SUBTRACT;
		case pxl::BlendOperation::ReverseSubtract:	return GL_FUNC_REVERSE_SUBTRACT;
		case pxl::BlendOperation::Min:				return GL_MIN;
		case pxl::BlendOperation::Max:				return GL_MAX;
	};
	return GL_FUNC_ADD;
}

static GLenum blendFactorToGl(pxl::BlendFactor factor)
{
	switch (factor)
	{
		case pxl::BlendFactor::Zero:					return GL_ZERO;
		case pxl::BlendFactor::One:						return GL_ONE;
		case pxl::BlendFactor::SrcColor:				return GL_SRC_COLOR;
		case pxl::BlendFactor::OneMinusSrcColor:		return GL_ONE_MINUS_SRC_COLOR;
		case pxl::BlendFactor::DstColor:				return GL_DST_COLOR;
		case pxl::BlendFactor::OneMinusDstColor:		return GL_ONE_MINUS_DST_COLOR;
		case pxl::BlendFactor::SrcAlpha:				return GL_SRC_ALPHA;
		case pxl::BlendFactor::OneMinusSrcAlpha:		return GL_ONE_MINUS_SRC_ALPHA;
		case pxl::BlendFactor::DstAlpha:				return GL_DST_ALPHA;
		case pxl::BlendFactor::OneMinusDstAlpha:		return GL_ONE_MINUS_DST_ALPHA;
		case pxl::BlendFactor::ConstantColor:			return GL_CONSTANT_COLOR;
		case pxl::BlendFactor::OneMinusConstantColor:	return GL_ONE_MINUS_CONSTANT_COLOR;
		case pxl::BlendFactor::ConstantAlpha:			return GL_CONSTANT_ALPHA;
		case pxl::BlendFactor::OneMinusConstantAlpha:	return GL_ONE_MINUS_CONSTANT_ALPHA;
		case pxl::BlendFactor::SrcAlphaSaturate:		return GL_SRC_ALPHA_SATURATE;
		case pxl::BlendFactor::Src1Color:				return GL_SRC1_COLOR;
		case pxl::BlendFactor::OneMinusSrc1Color:		return GL_ONE_MINUS_SRC1_COLOR;
		case pxl::BlendFactor::Src1Alpha:				return GL_SRC1_ALPHA;
		case pxl::BlendFactor::OneMinusSrc1Alpha:		return GL_ONE_MINUS_SRC1_ALPHA;
	};

	return GL_ZERO;
}



void pxl::graphics::bind()
{
	s_gl = GLState();
	s_gl.context = pxl::platform::glCreateContext();
	if (s_gl.context == nullptr)
	{
		pxl::log::error("Could not create gl context");
		assert(0);
	}
	pxl::platform::glBindContext(s_gl.context);

#define GL_FUNC(name, ...) s_gl.name = (GLState::name ## Func)(pxl::platform::glGetFunc("gl" #name));
	GL_FUNCTIONS
#undef GL_FUNC

		if (s_gl.DebugMessageCallback != nullptr)
		{
			s_gl.Enable(GL_DEBUG_OUTPUT);
			s_gl.Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			s_gl.DebugMessageCallback(gl_message_callback, nullptr);
		}

	// get opengl info
	s_gl.GetIntegerv(0x8CDF, &s_gl.max_color_attachments);
	s_gl.GetIntegerv(0x80E9, &s_gl.max_element_indices);
	s_gl.GetIntegerv(0x80E8, &s_gl.max_element_vertices);
	s_gl.GetIntegerv(0x84E8, &s_gl.max_renderbuffer_size);
	s_gl.GetIntegerv(0x8D57, &s_gl.max_samples);
	s_gl.GetIntegerv(0x8872, &s_gl.max_texture_image_units);
	s_gl.GetIntegerv(0x0D33, &s_gl.max_texture_size);

	s_gl.PixelStorei(GL_PACK_ALIGNMENT, 1);
	s_gl.PixelStorei(GL_UNPACK_ALIGNMENT, 1);

	s_gl.features.origin_bottom_left = true;
	s_gl.features.max_texture_size = s_gl.max_texture_size;
}

pxl::graphics::GraphicsFeatures pxl::graphics::features()
{
	return s_gl.features;
}

void pxl::graphics::unbind()
{
	pxl::platform::glDestroyContext(s_gl.context);
	s_gl.context = nullptr;
}


void pxl::graphics::clearBackbuffer(const pxl::Color& color)
{
	s_gl.BindFramebuffer(GL_FRAMEBUFFER, 0);
	s_gl.Disable(GL_SCISSOR_TEST);
	s_gl.ColorMask(true, true, true, true);
	s_gl.ClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	s_gl.ClearDepth(0);
	s_gl.ClearStencil(0);
	s_gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}



class GLTexture : public pxl::Texture
{
public:
	GLTexture(int width, int height, pxl::TextureFormat format, bool renderTarget) :
		_id(0), _width(width), _height(height), _formatt(format), _gl_internal_format(GL_RED), _gl_format(GL_RED), _gl_type(GL_UNSIGNED_BYTE)
	{
		_renderTarget = renderTarget;
		assert(width < s_gl.max_texture_size);
		assert(height < s_gl.max_texture_size);
		if (format == pxl::TextureFormat::RGBA)
		{
			_gl_internal_format = GL_RGBA;
			_gl_format = GL_RGBA;
			_gl_type = GL_UNSIGNED_BYTE;
		}
		else
		{
			pxl::log::error("Invalid texture format");
			assert(0);
		}

		s_gl.GenTextures(1, &_id);
		s_gl.ActiveTexture(GL_TEXTURE0);
		s_gl.BindTexture(GL_TEXTURE_2D, _id);
		s_gl.TexImage2D(GL_TEXTURE_2D, 0, _gl_internal_format, width, height, 0, _gl_format, _gl_type, nullptr);
		updateSampling();
	}
	~GLTexture()
	{
		if (_id > 0)
		{
			s_gl.DeleteTextures(1, &_id);
		}
	}
	int width() const override
	{
		return _width;
	}
	int height() const override
	{
		return _height;
	}
	pxl::TextureFormat format() const override
	{
		return _formatt;
	}
	void setData(pxl::u8* data) override
	{
		s_gl.ActiveTexture(GL_TEXTURE0);
		s_gl.BindTexture(GL_TEXTURE_2D, _id);
		s_gl.TexImage2D(GL_TEXTURE_2D, 0, _gl_internal_format, _width, _height, 0, _gl_format, _gl_type, data);
	}
	bool isRenderTarget() const
	{
		return _renderTarget;
	}
	GLuint getId() const
	{
		return _id;
	}
	void setSampler(const pxl::TextureSampler& sampler)
	{
		if (_sampler != sampler)
		{
			_sampler = sampler;
			updateSampling();
		}
	}
	void bind(int slot, const pxl::TextureSampler& sampler)
	{

	}
private:
	void updateSampling()
	{
		s_gl.BindTexture(GL_TEXTURE_2D, _id);
		s_gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (_sampler.filter == pxl::TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR));
		s_gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (_sampler.filter == pxl::TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR));
		s_gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (_sampler.wrap_x == pxl::TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT));
		s_gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (_sampler.wrap_y == pxl::TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT));
	}
	GLuint _id;
	int _width;
	int _height;
	pxl::TextureSampler _sampler;
	pxl::TextureFormat _formatt;
	GLenum _gl_internal_format;
	GLenum _gl_format;
	GLenum _gl_type;
	bool _renderTarget;
};

pxl::TextureRef pxl::graphics::createTexture(int width, int height, TextureFormat format)
{
	auto gltex = new GLTexture(width, height, format, false);
	if (gltex->getId() <= 0)
	{
		delete gltex;
		return nullptr;
	}

	return TextureRef(gltex);
}

class GLShader : public pxl::Shader
{
public:
	GLShader(const pxl::ShaderData& data) : _id(0)
	{
		assert(!data.pixel.empty());
		assert(!data.vertex.empty());

		GLchar log[1024];
		GLsizei log_length = 0;

		GLuint vertex_shader = s_gl.CreateShader(GL_VERTEX_SHADER);
		{
			const GLchar* src = (const GLchar*)data.vertex.cstr();
			s_gl.ShaderSource(vertex_shader, 1, &src, nullptr);
			s_gl.CompileShader(vertex_shader);
			s_gl.GetShaderInfoLog(vertex_shader, 1024, &log_length, log);
			if (log_length > 0)
			{
				s_gl.DeleteShader(vertex_shader);
				pxl::log::error("Vertex shader compilation failed");
				assert(0);
				return;
			}
		}
		GLuint pixel_shader = s_gl.CreateShader(GL_FRAGMENT_SHADER);
		{
			const GLchar* src = (const GLchar*)data.pixel.cstr();
			s_gl.ShaderSource(pixel_shader, 1, &src, nullptr);
			s_gl.CompileShader(pixel_shader);
			s_gl.GetShaderInfoLog(pixel_shader, 1024, &log_length, log);
			if (log_length > 0)
			{
				s_gl.DeleteShader(pixel_shader);
				pxl::log::error("Fragment shader compilation failed");
				assert(0);
				return;
			}
		}

		GLuint id = s_gl.CreateProgram();
		s_gl.AttachShader(id, vertex_shader);
		s_gl.AttachShader(id, pixel_shader);
		s_gl.LinkProgram(id);
		s_gl.GetProgramInfoLog(id, 1024, &log_length, log);

		s_gl.DetachShader(id, vertex_shader);
		s_gl.DetachShader(id, pixel_shader);
		s_gl.DeleteShader(vertex_shader);
		s_gl.DeleteShader(pixel_shader);

		if (log_length > 0)
		{
			pxl::log::error("Shader program link failed");
			assert(0);
			return;
		}

		bool valid_uniforms = true;
		const int max_name_length = 256;
		GLint active_uniforms = 0;
		s_gl.GetProgramiv(id, GL_ACTIVE_UNIFORMS, &active_uniforms);
		for (int i = 0; i < active_uniforms; i++)
		{
			GLsizei length;
			GLsizei size;
			GLenum type;
			GLchar name[max_name_length + 1];
			s_gl.GetActiveUniform(id, i, max_name_length, &length, &size, &type, name);

			// array names end with "[0]", and we don't want that
			for (int n = 0; n < max_name_length; n++)
			{
				if (name[n] == '[')
				{
					if (name[n + 1] == '0' && name[n + 2] == ']')
					{
						name[n] = '\0';
						break;
					}
				}
			}

			name[length] = '\0';

			if (type == GL_SAMPLER_2D)
			{
				pxl::UniformInfo tex_uniform;
				tex_uniform.name = name;
				tex_uniform.buffer_index = 0;
				tex_uniform.array_length = size;
				tex_uniform.type = pxl::UniformType::Texture2D;
				tex_uniform.shader = pxl::ShaderType::Fragment;
				_uniform_locations.add(s_gl.GetUniformLocation(id, name));
				addUniformInfo(tex_uniform);

				pxl::UniformInfo sampler_uniform;
				sampler_uniform.name = pxl::String(name).add("_sampler");
				sampler_uniform.buffer_index = 0;
				sampler_uniform.array_length = size;
				sampler_uniform.type = pxl::UniformType::Sampler2D;
				sampler_uniform.shader = pxl::ShaderType::Fragment;
				_uniform_locations.add(s_gl.GetUniformLocation(id, name));
				addUniformInfo(sampler_uniform);
			}
			else
			{
				pxl::UniformInfo uniform;
				uniform.name = name;
				uniform.type = pxl::UniformType::None;
				uniform.buffer_index = 0;
				uniform.array_length = size;
				_uniform_locations.add(s_gl.GetUniformLocation(id, name));
				uniform.shader = (pxl::ShaderType)((int)pxl::ShaderType::Vertex | (int)pxl::ShaderType::Fragment);

				if (type == GL_FLOAT)
				{
					uniform.type = pxl::UniformType::Float;
				}
				else if (type == GL_FLOAT_VEC2)
				{
					uniform.type = pxl::UniformType::Float2;
				}
				else if (type == GL_FLOAT_VEC3)
				{
					uniform.type = pxl::UniformType::Float3;
				}
				else if (type == GL_FLOAT_VEC4)
				{
					uniform.type = pxl::UniformType::Float4;
				}
				else if (type == GL_FLOAT_MAT3x2)
				{
					uniform.type = pxl::UniformType::Mat3x2;
				}
				else if (type == GL_FLOAT_MAT4)
				{
					uniform.type = pxl::UniformType::Mat4x4;
				}
				else
				{
					pxl::log::error("Invalid uniform type");
					valid_uniforms = false;
					assert(0);
					break;
				}

				addUniformInfo(uniform);
			}
		}
		if (!valid_uniforms)
		{
			s_gl.DeleteProgram(id);
		}
		else
		{
			_id = id;
		}
	}
	~GLShader()
	{
		if (_id > 0)
		{
			s_gl.DeleteProgram(_id);
		}
	}
	GLint getUniformLocation(int idx) const
	{
		return _uniform_locations[idx];
	}
	int getId() const
	{
		return _id;
	}

private:
	pxl::Vector<GLint> _uniform_locations;
	GLuint _id;
};

pxl::ShaderRef pxl::graphics::createShader(const pxl::ShaderData& data)
{
	auto shader = new GLShader(data);
	if (shader->getId() <= 0)
	{
		pxl::log::error("Shader creation failed");
		delete shader;
		assert(0);
		return nullptr;
	}

	return ShaderRef(shader);
}


const pxl::ShaderData s_default_shader = {
		"#version 330\n"
		"uniform mat4 u_matrix;\n"
		"layout(location=0) in vec2 a_position;\n"
		"layout(location=1) in vec2 a_tex;\n"
		"layout(location=2) in vec4 a_color;\n"
		"out vec2 v_tex;\n"
		"out vec4 v_col;\n"
		"void main(void)\n"
		"{\n"
		"	gl_Position = u_matrix * vec4(a_position.xy, 0, 1);\n"
		"	v_tex = a_tex;\n"
		"	v_col = a_color;\n"
		"}",
		"#version 330\n"
		"uniform sampler2D u_texture;\n"
		"in vec2 v_tex;\n"
		"in vec4 v_col;\n"
		"in vec4 v_type;\n"
		"out vec4 o_color;\n"
		"void main(void)\n"
		"{\n"
		"	vec4 color = texture(u_texture, v_tex);\n"
		"	o_color = color * v_col;\n"
		"}"
};

pxl::ShaderRef pxl::graphics::createDefaultShader() {
	return pxl::graphics::createShader(s_default_shader);
}


class GLMesh : public pxl::Mesh
{
public:
	GLMesh() : _id(0), _index_buffer(0), _vertex_buffer(0), _index_count(0), _vertex_count(0), _vertex_size(0), _vertex_attribs_enabled(false), _index_format(0), _index_size(0)
	{
		s_gl.GenVertexArrays(1, &_id);
	}
	~GLMesh()
	{
		if (_vertex_buffer != 0)
		{
			s_gl.DeleteBuffers(1, &_vertex_buffer);
		}

		if (_index_buffer != 0)
		{
			s_gl.DeleteBuffers(1, &_index_buffer);
		}

		if (_id != 0)
		{
			s_gl.DeleteVertexArrays(1, &_id);
		}
		_id = 0;
	}
	GLuint getId() const
	{
		return _id;
	}
	GLenum getIndexFormat() const
	{
		return _index_format;
	}
	int getIndexSize() const
	{
		return _index_size;
	}
	void setIndexData(pxl::IndexFormat format, const void* indices, pxl::i64 count)
	{
		s_gl.BindVertexArray(_id);
		if (_index_buffer == 0)
		{
			s_gl.GenBuffers(1, &_index_buffer);
		}
		if (format == pxl::IndexFormat::UInt16)
		{
			_index_format = GL_UNSIGNED_SHORT;
			_index_size = 2;
		}
		else if (format == pxl::IndexFormat::UInt32)
		{
			_index_format = GL_UNSIGNED_INT;
			_index_size = 4;
		}

		_index_count = count;
		s_gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
		s_gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, _index_size * count, indices, GL_DYNAMIC_DRAW);
		s_gl.BindVertexArray(0);
	}
	void setVertexData(const pxl::VertexFormat& format, const void* vertices, pxl::i64 count)
	{
		s_gl.BindVertexArray(_id);
		if (_vertex_buffer == 0)
		{
			s_gl.GenBuffers(1, &_vertex_buffer);
		}

		_vertex_count = count;
		_vertex_size = gl_mesh_assign_attributes(_vertex_buffer, GL_ARRAY_BUFFER, format, 0);
		s_gl.BindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
		s_gl.BufferData(GL_ARRAY_BUFFER, _vertex_size * count, vertices, GL_DYNAMIC_DRAW);
		s_gl.BindVertexArray(0);
	}
	pxl::i64 getIndexCount() const override
	{
		return _index_count;
	}
	pxl::i64 getVertexCount() const override
	{
		return _vertex_count;
	}
private:
	GLuint _id;
	GLuint _index_buffer;
	GLuint _vertex_buffer;
	pxl::i64 _index_count;
	pxl::i64 _vertex_count;
	pxl::u16 _vertex_size;
	pxl::u8 _vertex_attribs_enabled;
	pxl::Vector<GLuint> _vertex_attribs;
	GLenum _index_format;
	int _index_size;
};

pxl::MeshRef pxl::graphics::createMesh()
{
	auto mesh = new GLMesh();
	if (mesh->getId() <= 0)
	{
		pxl::log::error("Mesh creation failed");
		delete mesh;
		assert(0);
		return nullptr;
	}
	return pxl::MeshRef(mesh);
}

class GLRenderTarget : public pxl::RenderTarget
{
public:

	GLRenderTarget(int width, int height)
	{
		_width = width;
		_height = height;
		s_gl.GenFramebuffers(1, &_id);
		s_gl.BindFramebuffer(GL_FRAMEBUFFER, _id);
		_texture = pxl::TextureRef(new GLTexture(width, height, pxl::TextureFormat::RGBA, true));
		auto gltex = std::dynamic_pointer_cast<GLTexture>(_texture);
		s_gl.FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gltex->getId(), 0);
	}

	~GLRenderTarget()
	{
		if (_id > 0)
		{
			s_gl.DeleteFramebuffers(1, &_id);
			_id = 0;
		}
	}
	pxl::TextureRef texture() const override
	{
		return _texture;
	}
	GLuint getId() const
	{
		return _id;
	}
	void clear(const pxl::Color &color) override
	{
		s_gl.BindFramebuffer(GL_FRAMEBUFFER, _id);
		s_gl.Disable(GL_SCISSOR_TEST);
		s_gl.ColorMask(true, true, true, true);
		s_gl.ClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
		s_gl.ClearDepth(0);
		s_gl.ClearStencil(0);
		s_gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
private:
	GLuint _id;
	int _width;
	int _height;
	pxl::TextureRef _texture;
};

pxl::RenderTargetRef pxl::graphics::createRenderTarget(int width, int height)
{
	auto target = new GLRenderTarget(width, height);
	if (target->getId() <= 0)
	{
		pxl::log::error("Render target creation failed");
		assert(0);
		delete target;
		return nullptr;
	}
	return RenderTargetRef(target);
}

static void setBlendMode(const pxl::BlendState& blend)
{
	auto colorOperation = blendOperationToGl(blend.color_operation);
	auto colorSource = blendFactorToGl(blend.color_source);
	auto colorDestination = blendFactorToGl(blend.color_destination);

	auto alphaOperation = blendOperationToGl(blend.alpha_operation);
	auto alphaSource = blendFactorToGl(blend.alpha_source);
	auto alphaDestination = blendFactorToGl(blend.alpha_destination);

	s_gl.Enable(GL_BLEND);
	s_gl.BlendEquationSeparate(colorOperation, alphaOperation);
	s_gl.BlendFuncSeparate(colorSource, colorDestination, alphaSource, alphaDestination);

	s_gl.ColorMask(
		(int)blend.mask & (int)pxl::BlendMask::Red,
		(int)blend.mask & (int)pxl::BlendMask::Green,
		(int)blend.mask & (int)pxl::BlendMask::Blue,
		(int)blend.mask & (int)pxl::BlendMask::Alpha
	);
}

static void setView(const pxl::RenderTargetRef& renderTarget, const pxl::Rect &viewport)
{
	pxl::Vec2 size;
	if (renderTarget == nullptr)
	{
		s_gl.BindFramebuffer(GL_FRAMEBUFFER, 0);
		size = pxl::platform::drawSize();
	}
	else
	{
		auto frameBuffer = std::dynamic_pointer_cast<GLRenderTarget>(renderTarget);
		s_gl.BindFramebuffer(GL_FRAMEBUFFER, frameBuffer->getId());
		size = pxl::Vec2(renderTarget->width(), renderTarget->height());
	}

	// View
	float usey = size.y - viewport.y - viewport.height;
	s_gl.Viewport((GLint)viewport.x, (GLint)usey, (GLint)viewport.width, (GLint)viewport.height);
}

void pxl::graphics::render(const DrawCall& call)
{
	
	setView(call.target, call.viewport);


	auto shader = std::dynamic_pointer_cast<GLShader>(call.material->shader());
	auto mesh = std::dynamic_pointer_cast<GLMesh>(call.mesh);

	s_gl.UseProgram(shader->getId());
	auto data = call.material->floats();
	int texture_slot = 0;
	int gl_texture_slot = 0;
	GLint texture_ids[64];

	auto& uniforms = shader->uniforms();
	for (int i = 0; i < uniforms.size(); i++)
	{
		auto& uniform = uniforms[i];
		auto location = shader->getUniformLocation(i);

		if (uniform.type == UniformType::Sampler2D) continue;

		if (uniform.type == UniformType::Texture2D)
		{
			for (int n = 0; n < uniform.array_length; n++)
			{
				auto tex = std::dynamic_pointer_cast<GLTexture>(call.material->texture(texture_slot, n));
				auto sampler = call.material->sampler(texture_slot, n);
				s_gl.ActiveTexture(GL_TEXTURE0 + gl_texture_slot);
				if (tex)
				{
					tex->setSampler(sampler);
					s_gl.BindTexture(GL_TEXTURE_2D, tex->getId());
				}
				else
				{
					s_gl.BindTexture(GL_TEXTURE_2D, 0);
				}
				texture_ids[n] = gl_texture_slot;
				gl_texture_slot++;
			}

			s_gl.Uniform1iv(location, (GLint)uniform.array_length, &texture_ids[0]);
			texture_slot++;	
		}
		else if (uniform.type == UniformType::Float)
		{
			s_gl.Uniform1fv(location, (GLint)uniform.array_length, data);
			data += uniform.array_length;
		}
		else if (uniform.type == UniformType::Float2)
		{
			s_gl.Uniform2fv(location, (GLint)uniform.array_length, data);
			data += 2 * uniform.array_length;
		}
		else if (uniform.type == UniformType::Float3)
		{
			s_gl.Uniform3fv(location, (GLint)uniform.array_length, data);
			data += 3 * uniform.array_length;
		}
		else if (uniform.type == UniformType::Float4)
		{
			s_gl.Uniform4fv(location, (GLint)uniform.array_length, data);
			data += 4 * uniform.array_length;
		}
		else if (uniform.type == UniformType::Mat3x2)
		{
			s_gl.UniformMatrix3x2fv(location, (GLint)uniform.array_length, 0, data);
			data += 6 * uniform.array_length;
		}
		else if (uniform.type == UniformType::Mat4x4)
		{
			s_gl.UniformMatrix4fv(location, (GLint)uniform.array_length, 0, data);
			data += 16 * uniform.array_length;
		}
	}
	// Blending

	setBlendMode(call.blend);
	

	s_gl.BindVertexArray(mesh->getId());
	GLenum index_format = mesh->getIndexFormat();
	int index_size = mesh->getIndexSize();
	s_gl.DrawElements(GL_TRIANGLES, (GLint)(call.indices_count), index_format, (void*)(index_size * call.indices_start));
	s_gl.BindVertexArray(0);
}
