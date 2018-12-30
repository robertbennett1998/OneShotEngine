#include "pch.h"
#include <Logging/Sinks/FileSink.h>
#include "Logging/Sinks/VSDebugSink.h"
#include <thread>

using namespace std;

class CWin32ConsoleSink : public ILogSink
{
	public:
		CWin32ConsoleSink()
		{
			return;
		}

		~CWin32ConsoleSink()
		{
			return;
		}

		static std::shared_ptr<CWin32ConsoleSink> CreateWin32ConsoleSink()
		{
			std::shared_ptr<CWin32ConsoleSink> sink = std::make_shared<CWin32ConsoleSink>();
			return sink;
		}

		void Output(std::string sOut) override
		{
			cout << sOut << "\n";
		}
};

void printEverySecond()
{
	CStopwatch stopwatch;
	stopwatch.Start();
	__int64 prevTime = 0, currTime = stopwatch.GetTimeElapsed();
	while(currTime < 100)
	{
		if (currTime - prevTime >= 1.0)
		{
			std::cout << "Second elapsed\n";
			prevTime = currTime;
		}

		currTime = stopwatch.GetTimeElapsed();
	}
}

int main()
{
	CMemoryManager* pMemMgr = CMemoryManager::GetInstance();

	CVirtualFileSystem* pVFS = CVirtualFileSystem::GetInstance();
	pVFS->Mount("/Root", "C:\\Programming\\OneShotEngine\\");
	pVFS->Mount("/Logs", "/Root/Logs");

	OSE_DEBUG_CREATE_LOGGER("Memory", memoryLogger);
	OSE_DEBUG_ADD_SINK("Memory", CFileSink::CreateFileSink("/Logs", "Memory.log"));
	OSE_DEBUG_ADD_SINK("Memory", CVSDebugSink::CreateVSDebugSink());
	OSE_DEBUG_ADD_SINK("Memory", CWin32ConsoleSink::CreateWin32ConsoleSink());

	OSE_DEBUG_LOG_INFO("Memory", pMemMgr->GetRootHeap()->WriteHeapDetailsToString().c_str());

	int* pIntArray =  OSE_NEW_ARRAY(int, 20, "root");
	pIntArray = OSE_NEW_ARRAY(int, 20);
	pIntArray = OSE_NEW_ARRAY(int, 20);
	pIntArray = OSE_NEW_ARRAY(int, 20);
	pIntArray = OSE_NEW_ARRAY(int, 20);
	pIntArray = OSE_NEW_ARRAY(int, 20);

	OSE_DEBUG_LOG_INFO("Memory", pMemMgr->GetRootHeap()->WriteHeapDetailsToString().c_str());

	OSE_DELETE_ARRAY(pIntArray);
	pIntArray = nullptr;

	OSE_DEBUG_LOG_INFO("Memory", pMemMgr->GetRootHeap()->WriteHeapDetailsToString().c_str());

	system("PAUSE");
	return 0;
}