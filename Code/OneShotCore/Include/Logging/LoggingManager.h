#pragma once
#ifndef ONE_SHOT_CORE_LOGGER_MANAGER_H
#define ONE_SHOT_CORE_LOGGER_MANAGER_H

#include "Logger.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <map>

#define EXPAND(x) x
#define OSE_GET_CREATE_LOGGER_MACRO(_1,_2,NAME,...) NAME
#define OSE_CREATE_LOGGER_1(LoggerName) CLoggingManager::GetInstance()->CreateLogger(LoggerName)
#define OSE_CREATE_LOGGER_2(LoggerName, var) auto var = CLoggingManager::GetInstance()->CreateLogger(LoggerName)
#define OSE_CREATE_LOGGER(...) EXPAND(OSE_GET_CREATE_LOGGER_MACRO(__VA_ARGS__, OSE_CREATE_LOGGER_2, OSE_CREATE_LOGGER_1)(__VA_ARGS__))
#define OSE_GET_LOGGER(LoggerName, loggerToCreate) auto loggerToCreate = CLoggingManager::GetInstance()->GetLogger(LoggerName)
#define OSE_ADD_SINK(LoggerName, sink) CLoggingManager::GetInstance()->GetLogger(LoggerName)->AddSink(sink)
#define OSE_REMOVE_SINK(LoggerName, sink) CLoggingManager::GetInstance()->GetLogger(LoggerName)->RemoveSink(sink)
#define OSE_ADD_SIBLING(LoggerName, SiblingLoggerName) CLoggingManager::GetInstance()->GetLogger(LoggerName)->AddSibling(CLoggingManager::GetInstance()->GetLogger(SiblingLoggerName))
#define OSE_REMOVE_SIBLING(LoggerName, SiblingLoggerName) CLoggingManager::GetInstance()->GetLogger(LoggerName)->RemoveSibling(CLoggingManager::GetInstance()->GetLogger(SiblingLoggerName))
#define OSE_REMOVE_LOGGER(LoggerName) CLoggingManager::GetInstance()->RemoveLogger(LoggerName)
#define OSE_LOG_MESSAGE(LoggerName, ...) CLoggingManager::GetInstance()->GetLogger(LoggerName)->LogMessage(__VA_ARGS__)
#define OSE_LOG_INFO(LoggerName, ...) CLoggingManager::GetInstance()->GetLogger(LoggerName)->LogInfo(__VA_ARGS__)
#define OSE_LOG_WARNING(LoggerName, ...) CLoggingManager::GetInstance()->GetLogger(LoggerName)->LogWarning(__VA_ARGS__)
#define OSE_LOG_ERROR(LoggerName, ...) CLoggingManager::GetInstance()->GetLogger(LoggerName)->LogError(__VA_ARGS__)
#define OSE_LOG_FATAL(LoggerName, ...) CLoggingManager::GetInstance()->GetLogger(LoggerName)->LogFatal(__VA_ARGS__)

#ifdef _DEBUG
	#define OSE_DEBUG_CREATE_LOGGER(...) EXPAND(OSE_GET_CREATE_LOGGER_MACRO(__VA_ARGS__, OSE_CREATE_LOGGER_2, OSE_CREATE_LOGGER_1)(__VA_ARGS__))
	#define OSE_DEBUG_GET_LOGGER(LoggerName, loggerToCreate) auto loggerToCreate = CLoggingManager::GetInstance()->GetLogger(LoggerName)
	#define OSE_DEBUG_ADD_SINK(LoggerName, sink) CLoggingManager::GetInstance()->GetLogger(LoggerName)->AddSink(sink)
	#define OSE_DEBUG_REMOVE_SINK(LoggerName, sink) CLoggingManager::GetInstance()->GetLogger(LoggerName)->RemoveSink(sink)
	#define OSE_DEBUG_ADD_SIBLING(LoggerName, SiblingLoggerName) CLoggingManager::GetInstance()->GetLogger(LoggerName)->AddSibling(CLoggingManager::GetInstance()->GetLogger(SiblingLoggerName))
	#define OSE_DEBUG_REMOVE_SIBLING(LoggerName, SiblingLoggerName) CLoggingManager::GetInstance()->GetLogger(LoggerName)->RemoveSibling(CLoggingManager::GetInstance()->GetLogger(SiblingLoggerName))
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
	#define OSE_DEBUG_REMOVE_SINK(LoggerName, sink)
	#define OSE_DEBUG_ADD_SIBLING(LoggerName, SiblingLoggerName)
	#define OSE_DEBUG_REMOVE_SIBLING(LoggerName, SiblingLoggerName)
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

		std::shared_ptr<CLogger> CreateLogger(std::string sLoggerName);
		std::shared_ptr<CLogger> GetLogger(std::string sLoggerName) const;
		bool RemoveLogger(std::string sLoggerName);
		

	private:
		static CLoggingManager* sm_pInstance;

		std::map<std::string, std::shared_ptr<CLogger>> m_Loggers;
};

#endif