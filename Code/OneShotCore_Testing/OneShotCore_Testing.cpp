#include "pch.h"

using namespace std;

int main()
{
	CVirtualFileSystem::GetInstance()->Mount("/bin", "/Root/bin");
	system("PAUSE");
	OSE_DEBUG_LOG_INFO("General", CMemoryManager::GetInstance()->GetRootHeap()->WriteHeapDetailsToString());
	return 0;
}