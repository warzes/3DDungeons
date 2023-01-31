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

private:
	unsigned m_id = 0;
};