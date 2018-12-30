#pragma once
#ifndef ONE_SHOT_CORE_LOGGER_MANAGER_H
#define ONE_SHOT_CORE_LOGGER_MANAGER_H

#include "Logger.h"

#ifdef _DEBUG
	#define EXPAND(x) x
	#define OSE_GET_DEBUG_CREATE_LOGGER_MACRO(_1,_2,NAME,...) NAME
	#define OSE_DEBUG_CREATE_LOGGER_1(LoggerName) CLoggingManager::GetInstance()->CreateLogger(LoggerName)
	#define OSE_DEBUG_CREATE_LOGGER_2(LoggerName, var) auto var = CLoggingManager::GetInstance()->CreateLogger(LoggerName)
	/* Create Logger - Params:
	 * std::string - Logger name
	 * plain text - Variable name
	 */
	#define OSE_DEBUG_CREATE_LOGGER(...) EXPAND(OSE_GET_DEBUG_CREATE_LOGGER_MACRO(__VA_ARGS__, OSE_DEBUG_CREATE_LOGGER_2, OSE_DEBUG_CREATE_LOGGER_1)(__VA_ARGS__))
	#define OSE_DEBUG_GET_LOGGER(LoggerName, loggerToCreate) auto loggerToCreate = CLoggingManager::GetInstance()->GetLogger(LoggerName)
	#define OSE_DEBUG_ADD_SINK(LoggerName, sink) CLoggingManager::GetInstance()->GetLogger(LoggerName)->AddSink(sink)
	#define OSE_DEBUG_REMOVE_SINK(LoggerName, sink) CLoggingManager::GetInstance()->GetLogger(LoggerName)->RemoveSink(sink)
	#define OSE_DEBUG_REMOVE_LOGGER(LoggerName) CLoggingManager::GetInstance()->RemoveLogger(LoggerName)
	#define OSE_DEBUG_LOG_MESSAGE(LoggerName, ...) CLoggingManager::GetInstance()->GetLogger(LoggerName)->LogMessage(__VA_ARGS__)
	#define OSE_DEBUG_LOG_INFO(LoggerName, ...) CLoggingManager::GetInstance()->GetLogger(LoggerName)->LogInfo(__VA_ARGS__)
	#define OSE_DEBUG_LOG_WARNING(LoggerName, ...) CLoggingManager::GetInstance()->GetLogger(LoggerName)->LogWarning(__VA_ARGS__)
	#define OSE_DEBUG_LOG_ERROR(LoggerName, ...) CLoggingManager::GetInstance()->GetLogger(LoggerName)->LogError(__VA_ARGS__)
	#define OSE_DEBUG_LOG_FATAL(LoggerName, ...) CLoggingManager::GetInstance()->GetLogger(LoggerName)->LogFatal(__VA_ARGS__)
#else
	#define OSE_DEBUG_CREATE_LOGGER(...)
	#define OSE_DEBUG_GET_LOGGER(LoggerName, loggerToCreate)
	#define OSE_DEBUG_ADD_SINK(LoggerName, sink)
	#define OSE_DEBUG_REMOVE_LOGGER(LoggerName)
	#define OSE_DEBUG_LOG_MESSAGE(LoggerName, ...)
	#define OSE_DEBUG_LOG_INFO(LoggerName, ...)
	#define OSE_DEBUG_LOG_WARNING(LoggerName, ...)
	#define OSE_DEBUG_LOG_ERROR(LoggerName, ...)
	#define OSE_DEBUG_LOG_FATAL(LoggerName, ...)
#endif

class ONE_SHOT_CORE_DLL CLoggingManager
{
	public:
		static CLoggingManager* GetInstance();

		std::shared_ptr<CLogger> CreateLogger(std::string sLoggerName)
		{
			std::transform(sLoggerName.begin(), sLoggerName.end(), sLoggerName.begin(), ::tolower);
			const auto existingLoggerIter = m_Loggers.find(sLoggerName);
			if (existingLoggerIter != m_Loggers.end())
				return existingLoggerIter->second;

			auto logger = std::make_shared<CLogger>(sLoggerName);

			m_Loggers.try_emplace(sLoggerName, logger);

			return logger;
		}

		std::shared_ptr<CLogger> GetLogger(std::string sLoggerName) const
		{
			std::transform(sLoggerName.begin(), sLoggerName.end(), sLoggerName.begin(), ::tolower);
			const auto existingLoggerIter = m_Loggers.find(sLoggerName);
			if (existingLoggerIter == m_Loggers.end())
				return nullptr;

			return existingLoggerIter->second;
		}

		bool RemoveLogger(std::string sLoggerName)
		{
			std::transform(sLoggerName.begin(), sLoggerName.end(), sLoggerName.begin(), ::tolower);
			auto existingLoggerIter = m_Loggers.find(sLoggerName);
			if (existingLoggerIter == m_Loggers.end())
				return false;

			m_Loggers.erase(existingLoggerIter);

			return true;
		}

	private:
		static CLoggingManager* sm_pInstance;

		std::map<std::string, std::shared_ptr<CLogger>> m_Loggers;
};

#endif