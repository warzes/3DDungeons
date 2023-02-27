#pragma once

#if defined(_WIN32) || defined(_WIN64)
#endif // _WIN32

#if defined(__ANDROID__)
#endif // __ANDROID__

#if defined(__linux__)
#endif // __linux__

#if defined(__APPLE__)
#endif // __APPLE__

#if defined(__EMSCRIPTEN__)
#endif // __EMSCRIPTEN__

// Detect CPU architecture
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
// X86 CPU architecture
#	define SE_CPU_X86
#	if defined(__x86_64__) || defined(_M_X64)
#		define SE_CPU_ADDRESS_BITS 64
#	else
#		define SE_CPU_ADDRESS_BITS 32
#	endif
#	define SE_USE_SSE
#	define SE_VECTOR_ALIGNMENT 16

#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm__) || defined(_M_ARM)
// ARM CPU architecture
#	define SE_CPU_ARM
#	if defined(__aarch64__) || defined(_M_ARM64)
#		define SE_CPU_ADDRESS_BITS 64
#		define SE_USE_NEON
#		define SE_VECTOR_ALIGNMENT 16
#	else
#		define SE_CPU_ADDRESS_BITS 32
#		define SE_VECTOR_ALIGNMENT 8
#	endif
#elif defined(__EMSCRIPTEN__)
// WebAssembly CPU architecture
#	define SE_CPU_WASM
#	define SE_CPU_ADDRESS_BITS 32
#	define SE_VECTOR_ALIGNMENT 16
#	define SE_DISABLE_CUSTOM_ALLOCATOR
#else
#	error Unsupported CPU architecture
#endif