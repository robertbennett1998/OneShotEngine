#pragma once
#ifndef ONE_SHOT_CORE_FILE_SINK_H
#define ONE_SHOT_CORE_FILE_SINK_H

#include "Interfaces/ILogSink.h"

class ONE_SHOT_CORE_DLL CFileSink : public ILogSink
{
	public:
		CFileSink();
		~CFileSink();

		static std::shared_ptr<CFileSink> CreateFileSink(std::string sFilePath, std::string sFileName);

		void Output(std::string sOut) override;

	private:
		bool Initialize(std::string sFilePath, std::string sFileName);

		std::ofstream m_ofLogStream;
};

#endif