#include "CoreHeaders.h"
#include "Logging/LoggingManager.h"
#include "Logging/Sinks/VSDebugSink.h"
#include "Logging/Sinks/FileSink.h"

CLoggingManager* CLoggingManager::sm_pInstance = nullptr;

CLoggingManager* CLoggingManager::GetInstance()
{
	if (sm_pInstance == nullptr)
	{
		//As its a singleton dont track the allocation
		sm_pInstance = new CLoggingManager();
		auto genLogger = sm_pInstance->CreateLogger("General");

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
		return nullptr;

	return existingLoggerIter->second;
}
bool CLoggingManager::RemoveLogger(std::string sLoggerName)
{
	std::transform(sLoggerName.begin(), sLoggerName.end(), sLoggerName.begin(), ::tolower);
	auto existingLoggerIter = m_Loggers.find(sLoggerName);
	if (existingLoggerIter == m_Loggers.end())
		return false;

	m_Loggers.erase(existingLoggerIter);

	return true;
}
