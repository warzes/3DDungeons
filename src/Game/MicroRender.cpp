#include "MicroRender.h"
#include "MicroMath.h"

//=============================================================================
// Header
//=============================================================================
#if defined(_MSC_VER)
#	pragma warning(push, 0)
#	pragma warning(disable : 5264)
#endif // _MSC_VER

#include <assert.h>

#if defined(_MSC_VER)
#	pragma warning(pop)
#	pragma warning(disable : 5045)
#endif // _MSC_VER

// ==> MicroEngine
extern void LogPrint(const std::string& msg);
extern void LogWarning(const std::string& msg);
extern void LogError(const std::string& msg);
extern void Fatal(const std::string& msg);

//=============================================================================
// Global vars
//=============================================================================
namespace state
{
	unsigned CurrentShaderProgram = 0;
}

//=============================================================================
// Core
//=============================================================================

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