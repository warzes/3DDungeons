#pragma once

// TODO: для сортировки строк https://spiskin.ru/scripts/sort_alphabet/

#ifdef __gl_h_
#	error OpenGL (gl.h) header already included (API: gl), remove previous include!
#endif
#define __gl_h_ 1


#ifndef GLAPIENTRY
#	ifdef APIENTRY
#		define GLAPIENTRY APIENTRY
#	elif defined(_WIN32)
#		define GLAPIENTRY __stdcall
#	else
#		define GLAPIENTRY
#	endif
#endif

#ifndef GLAPI
#	define GLAPI __declspec(dllimport)
#endif

/* ----------------------------- GL_VERSION_1_1 ---------------------------- */
#define GL_VERSION_1_0 1
#define GL_VERSION_1_1 1

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
typedef char GLchar;
#if defined(_MSC_VER)
typedef signed long long GLint64;
typedef unsigned long long GLuint64;
#else
typedef int64_t GLint64;
typedef uint64_t GLuint64;
#endif

#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_DEPTH_TEST 0x0B71
#define GL_FALSE 0
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_TRUE 1
#define GL_UNPACK_ALIGNMENT 0x0CF5

// OpenGL32.lib
#ifdef __cplusplus
extern "C" {
#endif
	GLAPI void GLAPIENTRY glClear(GLbitfield mask);
	GLAPI void GLAPIENTRY glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	GLAPI void GLAPIENTRY glClearDepth(GLclampd depth);
	GLAPI void GLAPIENTRY glDepthRange(GLclampd zNear, GLclampd zFar);
	GLAPI void GLAPIENTRY glDisable(GLenum cap);
	GLAPI void GLAPIENTRY glEnable(GLenum cap);
	GLAPI void GLAPIENTRY glPixelStorei(GLenum pname, GLint param);
	GLAPI void GLAPIENTRY glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
	GLAPI void GLAPIENTRY glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
#ifdef __cplusplus
}
#endif

/* ----------------------------- GL_VERSION_1_2 ---------------------------- */
#define GL_VERSION_1_2 1

/* ----------------------------- GL_VERSION_1_3 ---------------------------- */
#define GL_VERSION_1_3 1

/* ----------------------------- GL_VERSION_1_4 ---------------------------- */
#define GL_VERSION_1_4 1

/* ----------------------------- GL_VERSION_1_5 ---------------------------- */
#define GL_VERSION_1_5 1

/* ----------------------------- GL_VERSION_2_0 ---------------------------- */
#define GL_VERSION_2_0 1

#define GL_COMPILE_STATUS 0x8B81
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_LINK_STATUS 0x8B82
#define GL_VERTEX_SHADER 0x8B31

// Shader Program
typedef GLuint(GLAPIENTRY* PFNGLCREATEPROGRAMPROC)();
typedef void (GLAPIENTRY* PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (GLAPIENTRY* PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (GLAPIENTRY* PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (GLAPIENTRY* PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (GLAPIENTRY* PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint* params);
typedef void (GLAPIENTRY* PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei* length, char* infoLog);

// Shader Program Uniform
typedef GLint(GLAPIENTRY* PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const char* name);
typedef void (GLAPIENTRY* PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (GLAPIENTRY* PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY* PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY* PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

// Shader
typedef GLuint(GLAPIENTRY* PFNGLCREATESHADERPROC)(GLenum type);
typedef void (GLAPIENTRY* PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (GLAPIENTRY* PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const char* const* string, const GLint* length);
typedef void (GLAPIENTRY* PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (GLAPIENTRY* PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
typedef void (GLAPIENTRY* PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei* length, char* infoLog);

// Shader Program
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

// Shader Program Uniform
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

// Shader
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

/* ----------------------------- GL_VERSION_2_1 ---------------------------- */
#define GL_VERSION_2_1 1

/* ----------------------------- GL_VERSION_3_0 ---------------------------- */
#define GL_VERSION_3_0 1

/* ----------------------------- GL_VERSION_3_1 ---------------------------- */
#define GL_VERSION_3_1 1

/* ----------------------------- GL_VERSION_3_2 ---------------------------- */
#define GL_VERSION_3_2 1

/* ----------------------------- GL_VERSION_3_3 ---------------------------- */
#define GL_VERSION_3_3 1


//=============================================================================
// OpenGL Func
//=============================================================================
typedef void* (OpenGLGetProcAddressFunc)(const char* funcName);
void OpenGLInit(OpenGLGetProcAddressFunc func);