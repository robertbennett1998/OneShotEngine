#pragma once
#ifndef ONE_SHOT_CORE_CORE_HEADERS_H
#define ONE_SHOT_CORE_CORE_HEADERS_H

#include <Windows.h>
#include <ShlObj_core.h>
#include <comutil.h>

//stdlib
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <fstream>
#include <thread>
#include <tuple>

#ifdef _ONE_SHOT_CORE_DLL
	#define ONE_SHOT_CORE_DLL __declspec(dllexport)
#else
	#define ONE_SHOT_CORE_DLL __declspec(dllimport)
#endif

#define PUGIXML_HEADER_ONLY
#include "Vendor/pugixml/src/pugixml.hpp"

#include <MemoryManagement/MemoryManager.h>
#include <Logging/LoggingManager.h>
#include <Logging/Logger.h>
#include <FileUtillities/VirtualFileSystem.h>
#include "Utillities/GlobalVariables.h"

inline std::string GetAppDataStoreFilePath()
{
	LPWSTR  wszPath;
	HRESULT hRes = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &wszPath);
	std::stringstream ssPath;
	if (FAILED(hRes))
	{
		OSE_LOG_WARNING("General", "Cannot resolve appdata directory!");
		return "";
	}

	_bstr_t bstrPath(wszPath);
	ssPath << ((char*)bstrPath) << "\\Robert Bennett\\OneShotEngine\\";

	if (!CVirtualFileSystem::DoesDirectoryPathExist(ssPath.str()))
		return "";

	return ssPath.str();
}

inline std::string CreateAppDataStoreFilePath()
{
	LPWSTR  wszPath;
	HRESULT hRes = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &wszPath);
	
	if (FAILED(hRes))
	{
		OSE_LOG_ERROR("General", "Cannot resolve appdata directory!");
		return "";
	}

	_bstr_t bstrPath(wszPath);

	std::stringstream ssPath;
	ssPath << ((char*)bstrPath) << "\\Robert Bennett\\";

	bool bRes = CreateDirectoryA(ssPath.str().c_str(), NULL);
	if(!bRes)
	{
		const DWORD dwLastError = GetLastError();
		if(dwLastError == ERROR_ALREADY_EXISTS)
		{
			OSE_LOG_WARNING("General", "Couldn't create appdata store as it already exists");
		}
		else
		{
			OSE_LOG_WARNING("General", "Couldn't create appdata store as one or more intermitent directories do not exist");
		}

		return "";
	}

	ssPath << "OneShotEngine\\";
	bRes = CreateDirectoryA(ssPath.str().c_str(), NULL);
	if (!bRes)
	{
		const DWORD dwLastError = GetLastError();
		if (dwLastError == ERROR_ALREADY_EXISTS)
		{
			OSE_LOG_WARNING("General", "Couldn't create appdata store as it already exists");
		}
		else
		{
			OSE_LOG_WARNING("General", "Couldn't create appdata store as one or more intermitent directories do not exist");
		}
	}

	return ssPath.str();
}

#endif