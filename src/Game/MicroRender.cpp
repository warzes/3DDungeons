//=============================================================================
// Header
//=============================================================================
#if defined(_MSC_VER)
#	pragma warning(push, 0)
#	pragma warning(disable : 4514)
#	pragma warning(disable : 5045)
#	pragma warning(disable : 5264)
#endif // _MSC_VER

#include <assert.h>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "MicroRender.h"
#include "MicroMath.h"

#if defined(_MSC_VER)
#	pragma warning(pop)
#	pragma warning(disable : 5045)
#endif // _MSC_VER

// ==> MicroEngine
extern void LogError(const std::string& msg);

// ==> MicroAdvance
namespace ResourceCacheSystem
{
	bool IsLoad(const ShaderProgram& shader);
	bool IsLoad(const Texture2D& texture);
}

//=============================================================================
// Global vars
//=============================================================================
namespace state
{
	unsigned CurrentShaderProgram = 0;
	unsigned CurrentVBO = 0;
	unsigned CurrentIBO = 0;
	unsigned CurrentVAO = 0;
	unsigned CurrentTexture2D[MaxBindingTextures] = { 0 };
	unsigned CurrentTextureCube = 0;

	FrameBuffer* CurrentFrameBuffer = nullptr;
}
//-----------------------------------------------------------------------------
namespace render
{
	int FramebufferWidth = 0;
	int FramebufferHeight = 0;

