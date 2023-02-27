#include "Profiler.h"
#if SE_PROFILE_ENABLED
#include <cassert>
#include <fstream>
#include "MicroMath.h"

Profiler *Profiler::sInstance = nullptr;
thread_local ProfileThread *ProfileThread::sInstance = nullptr;

bool ProfileMeasurement::s_outOfSamplesReported = false;

void Profiler::NextFrame()
{
	std::lock_guard lock(m_lock);

	if( m_dump )
	{
		dumpInternal();
		m_dump = false;
	}

	for( ProfileThread *t : m_threads )
		t->CurrentSample = 0;
}

void Profiler::Dump(const std::string_view& inTag)
{
	m_dump = true;
	m_dumpTag = inTag;
}

void Profiler::AddThread(ProfileThread* inThread)
{
	std::lock_guard lock(m_lock);

	m_threads.push_back(inThread);
}

void Profiler::RemoveThread(ProfileThread* inThread)
{
	std::lock_guard lock(m_lock);

	auto i = std::find(m_threads.begin(), m_threads.end(), inThread);
	assert(i != m_threads.end());
	m_threads.erase(i);
}

void Profiler::sAggregate(int inDepth, uint32_t inColor, ProfileSample*& ioSample, const ProfileSample* inEnd, Aggregators& ioAggregators, KeyToAggregator& ioKeyToAggregator)
{
	// Store depth
	ioSample->Depth = uint8_t(std::min(255, inDepth));

	// Update color
	if( ioSample->Color == 0 )
		ioSample->Color = inColor;
	else
		inColor = ioSample->Color;

	// Start accumulating totals
	uint64_t cycles_this_with_children = ioSample->EndCycle - ioSample->StartCycle;
	uint64_t cycles_in_children = 0;

	// Loop over following samples until we find a sample that starts on or after our end
	ProfileSample* sample;
	for( sample = ioSample + 1; sample < inEnd && sample->StartCycle < ioSample->EndCycle; ++sample )
	{
		assert(sample[-1].StartCycle <= sample->StartCycle);
		assert(sample->StartCycle >= ioSample->StartCycle);
		assert(sample->EndCycle <= ioSample->EndCycle);

		// This is a direct child of us, accumulate time
		cycles_in_children += sample->EndCycle - sample->StartCycle;

		// Recurse and skip over the children of this child
		sAggregate(inDepth + 1, inColor, sample, inEnd, ioAggregators, ioKeyToAggregator);
	}

	// Find the aggregator for this name / filename pair
	Aggregator* aggregator;
	auto aggregator_idx = ioKeyToAggregator.find(ioSample->Name);
	if( aggregator_idx == ioKeyToAggregator.end() )
	{
		// Not found, add to map and insert in array
		ioKeyToAggregator.try_emplace(ioSample->Name, ioAggregators.size());
		ioAggregators.emplace_back(ioSample->Name);
		aggregator = &ioAggregators.back();
	}
	else
	{
		// Found
		aggregator = &ioAggregators[aggregator_idx->second];
	}

	// Add the measurement to the aggregator
	aggregator->AccumulateMeasurement(cycles_this_with_children, cycles_in_children);

	// Update ioSample to the last child of ioSample
	assert(sample[-1].StartCycle <= ioSample->EndCycle);
	assert(sample >= inEnd || sample->StartCycle >= ioSample->EndCycle);
	ioSample = sample - 1;
}

