#include "pch.h"
#include "FileUtillities/ConfigurationFileParser.h"
#include "Logging/Sinks/FileSink.h"

using namespace std;

int main()
{
	CGlobalVariables globalEnviroment;
	CConfigurationFileParser configFile(&globalEnviroment);
	configFile.Load("/AppDataStore/Engine.config");
	std::string sRootVirtualPath = configFile.Get<std::string>("Configuration.VirtualPaths.Root");
	CVirtualFileSystem::GetInstance()->Mount("/Root", sRootVirtualPath);

	std::vector<std::pair<std::string, std::string>> virtualPaths = configFile.GetAll<std::string>("Configuration.VirtualPaths.*");

	for (int i = 0; i < virtualPaths.size(); i++)
	{
		size_t szLastDotPos = virtualPaths[i].first.find_last_of('.') + 1;
		if (szLastDotPos != std::string::npos)
		{
			OSE_LOG_INFO("General", "Retrived virtual path % - %", virtualPaths[i].first, virtualPaths[i].second);
			std::string sVirtualPathName = "/" + virtualPaths[i].first.erase(0, szLastDotPos);
			CVirtualFileSystem::GetInstance()->Mount(sVirtualPathName, virtualPaths[i].second);
		}
	}

	OSE_ADD_SINK("General", CFileSink::CreateFileSink("/Root/Logs", "General.log"));
	OSE_LOG_INFO("General", "Test");


	int height = 0;
	shared_ptr<int> pHeight = globalEnviroment.GetVariable<int>("Configuration.Window.Height");
	*pHeight = 1400;

	system("PAUSE");
	return 0;
}