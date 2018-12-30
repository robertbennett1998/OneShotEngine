#pragma once
#ifndef ONE_SHOT_CORE_I_LOG_SINK_H
#define ONE_SHOT_CORE_I_LOG_SINK_H

#include <string>

class ONE_SHOT_CORE_DLL ILogSink
{
	public:
		virtual void Output(std::string sOut) = 0;
};

#endif