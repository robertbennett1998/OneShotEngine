#include "CoreHeaders.h"
#include "Utillities/GlobalVariables.h"
#include "MemoryManagement/FreeAllocationHeap.h"

CGlobalVariables::CGlobalVariables()
{
	IHeap* pHeap = CMemoryManager::GetInstance()->CreateHeap<CFreeAllocationHeap>("CVars", 1, CMemoryManager::GetInstance()->GetRootHeap());
}

CGlobalVariables::~CGlobalVariables()
{
}