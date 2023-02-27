#include "TickCounter.h"

#if defined(_WIN32)
#	include <Windows.h>
#elif defined(__linux__) || defined(__ANDROID__)
#	include <fstream>
#endif

uint64_t GetProcessorTicksPerSecond()
{
#if defined(_WIN32)
	// Open the key where the processor speed is stored
	HKEY hkey;
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, 1, &hkey);

	// Query the speed in MHz
	uint32_t mhz = 0;
	DWORD mhz_size = sizeof(uint32_t);
	RegQueryValueExA(hkey, "~MHz", nullptr, nullptr, (LPBYTE)&mhz, &mhz_size);

	// Close key
	RegCloseKey(hkey);

	// Initialize amount of cycles per second
	return uint64_t(mhz) * 1000000UL;
#elif defined(__linux__) || defined(__ANDROID__) 
	// Open /proc/cpuinfo
	std::ifstream ifs("/proc/cpuinfo");
	if( ifs.is_open() )
	{
		// Read all lines
		while( ifs.good() )
		{
			// Get next line
			string line;
			getline(ifs, line);

#if defined(SE_CPU_X86)
			const char *cpu_str = "cpu MHz";
#elif defined(SE_CPU_ARM)
			const char *cpu_str = "BogoMIPS";
#else
#error Unsupported CPU architecture
#endif

			// Check if line starts with correct string
			const size_t num_chars = strlen(cpu_str);
			if( strncmp(line.c_str(), cpu_str, num_chars) == 0 )
			{
				// Find ':'
				string::size_type pos = line.find(':', num_chars);
				if( pos != String::npos )
				{
					// Convert to number
					string freq = line.substr(pos + 1);
					return uint64(stod(freq) * 1000000.0);
				}
			}
		}
	}

	assert(false);
	return uint64(0);
#elif defined(__EMSCRIPTEN__)
	return 1; // Not supported
#else
#error Undefined
#endif
}