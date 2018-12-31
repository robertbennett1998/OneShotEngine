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
			genLogger->AddSink(CFileSink::CreateFileSink("/Logs", "General.log"));
			OSE_DEBUG_LOG_WARNING("General", "File sink created! Messages may of been logged before and/or during the creation of this sink!");
		}
	}

	return sm_pInstance;
}