void Profiler::dumpInternal()
{
	// Freeze data from threads
	// Note that this is not completely thread safe: As a profile sample is added mCurrentSample is incremented
	// but the data is not written until the sample finishes. So if we dump the profile information while
	// some other thread is running, we may get some garbage information from the previous frame
	Threads threads;
	for( ProfileThread* t : m_threads )
		threads.push_back({ t->ThreadName, t->Samples, t->Samples + t->CurrentSample });

	// Shift all samples so that the first sample is at zero
	uint64_t min_cycle = 0xffffffffffffffffUL;
	for( const ThreadSamples& t : threads )
		if( t.SamplesBegin < t.SamplesEnd )
			min_cycle = std::min(min_cycle, t.SamplesBegin[0].StartCycle);
	for( const ThreadSamples& t : threads )
		for( ProfileSample* s = t.SamplesBegin, *end = t.SamplesEnd; s < end; ++s )
		{
			s->StartCycle -= min_cycle;
			s->EndCycle -= min_cycle;
		}

	// Determine tag of this profile
	std::string tag;
	if( m_dumpTag.empty() )
	{
		// Next sequence number
		static int number = 0;
		++number;
		tag = std::to_string(number);
	}
	else
	{
		// Take provided tag
		tag = m_dumpTag;
		m_dumpTag.clear();
	}

	// Aggregate data across threads
	Aggregators aggregators;
	KeyToAggregator key_to_aggregators;
	for( const ThreadSamples& t : threads )
		for( ProfileSample* s = t.SamplesBegin, *end = t.SamplesEnd; s < end; ++s )
			sAggregate(0, Color::Red.ToUInt(), s, end, aggregators, key_to_aggregators);

	// Dump as list
	dumpList(tag.c_str(), aggregators);

	// Dump as chart
	dumpChart(tag.c_str(), threads, key_to_aggregators, aggregators);
}

static std::string sHTMLEncode(const char *inString)
{
	std::string str(inString);
	StringReplace(str, "<", "&lt;");
	StringReplace(str, ">", "&gt;");
	return str;
}

void Profiler::dumpList(const char *inTag, const Aggregators& inAggregators)
{
	// Open file
	std::ofstream f;
	f.open(StringFormat("profile_list_%s.html", inTag).c_str(), std::ofstream::out | std::ofstream::trunc);
	if( !f.is_open() )
		return;

	// Write header
	f << R"(<!DOCTYPE html>
<html>
	<head>
		<title>Profile List</title>
		<link rel="stylesheet" href="WebIncludes/semantic.min.css">
		<script type="text/javascript" src="WebIncludes/jquery-3.2.1.min.js"></script>
		<script type="text/javascript" src="WebIncludes/semantic.min.js"></script>
		<script type="text/javascript" src="WebIncludes/tablesort.js"></script>
		<script type="text/javascript">$(document).ready(function() { $('table').tablesort({ compare: function(a, b) { return isNaN(a) || isNaN(b)? a.localeCompare(b) : Number(a) - Number(b); } }); });</script>
	</head>
	<body class="minimal pushable">
		<table id="profile" class="ui sortable celled striped table">
			<thead>
				<tr>
					<th>Description</th>
					<th class="sorted descending">Total time with children (%)</th>
					<th>Total time (%)</th>
					<th>Calls</th>
					<th>&micro;s / call with children</th>
					<th>&micro;s / call</th>
					<th>Min. &micro;s / call</th>
					<th>Max. &micro;s / call</th>
				</tr>
			</thead>
			<tbody style="text-align: right;">
)";

	// Get total time
	uint64_t total_time = 0;
	for( const Aggregator &item : inAggregators )
		total_time += item.TotalCyclesInCallWithChildren - item.TotalCyclesInChildren;

	// Get cycles per second
	uint64_t cycles_per_second = GetProcessorTicksPerSecond();

	// Sort the list
	Aggregators aggregators = inAggregators;
	QuickSort(aggregators.begin(), aggregators.end());

	// Write all aggregators
	for( const Aggregator &item : aggregators )
	{
		uint64_t cycles_in_call_no_children = item.TotalCyclesInCallWithChildren - item.TotalCyclesInChildren;

		char str[2048];
		snprintf(str, sizeof(str), R"(<tr>
	<td style="text-align: left;">%s</td>
	<td>%.1f</td>						
	<td>%.1f</td>						
	<td>%u</td>
	<td>%.2f</td>						
	<td>%.2f</td>						
	<td>%.2f</td>						
	<td>%.2f</td>						
</tr>)",
sHTMLEncode(item.Name).c_str(),													// Description
100.0 * item.TotalCyclesInCallWithChildren / total_time,								// Total time with children
100.0 * cycles_in_call_no_children / total_time,										// Total time no children
item.CallCounter,																		// Calls
1000000.0 * item.TotalCyclesInCallWithChildren / cycles_per_second / item.CallCounter,	// us / call with children
1000000.0 * cycles_in_call_no_children / cycles_per_second / item.CallCounter,			// us / call no children
1000000.0 * item.MinCyclesInCallWithChildren / cycles_per_second,						// Min. us / call with children
1000000.0 * item.MaxCyclesInCallWithChildren / cycles_per_second);						// Max. us / call with children

		f << str;
	}

	// End table
	f << R"(</tbody></table></body></html>)";
}

