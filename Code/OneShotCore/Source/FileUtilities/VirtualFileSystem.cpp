#include "CoreHeaders.h"
#include "FileUtillities/VirtualFileSystem.h"
#include <sys/stat.h>
#include <filesystem>

CVirtualFileSystem* CVirtualFileSystem::sm_pInstance = nullptr;

CVirtualFileSystem* CVirtualFileSystem::GetInstance()
{
	if (sm_pInstance == nullptr)
	{
		sm_pInstance = new CVirtualFileSystem();
		std::string sAppDataStorePhysicalPath = GetAppDataStoreFilePath();
		if(sAppDataStorePhysicalPath == "")
		{
			sAppDataStorePhysicalPath = CreateAppDataStoreFilePath();
			if(sAppDataStorePhysicalPath == "")
			{
				OSE_LOG_FATAL("General", "Cannot resolve appdata filepath!");
				throw;
			}
		}

		sm_pInstance->Mount("/AppDataStore", sAppDataStorePhysicalPath);
	}
	return sm_pInstance;
}

bool CVirtualFileSystem::Mount(std::string sVirtualPath, std::string sPath)
{
	if (sVirtualPath[0] != '/')
	{
		OSE_LOG_WARNING("General", "Virtual path(%) is invalid as it doesn't begin with a '/'", sVirtualPath);
		return false;
	}

	sPath = ResolvePhysicalPath(sPath);

	sVirtualPath += "\0";
	//sPath += "\0";

	auto mountIter = m_Mounts.find(sVirtualPath);
	if (mountIter != m_Mounts.end())
	{
		OSE_LOG_WARNING("General", "Physical path(%) couldn't be mounted to virtual path(%), as virtual path already has a mounted physical path!", sPath, sVirtualPath);
		return false;
	}

	if(DoesDirectoryPathExist(sPath))
	{
		m_Mounts.try_emplace(sVirtualPath, sPath);
		OSE_LOG_INFO("General", "Physical path(%) mounted to virtual path(%) !", sPath, sVirtualPath);
		return true;
	}

	OSE_LOG_WARNING("General", "Physical path(%) couldn't be mounted to virtual path(%), as physical path doesn't exist!", sPath, sVirtualPath);
	return false;
}

bool CVirtualFileSystem::Unmount(std::string sVirtualPath)
{
	auto mountIter = m_Mounts.find(sVirtualPath);
	if (mountIter == m_Mounts.end())
		return false;

	OSE_LOG_INFO("General", "Physical path(%) unmounted from virtual path(%) !", mountIter->second, sVirtualPath);
	m_Mounts.erase(mountIter);

	return true;
}

std::string CVirtualFileSystem::ResolvePhysicalPath(std::string sPath) const
{
	if (sPath[0] != '/')
		return sPath;

	size_t szPos = sPath.find_first_of('/', 1);
	std::string sBaseVirtualDir = sPath.substr(0, szPos);

	auto mountIter = m_Mounts.find(sBaseVirtualDir);
	if (mountIter == m_Mounts.end())
		return "";

	if (sBaseVirtualDir == sPath)
		return mountIter->second;

	size_t szLastDot = sPath.find_last_of('.');

	bool bIsFile = false;
	if (szLastDot != (size_t)-1)
	{
		std::string sExt = sPath.substr(szLastDot, sPath.length() - szLastDot);

		size_t szSlash = sExt.find_first_of('/');
		szSlash |= sExt.find_first_of('\\');

		if(szSlash == (size_t)-1)
			bIsFile = true;
	}

	if (bIsFile)
	{
		std::string sResolvedPath = mountIter->second + sPath.erase(0, szPos + 1);
		if (!DoesFilePathExist(sResolvedPath))
		{
			OSE_DEBUG_LOG_WARNING("General", "File with resolved path(%) doesn't exist", sResolvedPath);
			return "";
		}

		return sResolvedPath;
	}

	std::string sResolvedPath = mountIter->second + sPath.erase(0, szPos + 1) + "\\";
	if (!DoesFilePathExist(sResolvedPath))
	{
		OSE_DEBUG_LOG_WARNING("General", "Directory with resolved path(%) doesn't exist", sResolvedPath);
		return "";
	}

	return sResolvedPath;
}

bool CVirtualFileSystem::CreateFileStream(std::string sPath, std::fstream& fOut, std::ios_base::openmode openMode)
{
	std::string sPhysicalPath = ResolvePhysicalPath(sPath);
	fOut = std::fstream(sPhysicalPath, openMode);

	if (!fOut.is_open())
	{
		OSE_LOG_WARNING("General", "Couldn't open std::fstream: % \n\Argument path: %\n", sPhysicalPath, sPath);
		return false;
	}

	OSE_LOG_INFO("General", "std::fstream opened: % \n\Argument path: %\n", sPhysicalPath, sPath);
	return true;
}

bool CVirtualFileSystem::CreateFileStream(std::string sPath, std::ifstream& fOut, std::ios_base::openmode openMode)
{
	std::string sPhysicalPath = ResolvePhysicalPath(sPath);
	fOut = std::ifstream(sPhysicalPath, openMode);

	if (!fOut.is_open())
	{
		OSE_LOG_WARNING("General", "Couldn't open std::ifstream: % \nArgument path: %\n", sPhysicalPath, sPath);
		return false;
	}

	OSE_LOG_INFO("General", "std::ifstream opened: % \n\Argument path: %", sPhysicalPath, sPath);
	return true;
}

bool CVirtualFileSystem::CreateFileStream(std::string sPath, std::ofstream& fOut, std::ios_base::openmode openMode)
{
	std::string sPhysicalPath = ResolvePhysicalPath(sPath);
	fOut = std::ofstream(sPhysicalPath, openMode);

	if (!fOut.is_open())
	{
		OSE_LOG_WARNING("General", "Couldn't open std::ofstream: % \n\Argument path: %\n", sPhysicalPath, sPath);
		return false;
	}

	OSE_LOG_INFO("General", "std::ofstream opened: % \n\Argument path: %\n", sPhysicalPath, sPath);
	return true;
}

bool CVirtualFileSystem::DoesDirectoryPathExist(std::string sPath)
{
	struct stat info;
	if (stat(sPath.c_str(), &info) != 0)
	{
		return false;
	}
	else if (info.st_mode & S_IFDIR)
	{
		return true;
	}
}

bool CVirtualFileSystem::DoesFilePathExist(std::string sPath)
{
	struct stat info;
	if (stat(sPath.c_str(), &info) != 0)
	{
		return false;
	}
	else if (info.st_mode & S_IFREG)
	{
		return true;
	}
}