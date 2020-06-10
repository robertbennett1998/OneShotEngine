#include "CoreHeaders.h"
#include "Logging/LoggingManager.h"
#include "Logging/Sinks/VSDebugSink.h"
#include "Logging/Sinks/FileSink.h"

CLoggingManager* CLoggingManager::sm_pInstance = nullptr;

CLoggingManager::CLoggingManager(const std::string sNullLoggerName, const std::string sGeneralLoggerName) :
m_sNullLoggerName(sNullLoggerName),
m_sGeneralLoggerName(sGeneralLoggerName)
{
}

CLoggingManager* CLoggingManager::GetInstance()
{
	if (sm_pInstance == nullptr)
	{
		const std::string sNullLoggerName = "null";
		const std::string sGeneralLoggerName = "general";
		//As its a singleton dont track the allocation
		sm_pInstance = new CLoggingManager(sNullLoggerName, sGeneralLoggerName);
		auto genLogger = sm_pInstance->CreateLogger(sGeneralLoggerName);
		sm_pInstance->CreateLogger(sNullLoggerName);
		
		genLogger->AddSink(CVSDebugSink::CreateVSDebugSink());
		//TODO: Log warning if it did not work
		if(CVirtualFileSystem::GetInstance()->Mount("/Logs", "/Root/Logs"))
		{
			genLogger->AddSink(CFileSink::CreateFileSink("/Logs/General.log"));
		}
	}

	return sm_pInstance;
}

std::shared_ptr<CLogger> CLoggingManager::CreateLogger(std::string sLoggerName)
{
	std::transform(sLoggerName.begin(), sLoggerName.end(), sLoggerName.begin(), ::tolower);
	const auto existingLoggerIter = m_Loggers.find(sLoggerName);
	if (existingLoggerIter != m_Loggers.end())
		return existingLoggerIter->second;

	auto logger = std::make_shared<CLogger>(sLoggerName);

	m_Loggers.try_emplace(sLoggerName, logger);

	return logger;
}

std::shared_ptr<CLogger> CLoggingManager::GetLogger(std::string sLoggerName) const
{
	std::transform(sLoggerName.begin(), sLoggerName.end(), sLoggerName.begin(), ::tolower);
	const auto existingLoggerIter = m_Loggers.find(sLoggerName);
	if (existingLoggerIter == m_Loggers.end())
	{
		OSE_LOG_WARNING("General", "Cannot find a logger with the name %. Returning a blank logger, this logger has no sinks.", sLoggerName);
		return GetLogger(m_sNullLoggerName);
	}
	
	return existingLoggerIter->second;
}
bool CLoggingManager::RemoveLogger(std::string sLoggerName)
{
	if (sLoggerName == m_sNullLoggerName || sLoggerName == m_sGeneralLoggerName)
	{
		OSE_LOG_MESSAGE("General", "Cannot remove the default loggers (% or %).", m_sGeneralLoggerName, m_sNullLoggerName);
		return false;
	}
	
	std::transform(sLoggerName.begin(), sLoggerName.end(), sLoggerName.begin(), ::tolower);
	auto existingLoggerIter = m_Loggers.find(sLoggerName);
	if (existingLoggerIter == m_Loggers.end())
		return false;

	m_Loggers.erase(existingLoggerIter);

	return true;
}