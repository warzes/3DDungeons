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

	GLint compiled = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
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

		GLint success = 0;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint errorMsgLen;
			glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &errorMsgLen);

			std::vector<GLchar> errorInfo(static_cast<size_t>(errorMsgLen));
			glGetProgramInfoLog(m_id, errorMsgLen, nullptr, &errorInfo[0]);
			LogError("OPENGL: Shader program linking failed: " + std::string(&errorInfo[0]));
			glDeleteProgram(m_id);
			m_id = 0;
		}
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
int ShaderProgram::GetUniformLocation(const char* name) const
{
	return glGetUniformLocation(m_id, name);
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
void ShaderProgram::SetUniform(int uniformId, const Matrix4& m) const
{
	assert(state::CurrentShaderProgram == m_id);
	glUniformMatrix4fv(uniformId, 1, GL_FALSE, m.m);
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
	{
		const auto& att = attribs[i];
		GLuint loc = (GLuint)(att.location > -1 ? att.location : i);
		glEnableVertexAttribArray(loc);
		glVertexAttribPointer(loc, (GLint)att.size, GL_FLOAT, (GLboolean)(att.normalized ? GL_TRUE : GL_FALSE), (GLsizei)att.stride, att.pointer);
	}
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

		attribs[i].pointer = (void*)+offset;
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