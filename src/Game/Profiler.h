#pragma once

#define SE_PROFILE_ENABLED 1

#if SE_PROFILE_ENABLED

#include <cstdint>
#include <string_view>
#include <string>
#include <unordered_map>
#include <algorithm>

#include "Mutex.h"
#include "TickCounter.h"

class ProfileSample;
class ProfileThread;

/// Singleton class for managing profiling information
class Profiler
{
public:
	Profiler() = default;
	Profiler(const Profiler&) = delete;
	void operator=(const Profiler&) = delete;

	// Increments the frame counter to provide statistics per frame
	void NextFrame();

	// Dump profiling statistics at the start of the next frame
	// @param inTag If not empty, this overrides the auto incrementing number in the filename of the dump file
	void Dump(const std::string_view& inTag = std::string_view());

	// Add a thread to be instrumented
	void AddThread(ProfileThread* inThread);

	// Remove a thread from being instrumented
	void RemoveThread(ProfileThread* inThread);

	static Profiler* sInstance;
private:
	// Helper class to freeze ProfileSamples per thread while processing them
	struct ThreadSamples
	{
		std::string ThreadName;
		ProfileSample* SamplesBegin;
		ProfileSample* SamplesEnd;
	};

	// Helper class to aggregate ProfileSamples
	class Aggregator
	{
	public:
		Aggregator(const char* inName) : Name(inName) {}

		// Accumulate results for a measurement
		void AccumulateMeasurement(uint64_t inCyclesInCallWithChildren, uint64_t inCyclesInChildren)
		{
			CallCounter++;
			TotalCyclesInCallWithChildren += inCyclesInCallWithChildren;
			TotalCyclesInChildren += inCyclesInChildren;
			MinCyclesInCallWithChildren = std::min(inCyclesInCallWithChildren, MinCyclesInCallWithChildren);
			MaxCyclesInCallWithChildren = std::max(inCyclesInCallWithChildren, MaxCyclesInCallWithChildren);
		}

		// Sort descending by total cycles
		bool operator<(const Aggregator& inRHS) const
		{
			return TotalCyclesInCallWithChildren > inRHS.TotalCyclesInCallWithChildren;
		}

		// Identification			
		const char* Name; // User defined name of this item

		// Statistics				
		uint32_t CallCounter = 0; // Number of times AccumulateMeasurement was called
		uint64_t TotalCyclesInCallWithChildren = 0; // Total amount of cycles spent in this scope
		uint64_t TotalCyclesInChildren = 0; // Total amount of cycles spent in children of this scope
		uint64_t MinCyclesInCallWithChildren = 0xffffffffffffffffUL; // Minimum amount of cycles spent per call
		uint64_t MaxCyclesInCallWithChildren = 0; // Maximum amount of cycles spent per call
	};

	using Threads = std::vector<ThreadSamples>;
	using Aggregators = std::vector<Aggregator>;
	using KeyToAggregator = std::unordered_map<const char*, size_t>;

	/// Helper function to aggregate profile sample data
	static void sAggregate(int inDepth, uint32_t inColor, ProfileSample*& ioSample, const ProfileSample* inEnd, Aggregators& ioAggregators, KeyToAggregator& ioKeyToAggregator);

	// Dump profiling statistics
	void dumpInternal();
	void dumpList(const char* inTag, const Aggregators& inAggregators);
	void dumpChart(const char* inTag, const Threads& inThreads, const KeyToAggregator& inKeyToAggregators, const Aggregators& inAggregators);

	std::mutex m_lock; // Lock that protects mThreads
	std::vector<ProfileThread*> m_threads; // List of all active threads
	bool m_dump = false; // When true, the samples are dumped next frame
	std::string m_dumpTag; // When not empty, this overrides the auto incrementing number of the dump filename
};

// Class that contains the information of a single scoped measurement
class alignas(16) ProfileSample
{
public:
	const char* Name; // User defined name of this item
	uint32_t Color; // Color to use for this sample
	uint8_t Depth; // Calculated depth
	uint8_t Unused[3];
	uint64_t StartCycle; // Cycle counter at start of measurement
	uint64_t EndCycle; // Cycle counter at end of measurement
};

// Collects all samples of a single thread
class ProfileThread
{
public:	
	ProfileThread(const std::string_view& inThreadName);
	~ProfileThread();

	static const uint32_t cMaxSamples = 65536;

	std::string ThreadName; // Name of the thread that we're collecting information for
	ProfileSample Samples[cMaxSamples]; // Buffer of samples
	uint32_t CurrentSample = 0; // Next position to write a sample to

	static thread_local ProfileThread *sInstance;
};

// Create this class on the stack to start sampling timing information of a particular scope
class ProfileMeasurement
{
public:
	ProfileMeasurement(const char *inName, uint32_t inColor = 0);
	~ProfileMeasurement();

private:
	ProfileSample* m_sample;
	ProfileSample m_temp;
	static bool s_outOfSamplesReported;
};

#include "Profiler.inl"

// Start instrumenting program
#define SE_PROFILE_START(name) do { Profiler::sInstance = new Profiler; SE_PROFILE_THREAD_START(name); } while (false)

// End instrumenting program
#define SE_PROFILE_END() do { SE_PROFILE_THREAD_END(); delete Profiler::sInstance; Profiler::sInstance = nullptr; } while (false)

// Start instrumenting a thread
#define SE_PROFILE_THREAD_START(name) do { if (Profiler::sInstance) ProfileThread::sInstance = new ProfileThread(name); } while (false)

// End instrumenting a thread
#define SE_PROFILE_THREAD_END() do { delete ProfileThread::sInstance; ProfileThread::sInstance = nullptr; } while (false)

// Scope profiling measurement
#define SE_PROFILE_TAG2(line) profile##line
#define SE_PROFILE_TAG(line) SE_PROFILE_TAG2(line)
#define SE_PROFILE(...) ProfileMeasurement SE_PROFILE_TAG(__LINE__)(__VA_ARGS__)

// Scope profiling for function
#define SE_PROFILE_FUNCTION() SE_PROFILE(SE_FUNCTION_NAME)

// Update frame counter								
#define SE_PROFILE_NEXTFRAME() Profiler::sInstance->NextFrame()

// Dump profiling info
#define SE_PROFILE_DUMP(...) Profiler::sInstance->Dump(__VA_ARGS__)

#else

#define SE_PROFILE_START(name)
#define SE_PROFILE_END()
#define SE_PROFILE_THREAD_START(name)
#define SE_PROFILE_THREAD_END()
#define SE_PROFILE(...)
#define SE_PROFILE_FUNCTION()
#define SE_PROFILE_NEXTFRAME()
#define SE_PROFILE_DUMP(...)

#endif // SE_PROFILE_ENABLED