	Color ClearColor;
	float PerspectiveFOV = 45.0f;
	float PerspectiveNear = 0.01f;
	float PerspectiveFar = 1000.0f;
	Matrix4 ProjectionMatrix;
	Matrix4 OrthoMatrix;
}
//=============================================================================
// Core
//=============================================================================
//-----------------------------------------------------------------------------
inline GLenum translateToGL(RenderResourceUsage usage)
{
	switch (usage)
	{
	case RenderResourceUsage::Static:  return GL_STATIC_DRAW;
	case RenderResourceUsage::Dynamic: return GL_DYNAMIC_DRAW;
	case RenderResourceUsage::Stream:  return GL_STREAM_DRAW;
	}
	return 0;
}
//-----------------------------------------------------------------------------
inline GLenum translateToGL(PrimitiveDraw p)
{
	switch (p)
	{
	case PrimitiveDraw::Lines:     return GL_LINES;
	case PrimitiveDraw::Triangles: return GL_TRIANGLES;
	case PrimitiveDraw::Points:    return GL_POINTS;
	}
	return 0;
}
//-----------------------------------------------------------------------------
inline GLint translateToGL(TextureWrapping wrap)
{
	switch (wrap)
	{
	case TextureWrapping::Repeat:         return GL_REPEAT;
	case TextureWrapping::MirroredRepeat: return GL_MIRRORED_REPEAT;
	case TextureWrapping::Clamp:          return GL_CLAMP;
	case TextureWrapping::ClampToEdge:    return GL_CLAMP_TO_EDGE;
	case TextureWrapping::ClampToBorder:  return GL_CLAMP_TO_BORDER;
	}
	return 0;
}
//-----------------------------------------------------------------------------
inline GLint translateToGL(TextureMinFilter filter)
{
	switch (filter)
	{
	case TextureMinFilter::Nearest:              return GL_NEAREST;
	case TextureMinFilter::Linear:               return GL_LINEAR;
	case TextureMinFilter::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
	case TextureMinFilter::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
	case TextureMinFilter::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
	case TextureMinFilter::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;
	}
	return 0;
}
//-----------------------------------------------------------------------------
inline constexpr GLint translateToGL(TextureMagFilter filter)
{
	switch (filter)
	{
	case TextureMagFilter::Nearest: return GL_NEAREST;
	case TextureMagFilter::Linear:  return GL_LINEAR;
	}
	return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================
// Render States
//=============================================================================
//-----------------------------------------------------------------------------
void SetBlendState(const BlendState& state, float alphaRef)
{
	//if (state == state::CurrentBlendState) return;
	//state::CurrentBlendState = state;

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//=============================================================================
// Shader Program
//=============================================================================
//-----------------------------------------------------------------------------
unsigned createShader(GLenum openGLshaderType, const std::string& source)
{
	const char* shaderText = source.data();
	const GLint lenShaderText = static_cast<GLint>(source.size());
	const GLuint shaderId = glCreateShader(openGLshaderType);
	glShaderSource(shaderId, 1, &shaderText, &lenShaderText);
	glCompileShader(shaderId);

	GLint compileStatus = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
	if ( compileStatus == GL_FALSE)
	{
		GLint infoLogSize;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogSize);
		std::vector<GLchar> errorInfo(static_cast<size_t>(infoLogSize));
		glGetShaderInfoLog(shaderId, (GLsizei)errorInfo.size(), nullptr, &errorInfo[0]);
		glDeleteShader(shaderId);

		std::string shaderName;
		switch (openGLshaderType)
		{
		case GL_VERTEX_SHADER: shaderName = "Vertex "; break;
		case GL_FRAGMENT_SHADER: shaderName = "Fragment "; break;
		}
		LogError(shaderName + "Shader compilation failed : " + std::string(&errorInfo[0]) + ", Source: " + source);
		return 0;
	}

	return shaderId;
}
//-----------------------------------------------------------------------------
bool Uniform::IsReady() const
{
	return IsValid() && m_programId == state::CurrentShaderProgram;
}
//-----------------------------------------------------------------------------
void Uniform::operator=(int value) const
{
	assert(IsReady());
	glUniform1i(m_location, value);
}
//-----------------------------------------------------------------------------
void Uniform::operator=(float value) const
{
	assert(IsReady());
	glUniform1f(m_location, value);
}
//-----------------------------------------------------------------------------
void Uniform::operator=(const Vector2& v) const
{
	assert(IsReady());
	glUniform2fv(m_location, 1, &(v.x));
}
//-----------------------------------------------------------------------------
void Uniform::operator=(const Vector3& v) const
{
	assert(IsReady());
	glUniform3fv(m_location, 1, &(v.x));
}
//-----------------------------------------------------------------------------
void Uniform::operator=(const Matrix3& m) const
{
	assert(IsReady());
	glUniformMatrix3fv(m_location, 1, GL_FALSE, m.DataPtr());
}
//-----------------------------------------------------------------------------
void Uniform::operator=(const Matrix4& m) const
{
	assert(IsReady());
	glUniformMatrix4fv(m_location, 1, GL_FALSE, m.DataPtr());
}
//-----------------------------------------------------------------------------
bool ShaderProgram::CreateFromMemories(const std::string& vertexShaderMemory, const std::string& fragmentShaderMemory)
{
	Destroy();

	const GLuint glShaderVertex = createShader(GL_VERTEX_SHADER, vertexShaderMemory);
	const GLuint glShaderFragment = createShader(GL_FRAGMENT_SHADER, fragmentShaderMemory);
	if (glShaderVertex > 0 && glShaderFragment > 0)
	{
		m_id = glCreateProgram();
		glAttachShader(m_id, glShaderVertex);
		glAttachShader(m_id, glShaderFragment);
		glLinkProgram(m_id);

		GLint linkStatus = 0;
		glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus);
		if ( linkStatus == GL_FALSE )
		{
			GLint errorMsgLen;
			glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &errorMsgLen);

			std::vector<GLchar> errorInfo(static_cast<size_t>(errorMsgLen));
			glGetProgramInfoLog(m_id, errorMsgLen, nullptr, &errorInfo[0]);
			LogError("OPENGL: Shader program linking failed: " + std::string(&errorInfo[0]));
			glDeleteProgram(m_id);
			m_id = 0;
		}
		glDetachShader(m_id, glShaderVertex);
		glDetachShader(m_id, glShaderFragment);
	}
	glDeleteShader(glShaderVertex);
	glDeleteShader(glShaderFragment);

	return IsValid();
}
//-----------------------------------------------------------------------------
void ShaderProgram::Destroy()
{
	if (m_id > 0)
	{
		if (state::CurrentShaderProgram == m_id) UnBind();
		if (!ResourceCacheSystem::IsLoad(*this))
			glDeleteProgram(m_id);
		m_id = 0;
	}
}
//-----------------------------------------------------------------------------
void ShaderProgram::Bind() const
{
	if (state::CurrentShaderProgram == m_id) return;
	state::CurrentShaderProgram = m_id;
	glUseProgram(m_id);
}
//-----------------------------------------------------------------------------
void ShaderProgram::UnBind()
{
	state::CurrentShaderProgram = 0;
	glUseProgram(0);
}
//-----------------------------------------------------------------------------
Uniform ShaderProgram::operator[](const char* uniformName) const
{
	return Uniform(glGetUniformLocation(m_id, uniformName), m_id);
}
//-----------------------------------------------------------------------------
int ShaderProgram::GetUniformLocation(const char* uniformName) const
{
	return glGetUniformLocation(m_id, uniformName);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetSampler(int uniformId, int value) const
{
	assert(state::CurrentShaderProgram == m_id);
	glUniform1i(uniformId, value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(int uniformId, int value) const
{
	assert(state::CurrentShaderProgram == m_id);
	glUniform1i(uniformId, value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(int uniformId, float value) const
{
	assert(state::CurrentShaderProgram == m_id);
	glUniform1f(uniformId, value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(int uniformId, const Vector2& v) const
{
	assert(state::CurrentShaderProgram == m_id);
	glUniform2fv(uniformId, 1, &(v.x));
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(int uniformId, const Vector3& v) const
{
	assert(state::CurrentShaderProgram == m_id);
	glUniform3fv(uniformId, 1, &(v.x));
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(int uniformId, const Matrix3& m) const
{
	assert(state::CurrentShaderProgram == m_id);
	glUniformMatrix3fv(uniformId, 1, GL_FALSE, m.DataPtr());
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(int uniformId, const Matrix4& m) const
{
	assert(state::CurrentShaderProgram == m_id);
	glUniformMatrix4fv(uniformId, 1, GL_FALSE, m.DataPtr());
}
//-----------------------------------------------------------------------------
std::vector<ShaderAttribInfo> ShaderProgram::GetAttribInfo() const
{
	if (!IsValid()) return {};

	int activeAttribs = 0;
	glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &activeAttribs);
	int maxLength = 0;
	glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

	std::string name;
	name.resize(static_cast<size_t>(maxLength));

	std::vector<ShaderAttribInfo> attribs;
	attribs.reserve(static_cast<size_t>(activeAttribs));
	for (int i = 0; i < activeAttribs; i++)
	{
		GLint size;
		GLenum type = 0;
		glGetActiveAttrib(m_id, (GLuint)i, maxLength, nullptr, &size, &type, name.data());

		ShaderAttribInfo attrib = {
			.typeId = type,
			.name = name,
			.location = glGetAttribLocation(m_id, name.c_str())
		};
		attribs.emplace_back(std::move(attrib));
	}

	std::sort(attribs.begin(), attribs.end(), [](const ShaderAttribInfo& a, const ShaderAttribInfo& b) {return a.location < b.location; });

	return attribs;
}
//-----------------------------------------------------------------------------
//=============================================================================
// Vertex Buffer
//=============================================================================
//-----------------------------------------------------------------------------
bool VertexBuffer::Create(RenderResourceUsage usage, unsigned vertexCount, unsigned vertexSize, const void* data)
{
	Destroy();

	m_vertexCount = vertexCount;
	m_vertexSize = vertexSize;
	m_usage = usage;

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, data, translateToGL(m_usage));
	glBindBuffer(GL_ARRAY_BUFFER, state::CurrentVBO); // restore current vb

	return true;
}
//-----------------------------------------------------------------------------
void VertexBuffer::Destroy()
{
	if (m_id > 0)
	{
		if (state::CurrentVBO == m_id) state::CurrentVBO = 0;
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
}
//-----------------------------------------------------------------------------
void VertexBuffer::Update(unsigned offset, unsigned vertexCount, unsigned vertexSize, const void* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);

	if (m_vertexCount != vertexCount || m_vertexSize != vertexSize || m_usage != RenderResourceUsage::Dynamic)
	{
		glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, data, translateToGL(RenderResourceUsage::Dynamic));
		m_usage = RenderResourceUsage::Dynamic;
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, vertexCount * vertexSize, data);
	}
	m_vertexCount = vertexCount;
	m_vertexSize = vertexSize;

	glBindBuffer(GL_ARRAY_BUFFER, state::CurrentVBO); // restore current vb
}
//-----------------------------------------------------------------------------
void VertexBuffer::Bind() const
{
	if (state::CurrentVBO == m_id) return;
	state::CurrentVBO = m_id;
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}
//-----------------------------------------------------------------------------
//=============================================================================
// Index Buffer
//=============================================================================
//-----------------------------------------------------------------------------
bool IndexBuffer::Create(RenderResourceUsage usage, unsigned indexCount, unsigned indexSize, const void* data)
{
	Destroy();

	m_indexCount = indexCount;
	m_indexSize = indexSize;
	m_usage = usage;

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, data, translateToGL(m_usage));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state::CurrentIBO); // restore current ib
	return true;
}
//-----------------------------------------------------------------------------
void IndexBuffer::Destroy()
{
	if (m_id)
	{
		if (state::CurrentIBO == m_id) state::CurrentIBO = 0;
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
}
//-----------------------------------------------------------------------------
void IndexBuffer::Update(unsigned offset, unsigned indexCount, unsigned indexSize, const void* data)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	if (m_indexCount != indexCount || m_indexSize != indexSize || m_usage != RenderResourceUsage::Dynamic)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, data, translateToGL(RenderResourceUsage::Dynamic));
		m_usage = RenderResourceUsage::Dynamic;
	}
	else
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, indexCount * indexSize, data);
	}
	m_indexCount = indexCount;
	m_indexSize = indexSize;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state::CurrentIBO); // restore current ib
}
//-----------------------------------------------------------------------------
void IndexBuffer::Bind() const
{
	if (state::CurrentIBO == m_id) return;
	state::CurrentIBO = m_id;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}
