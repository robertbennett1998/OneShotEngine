#include "CoreHeaders.h"
#include "MemoryManagement/MemoryManager.h"
#include "MemoryManagement/FreeAllocationHeap.h"
#include "Logging/Sinks/FileSink.h"

CMemoryManager* CMemoryManager::sm_pInstance = nullptr;

CMemoryManager* CMemoryManager::GetInstance()
{
	if (sm_pInstance == nullptr)
	{
		sm_pInstance = new CMemoryManager();
		sm_pInstance->m_pRootHeap = sm_pInstance->CreateHeap<CFreeAllocationHeap>("root", 8);

		auto memLogger = CLoggingManager::GetInstance()->CreateLogger("Memory");
		memLogger->AddSink(CFileSink::CreateFileSink("/Logs/Memory.log"));
		memLogger->AddSibling(CLoggingManager::GetInstance()->GetLogger("General"));
	} 

	return sm_pInstance;
}

IHeap* CMemoryManager::GetHeap(std::string sHeapName) const
{
	std::transform(sHeapName.begin(), sHeapName.end(), sHeapName.begin(), ::tolower);
	auto heapIter = m_pHeaps.find(sHeapName);
	if (heapIter == m_pHeaps.end())
		return nullptr;

	return heapIter->second;
}

void CMemoryManager::DumpHeapDetails()
{
}

#pragma region Overload Definitions
void* operator new(size_t szSize, std::string sHeapName)
{
	IHeap* pHeap = CMemoryManager::GetInstance()->GetHeap(sHeapName);
	if (pHeap == nullptr)
		throw; //TODO: Add exception type

	return pHeap->Allocate(szSize);
}

void* operator new [](size_t szSize, std::string sHeapName)
{
	IHeap* pHeap = CMemoryManager::GetInstance()->GetHeap(sHeapName);
	if (pHeap == nullptr)
		throw; //TODO: Add exception type

	return pHeap->Allocate(szSize);
}

void* operator new(size_t szSize, const uint32_t iLine, const char* pFileName, std::string sHeapName)
{
	IHeap* pHeap = CMemoryManager::GetInstance()->GetHeap(sHeapName);
	if (pHeap == nullptr)
		throw; //TODO: Add exception type

	return pHeap->Allocate(szSize, iLine, pFileName);
}

void* operator new[](size_t szSize, const uint32_t iLine, const char* pFileName, std::string sHeapName)
{
	IHeap* pHeap = CMemoryManager::GetInstance()->GetHeap(sHeapName);
	if (pHeap == nullptr)
		throw; //TODO: Add exception type

	return pHeap->Allocate(szSize, iLine, pFileName);
}
#pragma endregion