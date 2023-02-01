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
#define GL_DEPTH_COMPONENT 0x1902
#define GL_DEPTH_TEST 0x0B71
#define GL_FALSE 0
#define GL_FLOAT 0x1406
#define GL_GREEN 0x1904
#define GL_LINEAR 0x2601
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_LINES 0x0001
#define GL_NEAREST 0x2600
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_POINTS 0x0000
#define GL_RED 0x1903
#define GL_REPEAT 0x2901
#define GL_RGB 0x1907
#define GL_RGB8 0x8051
#define GL_RGBA 0x1908
#define GL_RGBA8 0x8058
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TRIANGLES 0x0004
#define GL_TRUE 1
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT 0x1405
#define GL_UNSIGNED_SHORT 0x1403

// OpenGL32.lib
#ifdef __cplusplus
extern "C" {
#endif
	GLAPI void GLAPIENTRY glBindTexture(GLenum target, GLuint texture);
	GLAPI void GLAPIENTRY glClear(GLbitfield mask);
	GLAPI void GLAPIENTRY glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	GLAPI void GLAPIENTRY glClearDepth(GLclampd depth);
	GLAPI void GLAPIENTRY glDeleteTextures(GLsizei n, const GLuint* textures);
	GLAPI void GLAPIENTRY glDepthRange(GLclampd zNear, GLclampd zFar);
	GLAPI void GLAPIENTRY glDisable(GLenum cap);
	GLAPI void GLAPIENTRY glDrawArrays(GLenum mode, GLint first, GLsizei count);
	GLAPI void GLAPIENTRY glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
	GLAPI void GLAPIENTRY glEnable(GLenum cap);
	GLAPI void GLAPIENTRY glGenTextures(GLsizei n, GLuint* textures);
	GLAPI void GLAPIENTRY glGetIntegerv(GLenum pname, GLint* params);
	GLAPI void GLAPIENTRY glPixelStorei(GLenum pname, GLint param);
	GLAPI void GLAPIENTRY glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
	GLAPI void GLAPIENTRY glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
	GLAPI void GLAPIENTRY glTexParameteri(GLenum target, GLenum pname, GLint param);
	GLAPI void GLAPIENTRY glTexParameteriv(GLenum target, GLenum pname, const GLint* params);
	GLAPI void GLAPIENTRY glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
#ifdef __cplusplus
}
#endif

/* ----------------------------- GL_VERSION_1_2 ---------------------------- */

#define GL_CLAMP_TO_EDGE 0x812F

/* ----------------------------- GL_VERSION_1_3 ---------------------------- */

#define GL_CLAMP_TO_BORDER 0x812D
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515

typedef void (GLAPIENTRY* PFNGLACTIVETEXTUREPROC)(GLenum texture);
extern PFNGLACTIVETEXTUREPROC glActiveTexture;

/* ----------------------------- GL_VERSION_1_4 ---------------------------- */

#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_MIRRORED_REPEAT 0x8370

/* ----------------------------- GL_VERSION_1_5 ---------------------------- */

#ifdef _WIN64
typedef __int64          GLsizeiptr;
typedef __int64          GLintptr;
#else
typedef int              GLsizeiptr;
typedef int              GLintptr;
#endif

#define GL_ARRAY_BUFFER 0x8892
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_STREAM_DRAW 0x88E0

// Buffer
typedef void (GLAPIENTRY* PFNGLGENBUFFERSPROC)(GLsizei n, GLuint* buffers);
typedef void (GLAPIENTRY* PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint* buffers);
typedef void (GLAPIENTRY* PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (GLAPIENTRY* PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (GLAPIENTRY* PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);

extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;

// Vertex Arrays
typedef void (GLAPIENTRY* PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint* arrays);
typedef void (GLAPIENTRY* PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint* arrays);
typedef void (GLAPIENTRY* PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (GLAPIENTRY* PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (GLAPIENTRY* PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

/* ----------------------------- GL_VERSION_2_0 ---------------------------- */

#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_COMPILE_STATUS 0x8B81
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_LINK_STATUS 0x8B82
#define GL_VERTEX_SHADER 0x8B31

// Shader Program
typedef GLuint(GLAPIENTRY* PFNGLCREATEPROGRAMPROC)();
typedef void (GLAPIENTRY* PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (GLAPIENTRY* PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (GLAPIENTRY* PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (GLAPIENTRY *PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (GLAPIENTRY* PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (GLAPIENTRY* PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint* params);
typedef void (GLAPIENTRY* PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei* length, char* infoLog);

extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

// Shader Program Attrib
typedef void (GLAPIENTRY* PFNGLGETACTIVEATTRIBPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, char* name);
typedef GLint(GLAPIENTRY* PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const char* name);

extern PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;

// Shader Program Uniform
typedef GLint(GLAPIENTRY* PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const char* name);
typedef void (GLAPIENTRY* PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (GLAPIENTRY* PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY* PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY* PFNGLUNIFORMMATRIX3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY* PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

// Shader
typedef GLuint(GLAPIENTRY* PFNGLCREATESHADERPROC)(GLenum type);
typedef void (GLAPIENTRY* PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (GLAPIENTRY* PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const char* const* string, const GLint* length);
typedef void (GLAPIENTRY* PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (GLAPIENTRY* PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
typedef void (GLAPIENTRY* PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei* length, char* infoLog);

extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

/* ----------------------------- GL_VERSION_2_1 ---------------------------- */

/* ----------------------------- GL_VERSION_3_0 ---------------------------- */

/* ----------------------------- GL_VERSION_3_1 ---------------------------- */


/* ----------------------------- GL_VERSION_3_2 ---------------------------- */

/* ----------------------------- GL_VERSION_3_3 ---------------------------- */

/* ----------------------- GL_ARB_framebuffer_object ----------------------- */

#define GL_DEPTH_STENCIL 0x84F9
#define GL_DEPTH24_STENCIL8 0x88F0

typedef void (GLAPIENTRY* PFNGLGENERATEMIPMAPPROC)(GLenum target);
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

/* --------------------------- GL_ARB_texture_rg --------------------------- */

#define GL_R8 0x8229
#define GL_RG 0x8227
#define GL_RG8 0x822B

/* ------------------------- GL_ARB_texture_swizzle ------------------------ */

#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46

//=============================================================================
// OpenGL Func
//=============================================================================
typedef void* (OpenGLGetProcAddressFunc)(const char* funcName);
void OpenGLInit(OpenGLGetProcAddressFunc func);