//-----------------------------------------------------------------------------
//=============================================================================
// Vertex Array Buffer
//=============================================================================
//-----------------------------------------------------------------------------
void VertexAttribute::Bind(unsigned loc) const
{
	const GLuint oglLocation = static_cast<GLuint>(location > -1 ? location : loc);
	glEnableVertexAttribArray(oglLocation);
	glVertexAttribPointer(oglLocation, size, GL_FLOAT, (GLboolean)(normalized ? GL_TRUE : GL_FALSE), stride, offset);
}
//-----------------------------------------------------------------------------
bool VertexArrayBuffer::Create(VertexBuffer* vbo, IndexBuffer* ibo, const std::vector<VertexAttribute>& attribs)
{
	if (!vbo || attribs.empty()) return false;

	Destroy();

	m_vbo = vbo;
	m_ibo = ibo;

	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);

	vbo->Bind();
	for (size_t i = 0; i < attribs.size(); i++)
		attribs[i].Bind(i);

	m_attribsCount = (unsigned)attribs.size();

	if (m_ibo) m_ibo->Bind();

	glBindVertexArray(state::CurrentVAO); // restore VAO
	// TODO: нужно ли ресторить vbo и ibo?
	return true;
}
//-----------------------------------------------------------------------------
bool VertexArrayBuffer::Create(VertexBuffer* vbo, IndexBuffer* ibo, ShaderProgram* shaders)
{
	if (!shaders || !shaders->IsValid()) return false;

	auto attribInfo = shaders->GetAttribInfo();
	if (attribInfo.empty()) return false;

	size_t offset = 0;
	std::vector<VertexAttribute> attribs(attribInfo.size());
	for (size_t i = 0; i < attribInfo.size(); i++)
	{
		attribs[i].location = attribInfo[i].location;
		attribs[i].normalized = false;

		size_t sizeType = 0;
		switch (attribInfo[i].typeId)
		{
		case GL_FLOAT:
			attribs[i].size = 1;
			sizeType = attribs[i].size * sizeof(float);
			break;
		case GL_FLOAT_VEC2:
			attribs[i].size = 2;
			sizeType = attribs[i].size * sizeof(float);
			break;
		case GL_FLOAT_VEC3:
			attribs[i].size = 3;
			sizeType = attribs[i].size * sizeof(float);
			break;
		case GL_FLOAT_VEC4:
			attribs[i].size = 4;
			sizeType = attribs[i].size * sizeof(float);
			break;

		default:
			LogError("Shader attribute type: " + attribInfo[i].name + " not support!");
			return false;
		}

		attribs[i].offset = (void*)+offset;
		offset += sizeType;
	}
	for (size_t i = 0; i < attribs.size(); i++)
	{
		attribs[i].stride = (unsigned int)offset;
	}

	return Create(vbo, ibo, attribs);
}
//-----------------------------------------------------------------------------
void VertexArrayBuffer::Destroy()
{
	if (m_id > 0)
	{
		if (state::CurrentVAO == m_id)
			UnBind();

		glDeleteVertexArrays(1, &m_id);
		m_id = 0;
	}
}
//-----------------------------------------------------------------------------
void VertexArrayBuffer::UnBind()
{
	state::CurrentVAO = 0;
	state::CurrentVBO = 0;
	state::CurrentIBO = 0;
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//-----------------------------------------------------------------------------
void VertexArrayBuffer::Draw(PrimitiveDraw primitive)
{
	if (state::CurrentVAO != m_id)
	{
		state::CurrentVAO = m_id;
		glBindVertexArray(m_id);
		m_vbo->Bind();
		if (m_ibo) m_ibo->Bind();
	}

	if (m_ibo)
	{
		const GLenum indexSizeType = (GLenum)(m_ibo->GetIndexSize() == sizeof(uint32_t) ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT);
		glDrawElements(translateToGL(primitive), (GLsizei)m_ibo->GetIndexCount(), indexSizeType, nullptr);

	}
	else
	{
		glDrawArrays(translateToGL(primitive), 0, (GLsizei)m_vbo->GetVertexCount());
	}
}
//-----------------------------------------------------------------------------
void VertexArrayBuffer::DrawNoCache(PrimitiveDraw primitive)
{
	state::CurrentVAO = 0;
	state::CurrentVBO = 0;
	state::CurrentIBO = 0;
	Draw(primitive);
	state::CurrentVAO = 0;
	state::CurrentVBO = 0;
	state::CurrentIBO = 0;
}
//-----------------------------------------------------------------------------
//=============================================================================
// Texture 2D
//=============================================================================
//-----------------------------------------------------------------------------
inline bool getTextureFormatType(TexelsFormat inFormat, GLenum textureType, GLenum& format, GLint& internalFormat, GLenum& oglType)
{
	if (inFormat == TexelsFormat::R_U8)
	{
		format = GL_RED;
		internalFormat = GL_R8;
		oglType = GL_UNSIGNED_BYTE;
	}
	else if (inFormat == TexelsFormat::RG_U8)
	{
		format = GL_RG;
		internalFormat = GL_RG8;
		oglType = GL_UNSIGNED_BYTE;
		const GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
		glTexParameteriv(textureType, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask); // TODO: могут быть проблемы с браузерами, тогда только грузить stb с указанием нужного формата
	}
	else if (inFormat == TexelsFormat::RGB_U8)
	{
		format = GL_RGB;
		internalFormat = GL_RGB8;
		oglType = GL_UNSIGNED_BYTE;
	}
	else if (inFormat == TexelsFormat::RGBA_U8)
	{
		format = GL_RGBA;
		internalFormat = GL_RGBA8;
		oglType = GL_UNSIGNED_BYTE;
	}
	else if (inFormat == TexelsFormat::RGBA_UINT8888Rev )
	{
		format = GL_RGBA;
		internalFormat = GL_RGBA;
		oglType = GL_UNSIGNED_INT_8_8_8_8_REV;
	}
	else if (inFormat == TexelsFormat::Depth_U16)
	{
		format = GL_DEPTH_COMPONENT;
		internalFormat = GL_DEPTH_COMPONENT16;
		oglType = GL_UNSIGNED_SHORT;
	}
	else if (inFormat == TexelsFormat::DepthStencil_U16)
	{
		format = GL_DEPTH_STENCIL;
		internalFormat = GL_DEPTH24_STENCIL8;
		oglType = GL_UNSIGNED_SHORT;
	}
	else if (inFormat == TexelsFormat::Depth_U24)
	{
		format = GL_DEPTH_COMPONENT;
		internalFormat = GL_DEPTH_COMPONENT24;
		oglType = GL_UNSIGNED_INT;
	}
	else if (inFormat == TexelsFormat::DepthStencil_U24)
	{
		format = GL_DEPTH_STENCIL;
		internalFormat = GL_DEPTH24_STENCIL8;
		oglType = GL_UNSIGNED_INT;
	}
	else
	{
		LogError("unknown texture format");
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------------
bool Texture2D::Create(const char* fileName, const Texture2DInfo& textureInfo)
{
	//stbi_set_flip_vertically_on_load(verticallyFlip ? 1 : 0);

	const int desiredСhannels = STBI_default;
	int width = 0;
	int height = 0;
	int nrChannels = 0;
	stbi_uc* pixelData = stbi_load(fileName, &width, &height, &nrChannels, desiredСhannels);
	if (!pixelData || nrChannels < STBI_grey || nrChannels > STBI_rgb_alpha || width == 0 || height == 0)
	{
		LogError("Image loading failed! Filename='" + std::string(fileName) + "'");
		stbi_image_free((void*)pixelData);
		return false;
	}

	const size_t imageDataSize = (size_t)width * height * nrChannels;

	bool IsTransparent = false;
	// TODO: может быть медленно, проверить скорость и поискать другое решение
	if (nrChannels == STBI_rgb_alpha) // TODO: сделать еще и для 2
	{
		for (size_t i = 0; i < imageDataSize-4; i += 4)
		{
			//uint8_t r = tempImage[i];
			//uint8_t g = tempImage[i + 1];
			//uint8_t b = tempImage[i + 2];
			const uint8_t& a = pixelData[i + 3];
			if (a < 255)
			{
				IsTransparent = true;
				break;
			}
		}
	}

	Texture2DCreateInfo createInfo;
	{
		createInfo.isTransparent = IsTransparent;
		
		if (nrChannels == STBI_grey) createInfo.format = TexelsFormat::R_U8;
		else if (nrChannels == STBI_grey_alpha) createInfo.format = TexelsFormat::RG_U8;
		else if (nrChannels == STBI_rgb) createInfo.format = TexelsFormat::RGB_U8;
		else if (nrChannels == STBI_rgb_alpha) createInfo.format = TexelsFormat::RGBA_U8;

		createInfo.width = static_cast<uint16_t>(width);
		createInfo.height = static_cast<uint16_t>(height);
		createInfo.pixelData = pixelData;
	}

	bool ret = Create(createInfo, textureInfo);
	stbi_image_free((void*)pixelData);
	return ret;
}
//-----------------------------------------------------------------------------
bool Texture2D::Create(const Texture2DCreateInfo& createInfo, const Texture2DInfo& textureInfo)
{
	Destroy();

	isTransparent = createInfo.isTransparent;
	m_width = createInfo.width;
	m_height = createInfo.height;

	// gen texture res
	glGenTextures(1, &m_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_id);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, translateToGL(textureInfo.wrapS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, translateToGL(textureInfo.wrapT));

	// set texture filtering parameters
	TextureMinFilter minFilter = textureInfo.minFilter;
	if (!textureInfo.mipmap)
	{
		if (textureInfo.minFilter == TextureMinFilter::NearestMipmapNearest) minFilter = TextureMinFilter::Nearest;
		else if (textureInfo.minFilter != TextureMinFilter::Nearest) minFilter = TextureMinFilter::Linear;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, translateToGL(minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, translateToGL(textureInfo.magFilter));

	// set texture format
	m_format = createInfo.format;
	GLenum format = GL_RGB;
	GLint internalFormat = GL_RGB;
	GLenum oglType = GL_UNSIGNED_BYTE;
	getTextureFormatType(createInfo.format, GL_TEXTURE_2D, format, internalFormat, oglType);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei)m_width, (GLsizei)m_height, 0, format, oglType, createInfo.pixelData);

	if (textureInfo.mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);

	// restore prev state
	glBindTexture(GL_TEXTURE_2D, state::CurrentTexture2D[0]);

	return true;
}
//-----------------------------------------------------------------------------
void Texture2D::Destroy()
{
	if (m_id > 0)
	{
		for (unsigned i = 0; i < MaxBindingTextures; i++)
		{
			if (state::CurrentTexture2D[i] == m_id)
				Texture2D::UnBind(i);
		}
		if (!ResourceCacheSystem::IsLoad(*this))
			glDeleteTextures(1, &m_id);
		m_id = 0;
	}
}
//-----------------------------------------------------------------------------
void Texture2D::Bind(unsigned slot) const
{
	if (state::CurrentTexture2D[slot] == m_id) return;
	state::CurrentTexture2D[slot] = m_id;
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}
//-----------------------------------------------------------------------------
void Texture2D::BindUnCache(unsigned slot) const
{
	state::CurrentTexture2D[slot] = 0;
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}
//-----------------------------------------------------------------------------
void Texture2D::UnBind(unsigned slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
	state::CurrentTexture2D[slot] = 0;
}
//-----------------------------------------------------------------------------
void Texture2D::UnBindAll()
{
	for (unsigned i = 0; i < MaxBindingTextures; i++)
		UnBind(i);
}
//-----------------------------------------------------------------------------
void Texture2D::SetData(uint8_t* pixelData)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_id);

	GLenum format = GL_RGB;
	GLint internalFormat = GL_RGB;
	GLenum oglType = GL_UNSIGNED_BYTE;
	getTextureFormatType(m_format, GL_TEXTURE_2D, format, internalFormat, oglType);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, format, oglType, pixelData);

	glBindTexture(GL_TEXTURE_2D, state::CurrentTexture2D[0]);
}
//-----------------------------------------------------------------------------
//=============================================================================
// Texture Cube
//=============================================================================
//-----------------------------------------------------------------------------
bool TextureCube::Create(const char* fileNameRight, const char* fileNameLeft, const char* fileNameBottom, const char* fileNameTop, const char* fileNameFront, const char* fileNameBack, const TextureCubeInfo& textureInfo)
{
	Destroy();

	const std::vector<const char*> fileNames = { fileNameRight , fileNameLeft, fileNameBottom, fileNameTop, fileNameFront, fileNameBack };

	std::vector<uint8_t> pixels[6];

	for( int i = 0; i < 6; i++ )
	{
		stbi_set_flip_vertically_on_load(1);

		const int desiredСhannels = STBI_rgb; // Конвертировать в RGB
		int width = 0;
		int height = 0;
		int nrChannels = 0;
		stbi_uc* pixelData = stbi_load(fileNames[i], &width, &height, &nrChannels, desiredСhannels);
		if( !pixelData || width == 0 || height == 0 )
		{
			LogError("Image loading failed! Filename='" + std::string(fileNames[i]) + "'");
			stbi_image_free((void*)pixelData);
			return false;
		}
		const size_t imageDataSize = (size_t)width * height * nrChannels;

		pixels[i].assign(pixelData, pixelData + imageDataSize);
		m_width = Max(m_width, width);
		m_height = Max(m_height, height);
		stbi_image_free((void*)pixelData);
	}

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, translateToGL(textureInfo.wrapS));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, translateToGL(textureInfo.wrapT));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, translateToGL(textureInfo.wrapR));

	// set texture filtering parameters
	TextureMinFilter minFilter = textureInfo.minFilter;
	if( !textureInfo.mipmap )
	{
		if( textureInfo.minFilter == TextureMinFilter::NearestMipmapNearest ) minFilter = TextureMinFilter::Nearest;
		else if( textureInfo.minFilter != TextureMinFilter::Nearest ) minFilter = TextureMinFilter::Linear;
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, translateToGL(minFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, translateToGL(textureInfo.magFilter));

	for( int i = 0; i < 6; i++ )
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels[i].data());
	}

	if( textureInfo.mipmap )
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// restore prev state
	glBindTexture(GL_TEXTURE_CUBE_MAP, state::CurrentTextureCube);


	return false;
}
//-----------------------------------------------------------------------------
void TextureCube::Destroy()
{
	if( m_id > 0 )
	{
		if( state::CurrentTextureCube == m_id )
			TextureCube::UnBind();

		glDeleteTextures(1, &m_id);
		m_id = 0;
	}
}
//-----------------------------------------------------------------------------
void TextureCube::Bind() const
{
	if( state::CurrentTextureCube == m_id ) return;
	state::CurrentTextureCube = m_id;
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
}
//-----------------------------------------------------------------------------
void TextureCube::UnBind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	state::CurrentTextureCube = 0;
}
//-----------------------------------------------------------------------------
//=============================================================================
// FrameBuffer
//=============================================================================
//-----------------------------------------------------------------------------
bool FrameBuffer::Create(int width, int height)
{
	if( width < 1 || height < 1 ) return false;
	m_width = width;
	m_height = height;
	glGenFramebuffers(1, &m_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	glGenTextures(1, &m_texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_texColorBuffer);
#if 1
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
#else
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, nullptr);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // TODO: GL_LINEAR 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	Texture2D::UnBind(); // TODO:

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColorBuffer, 0);

	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	if( !checkFramebuffer() )
	{
		LogError("Framebuffer is not complete!");
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	state::CurrentFrameBuffer = nullptr;

	return true;
}
//-----------------------------------------------------------------------------
void FrameBuffer::Destroy()
{
	if( state::CurrentFrameBuffer == this ) MainFrameBufferBind();

	glDeleteTextures(1, &m_texColorBuffer);
	glDeleteRenderbuffers(1, &m_rbo);
	glDeleteFramebuffers(1, &m_id);
}
//-----------------------------------------------------------------------------
void FrameBuffer::Bind(const Vector3& color)
{
	if( state::CurrentFrameBuffer != this )
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		glViewport(0, 0, m_width, m_height);
		state::CurrentFrameBuffer = this;
	}
	glClearColor(color.x, color.y, color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//-----------------------------------------------------------------------------
void FrameBuffer::MainFrameBufferBind()
{
	if( state::CurrentFrameBuffer ) glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, GetRenderWidth(), GetRenderHeight()); // TODO:
	//glClearColor(RendererState::ClearColor.x, RendererState::ClearColor.y, RendererState::ClearColor.z, 1.0f); // TODO:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	state::CurrentFrameBuffer = nullptr;
}
//-----------------------------------------------------------------------------
void FrameBuffer::BindTextureBuffer()
{
	Texture2D::UnBind(); // TODO:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texColorBuffer);
}
//-----------------------------------------------------------------------------
bool FrameBuffer::checkFramebuffer()
{
	const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if( status == GL_FRAMEBUFFER_COMPLETE )
		return true;

	std::string strStatus = "";
	switch( status )
	{
	case GL_FRAMEBUFFER_UNDEFINED: strStatus = "GL_FRAMEBUFFER_UNDEFINED"; break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: strStatus = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: strStatus = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: strStatus = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: strStatus = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
	case GL_FRAMEBUFFER_UNSUPPORTED: strStatus = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: strStatus = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: strStatus = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
	default: strStatus = "UNKNOWN"; break;
	}
	LogError("OpenGL Error = " + strStatus);

	return false;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//=============================================================================
// Render System
//=============================================================================
//-----------------------------------------------------------------------------
#if defined(_DEBUG)
void openglDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/) noexcept
{
	// Ignore non-significant error/warning codes (NVidia drivers)
	// NOTE: Here there are the details with a sample output:
	// - #131169 - Framebuffer detailed info: The driver allocated storage for renderbuffer 2. (severity: low)
	// - #131185 - Buffer detailed info: Buffer object 1 (bound to GL_ELEMENT_ARRAY_BUFFER_ARB, usage hint is GL_ENUM_88e4)
	//             will use VIDEO memory as the source for buffer object operations. (severity: low)
	// - #131218 - Program/shader state performance warning: Vertex shader in program 7 is being recompiled based on GL state. (severity: medium)
	// - #131204 - Texture state usage warning: The texture object (0) bound to texture image unit 0 does not have
	//             a defined base level and cannot be used for texture mapping. (severity: low)
	if( (id == 131169) || (id == 131185) || (id == 131218) || (id == 131204) ) return;

	std::string msgSource;
	switch( source )
	{
	case 0x8246/*GL_DEBUG_SOURCE_API*/:             msgSource = "API"; break;
	case 0x8247/*GL_DEBUG_SOURCE_WINDOW_SYSTEM*/:   msgSource = "WINDOW_SYSTEM"; break;
	case 0x8248/*GL_DEBUG_SOURCE_SHADER_COMPILER*/: msgSource = "SHADER_COMPILER"; break;
	case 0x8249/*GL_DEBUG_SOURCE_THIRD_PARTY*/:     msgSource = "THIRD_PARTY"; break;
	case 0x824A/*GL_DEBUG_SOURCE_APPLICATION*/:     msgSource = "APPLICATION"; break;
	case 0x824B/*GL_DEBUG_SOURCE_OTHER*/:           msgSource = "OTHER"; break;
	default: break;
	}

	std::string msgType;
	switch( type )
	{
	case 0x824C/*GL_DEBUG_TYPE_ERROR*/:               msgType = "ERROR"; break;
	case 0x824D/*GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR*/: msgType = "DEPRECATED_BEHAVIOR"; break;
	case 0x824E/*GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR*/:  msgType = "UNDEFINED_BEHAVIOR"; break;
	case 0x824F/*GL_DEBUG_TYPE_PORTABILITY*/:         msgType = "PORTABILITY"; break;
	case 0x8250/*GL_DEBUG_TYPE_PERFORMANCE*/:         msgType = "PERFORMANCE"; break;
	case 0x8268/*GL_DEBUG_TYPE_MARKER*/:              msgType = "MARKER"; break;
	case 0x8269/*GL_DEBUG_TYPE_PUSH_GROUP*/:          msgType = "PUSH_GROUP"; break;
	case 0x826A/*GL_DEBUG_TYPE_POP_GROUP*/:           msgType = "POP_GROUP"; break;
	case 0x8251/*GL_DEBUG_TYPE_OTHER*/:               msgType = "OTHER"; break;
	default: break;
	}

	std::string msgSeverity = "DEFAULT";
	switch( severity )
	{
	case 0x9148/*GL_DEBUG_SEVERITY_LOW*/:          msgSeverity = "LOW"; break;
	case 0x9147/*GL_DEBUG_SEVERITY_MEDIUM*/:       msgSeverity = "MEDIUM"; break;
	case 0x9146/*GL_DEBUG_SEVERITY_HIGH*/:         msgSeverity = "HIGH"; break;
	case 0x826B/*GL_DEBUG_SEVERITY_NOTIFICATION*/: msgSeverity = "NOTIFICATION"; break;
	default: break;
	}

	LogError("GL: OpenGL debug message: " + std::string(message));
	LogError("    > Type: " + msgType);
	LogError("    > Source: " + msgSource);
	LogError("    > Severity: " + msgSeverity);
}
#endif // _DEBUG
//-----------------------------------------------------------------------------
void RenderSystemInit()
{
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glDepthRange(0.0f, 1.0f);
	glClearColor(0.2f, 0.4f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
//-----------------------------------------------------------------------------
void RenderSystemBeginFrame(int WindowClientWidth, int WindowClientHeight)
{
	if( render::FramebufferWidth != WindowClientWidth || render::FramebufferHeight != WindowClientHeight )
	{
		render::FramebufferWidth = WindowClientWidth;
		render::FramebufferHeight = WindowClientHeight;

		glViewport(0, 0, render::FramebufferWidth, render::FramebufferHeight);
		glScissor(0, 0, render::FramebufferWidth, render::FramebufferHeight);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
//-----------------------------------------------------------------------------