void Profiler::dumpChart(const char* inTag, const Threads& inThreads, const KeyToAggregator& inKeyToAggregators, const Aggregators& inAggregators)
{
	// Open file
	std::ofstream f;
	f.open(StringFormat("profile_chart_%s.html", inTag).c_str(), std::ofstream::out | std::ofstream::trunc);
	if( !f.is_open() )
		return;

	// Write header
	f << R"(<!DOCTYPE html>
<html>
	<head>
		<title>Profile Chart</title>
		<link rel="stylesheet" href="WebIncludes/profile_chart.css">
		<script type="text/javascript" src="WebIncludes/profile_chart.js"></script>
	</head>
	<body onload="startChart();">
	<script type="text/javascript">
)";

	// Get cycles per second
	uint64_t cycles_per_second = GetProcessorTicksPerSecond();
	f << "var cycles_per_second = " << cycles_per_second << ";\n";

	// Dump samples
	f << "var threads = [\n";
	bool first_thread = true;
	for( const ThreadSamples &t : inThreads )
	{
		if( !first_thread )
			f << ",\n";
		first_thread = false;

		f << "{\nthread_name: \"" << t.ThreadName << "\",\naggregator: [";
		bool first = true;
		for( const ProfileSample *s = t.SamplesBegin, *end = t.SamplesEnd; s < end; ++s )
		{
			if( !first )
				f << ",";
			first = false;
			f << inKeyToAggregators.find(s->Name)->second;
		}
		f << "],\ncolor: [";
		first = true;
		for( const ProfileSample *s = t.SamplesBegin, *end = t.SamplesEnd; s < end; ++s )
		{
			if( !first )
				f << ",";
			first = false;
			Color c(s->mColor);
			f << StringFormat("\"#%02x%02x%02x\"", c.r, c.g, c.b);
		}
		f << "],\nstart: [";
		first = true;
		for( const ProfileSample* s = t.SamplesBegin, *end = t.SamplesEnd; s < end; ++s )
		{
			if( !first )
				f << ",";
			first = false;
			f << s->StartCycle;
		}
		f << "],\ncycles: [";
		first = true;
		for( const ProfileSample* s = t.SamplesBegin, *end = t.SamplesEnd; s < end; ++s )
		{
			if( !first )
				f << ",";
			first = false;
			f << s->EndCycle - s->StartCycle;
		}
		f << "],\ndepth: [";
		first = true;
		for( const ProfileSample* s = t.SamplesBegin, *end = t.SamplesEnd; s < end; ++s )
		{
			if( !first )
				f << ",";
			first = false;
			f << int(s->Depth);
		}
		f << "]\n}";
	}

	// Dump aggregated data
	f << "];\nvar aggregated = {\nname: [";
	bool first = true;
	for( const Aggregator &a : inAggregators )
	{
		if( !first )
			f << ",";
		first = false;
		std::string name = "\"" + sHTMLEncode(a.Name) + "\"";
		f << name;
	}
	f << "],\ncalls: [";
	first = true;
	for( const Aggregator &a : inAggregators )
	{
		if( !first )
			f << ",";
		first = false;
		f << a.CallCounter;
	}
	f << "],\nmin_cycles: [";
	first = true;
	for( const Aggregator &a : inAggregators )
	{
		if( !first )
			f << ",";
		first = false;
		f << a.MinCyclesInCallWithChildren;
	}
	f << "],\nmax_cycles: [";
	first = true;
	for( const Aggregator &a : inAggregators )
	{
		if( !first )
			f << ",";
		first = false;
		f << a.MaxCyclesInCallWithChildren;
	}
	f << "],\ncycles_per_frame: [";
	first = true;
	for( const Aggregator &a : inAggregators )
	{
		if( !first )
			f << ",";
		first = false;
		f << a.TotalCyclesInCallWithChildren;
	}

	// Write footer
	f << R"(]};
</script>

<canvas id="canvas"></canvas>
<div id="tooltip"></div>

</tbody></table></body></html>)";
}


#endif // SE_PROFILE_ENABLED