ProfileThread::ProfileThread(const std::string_view& inThreadName) 
	: ThreadName(inThreadName)
{
	Profiler::sInstance->AddThread(this);
}

ProfileThread::~ProfileThread()
{
	Profiler::sInstance->RemoveThread(this);
}

ProfileMeasurement::ProfileMeasurement(const char* inName, uint32_t inColor)
{
	if( ProfileThread::sInstance == nullptr )
	{
		// Thread not instrumented
		m_sample = nullptr;
	}
	else if( ProfileThread::sInstance->CurrentSample < ProfileThread::cMaxSamples )
	{
		// Get pointer to write data to
		m_sample = &ProfileThread::sInstance->Samples[ProfileThread::sInstance->CurrentSample++];

		// Start constructing sample (will end up on stack)
		m_temp.Name = inName;
		m_temp.Color = inColor;

		// Collect start sample last
		m_temp.StartCycle = GetProcessorTickCount();
	}
	else
	{
		// Out of samples
		if( !s_outOfSamplesReported )
		{
			extern void LogError(const std::string & msg);
			LogError("ProfileMeasurement: Too many samples, some data will be lost!");
			s_outOfSamplesReported = true;
		}
		m_sample = nullptr;
	}
}

ProfileMeasurement::~ProfileMeasurement()
{
	if( m_sample != nullptr )
	{
		// Finalize sample
		m_temp.EndCycle = GetProcessorTickCount();

		// Write it to the memory buffer bypassing the cache
		static_assert(sizeof(ProfileSample) == 32, "Assume 32 bytes");
		static_assert(alignof(ProfileSample) == 16, "Assume 16 byte alignment");
#if defined(SE_USE_SSE)
		const __m128i *src = reinterpret_cast<const __m128i *>(&m_temp);
		__m128i *dst = reinterpret_cast<__m128i *>(m_sample);
		__m128i val = _mm_loadu_si128(src);
		_mm_stream_si128(dst, val);
		val = _mm_loadu_si128(src + 1);
		_mm_stream_si128(dst + 1, val);
#elif defined(SE_USE_NEON)
		const int *src = reinterpret_cast<const int *>(&mTemp);
		int *dst = reinterpret_cast<int *>(mSample);
		int32x4_t val = vld1q_s32(src);
		vst1q_s32(dst, val);
		val = vld1q_s32(src + 4);
		vst1q_s32(dst + 4, val);
#else
		memcpy(mSample, &mTemp, sizeof(ProfileSample));
#endif
		m_sample = nullptr;
	}
}