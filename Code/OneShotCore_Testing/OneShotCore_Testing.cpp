#include "pch.h"
#include "FileUtillities/XmlReader.h"

using namespace std;

int main()
{
	CVirtualFileSystem::GetInstance()->Mount("/Settings", "/Root/Settings");

	CXmlReader xmlReader;
	auto doc = xmlReader.LoadDocument("/Settings/General.xml");

	//std::string sInstallDirectory();

	//OSE_DEBUG_LOG_INFO("General", "Install Directory: %", sInstallDirectory);

	for(auto child : doc->child("Settings").child("Directories").children())
	{
		std::string sPathName = child.attribute("pathName").value();
		std::string sPath = child.attribute("path").value();
		OSE_DEBUG_LOG_INFO("General", "% : %", sPathName, sPath);
	}

	system("PAUSE");
	OSE_DEBUG_LOG_INFO("Memory", CMemoryManager::GetInstance()->GetRootHeap()->WriteHeapDetailsToString());
	return 0;
}