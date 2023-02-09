#pragma once

//=============================================================================
// Header
//=============================================================================
#if defined(_MSC_VER)
#	pragma warning(push, 0)
#	pragma warning(disable : 5264)
#endif // _MSC_VER

#include <string>
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // _MSC_VER

#include "MicroOpenGLLoader.h"

class Vector2;
class Vector3;
class Matrix3;
class Matrix4;

//=============================================================================
// Render Config
//=============================================================================
constexpr int MaxBindingTextures = 16;

//=============================================================================
// Core
//=============================================================================

enum class RenderResourceUsage
{
	Static,
	Dynamic,
	Stream,
};

//=============================================================================
// Render States
//=============================================================================

enum ColorWriteMask
{
	ColorWriteA = 1,
	ColorWriteB = 2,
	ColorWriteG = 4,
	ColorWriteR = 8,
	ColorWriteAll = (ColorWriteR | ColorWriteG | ColorWriteB | ColorWriteA)
};

enum class StencilOp
{
	Keep,
	Zero,
	Replace,
	IncrSat,
	DecrSat,
	Invert,
	IncrWrap,
	DecrWrap
};

enum class BlendOp
{
	Add,
	Sub,
	RevSub,
	Min,
	Max,
	LogicalClear,
	LogicalSet,
	LogicalCopy,
	LogicalCopyInverted,
	LogicalNoop,
	LogicalInvert,
	LogicalAnd,
	LogicalNand,
	LogicalOr,
	LogicalNor,
	LogicalXor,
	LogicalEquiv,
	LogicalAndReverse,
	LogicalAndInverted,
	LogicalOrReverse,
	LogicalOrInverted,
};

enum class BlendMode
{
	Zero,
	One,
	DstColor,
	SrcColor,
	OneMinusDstColor,
	SrcAlpha,
	OneMinusSrcColor,
	DstAlpha,
	OneMinusDstAlpha,
	SrcAlphaSaturate,
	OneMinusSrcAlpha
};

enum class CompareFunction
{
	Disabled,
	Never,
	Less,
	Equal,
	LEqual,
	Greater,
	NotEqual,
	GEqual,
	Always,
};

enum class CullMode
{
	Full,
	Front,
	Back,
};

struct BlendState
{
	BlendMode       srcBlend = BlendMode::One;
	BlendMode       dstBlend = BlendMode::Zero;
	BlendMode       srcBlendAlpha = BlendMode::One;
	BlendMode       dstBlendAlpha = BlendMode::Zero;
	BlendOp         blendOp = BlendOp::Add;
	BlendOp         blendOpAlpha = BlendOp::Add;
	uint32_t        renderTargetWriteMask = ColorWriteAll;
	CompareFunction alphaTest = CompareFunction::Disabled;
	bool            alphaToMask = false;
};

void SetBlendState(const BlendState& state, float alphaRef);

//=============================================================================
// Shader Program
//=============================================================================

struct ShaderAttribInfo
{
	unsigned typeId;
	std::string name;
	int location;
};

class ShaderProgram
{
public:
	[[nodiscard]] bool CreateFromMemories(const std::string& vertexShaderMemory, const std::string& fragmentShaderMemory);
	void Destroy();

	void Bind() const;
	static void UnBind();

	[[nodiscard]] int GetUniformLocation(const char* name) const;
	void SetUniform(int uniformId, float value) const;
	void SetUniform(int uniformId, const Vector2& v) const;
	void SetUniform(int uniformId, const Vector3& v) const;
	void SetUniform(int uniformId, const Matrix3& m) const;
	void SetUniform(int uniformId, const Matrix4& m) const;

	[[nodiscard]] bool IsValid() const { return m_id > 0; }

	[[nodiscard]] std::vector<ShaderAttribInfo> GetAttribInfo() const;

private:
	unsigned m_id = 0;
};

//=============================================================================
// Vertex Buffer
//=============================================================================

class VertexBuffer
{
public:
	[[nodiscard]] bool Create(RenderResourceUsage usage, unsigned vertexCount, unsigned vertexSize, const void* data);
	void Destroy();

	void Update(unsigned offset, unsigned vertexCount, unsigned vertexSize, const void* data);

	void Bind() const;

	[[nodiscard]] unsigned GetVertexCount() const { return m_vertexCount; }

	[[nodiscard]] bool IsValid() const { return m_id > 0; }

private:
	RenderResourceUsage m_usage = RenderResourceUsage::Static;
	unsigned m_id = 0;
	unsigned m_vertexCount = 0;
	unsigned m_vertexSize = 0;
};

//=============================================================================
// Index Buffer
//=============================================================================

class IndexBuffer
{
public:
	[[nodiscard]] bool Create(RenderResourceUsage usage, unsigned indexCount, unsigned indexSize, const void* data);
	void Destroy();

	void Bind() const;

	void Update(unsigned offset, unsigned indexCount, unsigned indexSize, const void* data);

	[[nodiscard]] unsigned GetIndexCount() const { return m_indexCount; }
	[[nodiscard]] unsigned GetIndexSize() const { return m_indexSize; }

	[[nodiscard]] bool IsValid() const { return m_id > 0; }

private:
	RenderResourceUsage m_usage = RenderResourceUsage::Static;
	unsigned m_id = 0;
	unsigned m_indexCount = 0;
	unsigned m_indexSize = 0;
};

