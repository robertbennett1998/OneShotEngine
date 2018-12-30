#pragma once
#ifndef ONE_SHOT_CORE_VS_DEBUG_SINK_H
#define ONE_SHOT_CORE_VS_DEBUG_SINK_H

#include "Interfaces/ILogSink.h"

class ONE_SHOT_CORE_DLL CVSDebugSink : public ILogSink
{
	public:
		CVSDebugSink();
		~CVSDebugSink();

		static std::shared_ptr<CVSDebugSink> CreateVSDebugSink()
		{
			std::shared_ptr<CVSDebugSink> sink = std::make_shared<CVSDebugSink>();
			return sink;
		}

		void Output(std::string sOut) override;
};

#endif