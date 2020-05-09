#pragma once
#ifndef ONE_SHOT_CORE_COUT_SINK_H
#define ONE_SHOT_CORE_COUT_SINK_H

#include "Interfaces/ILogSink.h"

class ONE_SHOT_CORE_DLL CCoutSink : public ILogSink
{
	public:
		CCoutSink();
		~CCoutSink();

		static std::shared_ptr<CCoutSink> CreateCoutSink()
		{
			std::shared_ptr<CCoutSink> sink = std::make_shared<CCoutSink>();
			return sink;
		}

		void Output(std::string sOut) override;
};

#endif