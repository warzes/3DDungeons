#pragma once

#include <cstdint>

#if defined(_WIN32)
#	include <intrin.h> 
#elif defined(SE_CPU_X86) && defined(__GNUC__)
#	include <x86intrin.h>
#endif

#include "TempCore.h"

inline uint64_t GetProcessorTickCount()
{
#if defined(SE_CPU_X86)
	return __rdtsc();
#elif defined(SE_CPU_ARM)
	uint64 val;
	asm volatile("mrs %0, cntvct_el0" : "=r" (val));
	return val;
#elif defined(__EMSCRIPTEN__)
	return 0; // Not supported
#else
#error Undefined
#endif
}

// Кол-во тиков в секунду. Не точное, используется только для профилирования
uint64_t GetProcessorTicksPerSecond();