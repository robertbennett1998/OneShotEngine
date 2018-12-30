#include "CoreHeaders.h"
#include "Logging/LoggingManager.h"
#include "Logging/Sinks/VSDebugSink.h"

CLoggingManager* CLoggingManager::sm_pInstance = nullptr;

CLoggingManager* CLoggingManager::GetInstance()
{
	if (sm_pInstance == nullptr)
	{
		//As its a singleton dont track the allocation
		sm_pInstance = new CLoggingManager();
		OSE_DEBUG_CREATE_LOGGER("General", genLogger);
		genLogger->AddSink(CVSDebugSink::CreateVSDebugSink());
	}

	return sm_pInstance;
}
