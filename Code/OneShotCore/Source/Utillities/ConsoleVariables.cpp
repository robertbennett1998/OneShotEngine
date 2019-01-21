#include "CoreHeaders.h"
#include "Utillities/ConsoleVariables.h"
#include "MemoryManagement/FreeAllocationHeap.h"

CConsoleVariables::CConsoleVariables()
{
	IHeap* pHeap = CMemoryManager::GetInstance()->CreateHeap<CFreeAllocationHeap>("CVars", 1, CMemoryManager::GetInstance()->GetRootHeap());
}

CConsoleVariables::~CConsoleVariables()
{
}