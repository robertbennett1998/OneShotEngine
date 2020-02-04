#pragma once
#ifndef ONE_SHOT_CORE_LOGGER_H
#define ONE_SHOT_CORE_LOGGER_H

#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <sstream>
#include <string>

#include "Sinks/Interfaces/ILogSink.h"

class ONE_SHOT_CORE_DLL CLogger
{
	public:
		CLogger(std::string sLoggerName);

		template<class T>
		bool AddSink(std::shared_ptr<T> sink)
		{
			std::shared_ptr<ILogSink> baseSink = std::static_pointer_cast<ILogSink>(sink);
			if (baseSink == nullptr)
				return false;

			if (!m_Sinks.empty())
			{
				for (size_t i = 0; i < m_Sinks.size(); i++)
				{
					if (m_Sinks[i] == baseSink)
					{
						return false;
					}
				}
			}

			m_Sinks.push_back(baseSink);
			return true;
		}

		template<class T>
		bool RemoveSink(std::shared_ptr<T>* sink)
		{
			std::shared_ptr<ILogSink> baseSink = std::static_pointer_cast<ILogSink>(sink);
			
			if(!m_Sinks.empty())
			{
				for(size_t i = 0; i < m_Sinks.size(); i++)
				{
					if (m_Sinks[i] == baseSink)
					{
						m_Sinks.erase(m_Sinks.begin() + i);
						return true;
					}
				}
			}

			return false;
		}

		template<class ...Args>
		void LogMessage(std::string sFmt, Args... args)
		{
			if (m_Sinks.size() == 0)
				return;

			const std::string sOut = Format(sFmt, "Message", args...);
			for (auto sink : m_Sinks)
				sink->Output(sOut);

			for (auto sib : m_Siblings)
				sib->LogMessage(sOut);
		}

		template<class ...Args>
		void LogInfo(std::string sFmt, Args... args)
		{
			if (m_Sinks.size() == 0)
				return;

			const std::string sOut = Format(sFmt, "Info", args...);
			for (auto sink : m_Sinks)
				sink->Output(sOut);

			for (auto sib : m_Siblings)
				sib->LogInfo(sOut);
		}

		template<class ...Args>
		void LogWarning(std::string sFmt, Args... args)
		{
			if (m_Sinks.size() == 0)
				return;

			const std::string sOut = Format(sFmt, "Warning", args...);
			for (auto sink : m_Sinks)
				sink->Output(sOut);

			for (auto sib : m_Siblings)
				sib->LogWarning(sOut);
		}

		template<class ...Args>
		void LogError(std::string sFmt, Args... args)
		{
			if (m_Sinks.size() == 0)
				return;

			const std::string sOut = Format(sFmt, "Error", args...);
			for (auto sink : m_Sinks)
				sink->Output(sOut);

			for (auto sib : m_Siblings)
				sib->LogError(sOut);
		}

		template<class ...Args>
		void LogFatal(std::string sFmt, Args... args)
		{
			if (m_Sinks.size() == 0)
				return;

			const std::string sOut = Format(sFmt, "Fatal", args...);
			for (auto sink : m_Sinks)
				sink->Output(sOut);

			for (auto sib : m_Siblings)
				sib->LogFatal(sOut);
		}

		bool AddSibling(std::shared_ptr<CLogger> logger);
		bool RemoveSibling(std::shared_ptr<CLogger> logger);

	private:
		inline std::string GetCurrentLocalTime();

		template<class ...Args>
		std::string Format(std::string fmt, std::string sMsgLevel, Args... args)
		{
			std::stringstream ssOut;
			ssOut << sMsgLevel << ": " << GetCurrentLocalTime() + " - " + FillChars(fmt, args...) << "\n";
			return ssOut.str();
		}

		std::string FillChars(std::string fmt)
		{
			return fmt;
		}

		template<class T, class ...Args>
		std::string FillChars(std::string fmt, T firstArg, Args... args)
		{
			for(size_t i = 0; i < fmt.size(); i++)
			{
				if (fmt[i] == '%')
				{
					fmt.erase(i, 1);
					std::stringstream ss;
					ss << firstArg;
					fmt.insert(i, ss.str());
					fmt = FillChars(fmt, args...);
				}
			}

			return fmt;
		}

		CLogger() { return; }

		std::string m_sLoggerName;
		std::vector<std::shared_ptr<CLogger>> m_Siblings;
		std::vector<std::shared_ptr<ILogSink>> m_Sinks;
		std::chrono::system_clock m_SystemClock;
};

#endif
