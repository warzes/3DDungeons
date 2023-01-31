#include "MicroOpenGLLoader.h"
//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
#	pragma comment( lib, "OpenGL32.lib" )
#endif
//-----------------------------------------------------------------------------
#if defined(_WIN32)
extern "C"
{
	// NVIDIA: Force usage of NVidia GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> See "Enabling High Performance Graphics Rendering on Optimus Systems" http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;

	// AMD: Force usage of AMD GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> Named "Dynamic Switchable Graphics", found no official documentation, only https://community.amd.com/message/1307599#comment-1307599 - "Can an OpenGL app default to the discrete GPU on an Enduro system?"
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif // _WIN32

/* ----------------------------- GL_VERSION_2_0 ---------------------------- */

// Shader Program
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;

// Shader Program Uniform
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;

// Shader
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;

//-----------------------------------------------------------------------------
void OpenGLInit(OpenGLGetProcAddressFunc func)
{
	// Shader Program
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)func("glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)func("glDeleteProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)func("glUseProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)func("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)func("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)func("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)func("glGetProgramInfoLog");

	// Shader Program Uniform
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)func("glGetUniformLocation");
	glUniform1f = (PFNGLUNIFORM1FPROC)func("glUniform1f");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)func("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)func("glUniform3fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)func("glUniformMatrix4fv");

	// Shader
	glCreateShader = (PFNGLCREATESHADERPROC)func("glCreateShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)func("glDeleteShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)func("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)func("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)func("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)func("glGetShaderInfoLog");
}
//-----------------------------------------------------------------------------