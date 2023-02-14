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
//-----------------------------------------------------------------------------
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLBLENDFUNCPROC glBlendFunc = nullptr;;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLCULLFACEPROC glCullFace = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLDEPTHFUNCPROC glDepthFunc = nullptr;
PFNGLDETACHSHADERPROC glDetachShader = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = nullptr;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = nullptr;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLPOLYGONMODEPROC glPolygonMode = nullptr;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;

//-----------------------------------------------------------------------------
void OpenGLInit(OpenGLGetProcAddressFunc func)
{
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)func("glActiveTexture");
	glAttachShader = (PFNGLATTACHSHADERPROC)func("glAttachShader");
	glBindBuffer = (PFNGLBINDBUFFERPROC)func("glBindBuffer");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)func("glBindFramebuffer");
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)func("glBindRenderbuffer");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)func("glBindVertexArray");
	glBlendFunc = (PFNGLBLENDFUNCPROC)func("glBlendFunc");
	glBufferData = (PFNGLBUFFERDATAPROC)func("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)func("glBufferSubData");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)func("glCheckFramebufferStatus");
	glCompileShader = (PFNGLCOMPILESHADERPROC)func("glCompileShader");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)func("glCreateProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)func("glCreateShader");
	glCullFace = (PFNGLCULLFACEPROC)func("glCullFace");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)func("glDeleteBuffers");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)func("glDeleteFramebuffers");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)func("glDeleteProgram");
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)func("glDeleteRenderbuffers");
	glDeleteShader = (PFNGLDELETESHADERPROC)func("glDeleteShader");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)func("glDeleteVertexArrays");
	glDepthFunc = (PFNGLDEPTHFUNCPROC)func("glDepthFunc");
	glDetachShader = (PFNGLDETACHSHADERPROC)func("glDetachShader");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)func("glEnableVertexAttribArray");
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)func("glFramebufferRenderbuffer");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)func("glFramebufferTexture2D");
	glGenBuffers = (PFNGLGENBUFFERSPROC)func("glGenBuffers");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)func("glGenerateMipmap");
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)func("glGenFramebuffers");
	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)func("glGenRenderbuffers");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)func("glGenVertexArrays");
	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)func("glGetActiveAttrib");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)func("glGetAttribLocation");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)func("glGetProgramInfoLog");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)func("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)func("glGetShaderInfoLog");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)func("glGetShaderiv");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)func("glGetUniformLocation");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)func("glLinkProgram");
	glPolygonMode = (PFNGLPOLYGONMODEPROC)func("glPolygonMode");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)func("glRenderbufferStorage");
	glShaderSource = (PFNGLSHADERSOURCEPROC)func("glShaderSource");
	glUniform1f = (PFNGLUNIFORM1FPROC)func("glUniform1f");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)func("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)func("glUniform3fv");
	glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)func("glUniformMatrix3fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)func("glUniformMatrix4fv");
	glUseProgram = (PFNGLUSEPROGRAMPROC)func("glUseProgram");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)func("glVertexAttribPointer");
}
//-----------------------------------------------------------------------------