//=============================================================================
// Vertex Array Buffer
//=============================================================================

enum class PrimitiveDraw
{
	Lines,
	Triangles,
	Points,
};

struct VertexAttribute
{
	int location = -1;   // если -1, то берется индекс массива атрибутов
	unsigned size;
	bool normalized;
	unsigned stride;     // sizeof Vertex
	const void* pointer; // (void*)offsetof(Vertex, TexCoord)}
};

class VertexArrayBuffer
{
public:
	[[nodiscard]] bool Create(VertexBuffer* vbo, IndexBuffer* ibo, const std::vector<VertexAttribute>& attribs);
	[[nodiscard]] bool Create(VertexBuffer* vbo, IndexBuffer* ibo, ShaderProgram* shaders);
	void Destroy();

	static void UnBind();

	void Draw(PrimitiveDraw primitive = PrimitiveDraw::Triangles);

	[[nodiscard]] bool IsValid() const { return m_id > 0; }

	[[nodiscard]] VertexBuffer* GetVertexBuffer() { return m_vbo; }
	[[nodiscard]] IndexBuffer* GetIndexBuffer() { return m_ibo; }
private:
	unsigned m_id = 0;
	VertexBuffer* m_vbo = nullptr;
	IndexBuffer* m_ibo = nullptr;
	unsigned m_attribsCount = 0;
};

//=============================================================================
// Texture 2D
//=============================================================================

enum class TextureMinFilter
{
	Nearest,
	Linear,
	NearestMipmapNearest,
	NearestMipmapLinear,
	LinearMipmapNearest,
	LinearMipmapLinear,
};

enum class TextureMagFilter
{
	Nearest,
	Linear,
};

enum class TextureWrapping
{
	Repeat,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder,
};

enum class TexelsFormat
{
	None = 0,
	R_U8,
	RG_U8,
	RGB_U8,
	RGBA_U8,
	Depth_U16,
	DepthStencil_U16,
	Depth_U24,
	DepthStencil_U24,
};

struct Texture2DInfo
{
	RenderResourceUsage usage = RenderResourceUsage::Static;

	TextureMinFilter minFilter = TextureMinFilter::NearestMipmapNearest;
	TextureMagFilter magFilter = TextureMagFilter::Nearest;
	TextureWrapping wrapS = TextureWrapping::Repeat;
	TextureWrapping wrapT = TextureWrapping::Repeat;

	bool mipmap = true;
};

struct Texture2DCreateInfo
{
	TexelsFormat format = TexelsFormat::RGBA_U8;
	uint16_t width = 1;
	uint16_t height = 1;
	uint8_t* pixelData = nullptr;
	unsigned mipMapCount = 1; // TODO: only compressed

	bool isTransparent = false;
};

class Texture2D
{
public:
	bool Create(const char* fileName, const Texture2DInfo& textureInfo = {});
	bool Create(const Texture2DCreateInfo& createInfo, const Texture2DInfo& textureInfo = {});

	void Destroy();

	void Bind(unsigned slot = 0) const;

	static void UnBind(unsigned slot = 0);
	static void UnBindAll();

	unsigned GetWidth() const { return m_width; }
	unsigned GetHeight() const { return m_height; }

	bool IsValid() const { return m_id > 0; }

	bool operator==(const Texture2D&) const = default;

	bool isTransparent = false;

private:
	unsigned m_id = 0;
	unsigned m_width = 0;
	unsigned m_height = 0;
};

//=============================================================================
// Texture Cube
//=============================================================================

struct TextureCubeInfo
{
	RenderResourceUsage usage = RenderResourceUsage::Static;

	TextureMinFilter minFilter = TextureMinFilter::NearestMipmapNearest;
	TextureMagFilter magFilter = TextureMagFilter::Nearest;
	TextureWrapping wrapS = TextureWrapping::ClampToEdge;
	TextureWrapping wrapT = TextureWrapping::ClampToEdge;
	TextureWrapping wrapR = TextureWrapping::ClampToEdge;

	bool mipmap = true;
};

class TextureCube
{
public:
	bool Create(
		const char* fileNameRight,
		const char* fileNameLeft,
		const char* fileNameBottom,
		const char* fileNameTop,
		const char* fileNameFront,
		const char* fileNameBack,
		const TextureCubeInfo& textureInfo = {});

	void Destroy();

	void Bind() const;
	static void UnBind();

	unsigned GetWidth() const { return m_width; }
	unsigned GetHeight() const { return m_height; }

	bool IsValid() const { return m_id > 0; }

private:
	unsigned m_id = 0;
	unsigned m_width = 0;
	unsigned m_height = 0;
};

//=============================================================================
// FrameBuffer
//=============================================================================

class FrameBuffer
{
public:
	bool Create(int width, int height);
	void Destroy();

	void Bind(const Vector3& color);

	void BindTextureBuffer();

	static void MainFrameBufferBind();

	bool IsValid() const { return m_id > 0 && m_texColorBuffer > 0 && m_rbo > 0; }

private:
	bool checkFramebuffer();

	unsigned m_id = 0;
	unsigned m_texColorBuffer = 0;
	unsigned m_rbo = 0;
	int m_width = 0;
	int m_height = 0;
};