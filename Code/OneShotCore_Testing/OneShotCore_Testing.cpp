#include "pch.h"
#include "FileUtillities/ConfigurationFileParser.h"

using namespace std;

int main()
{
	CConfigurationFileParser configFile;
	configFile.Load("/AppDataStore/Engine.config");

	system("PAUSE");
	return 0;
}