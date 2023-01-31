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
class Matrix4;

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