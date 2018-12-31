#include "CoreHeaders.h"
#include "FileUtillities/VirtualFileSystem.h"
#include <sys/stat.h>
#include <filesystem>
//#include "Logging/LoggerManager.h"

CVirtualFileSystem* CVirtualFileSystem::sm_pInstance = nullptr;

CVirtualFileSystem* CVirtualFileSystem::GetInstance()
{
	if (sm_pInstance == nullptr)
	{
		sm_pInstance = new CVirtualFileSystem();
		sm_pInstance->Mount("/Root", "C:\\Programming\\OneShotEngine\\");
	}
	return sm_pInstance;
}

bool CVirtualFileSystem::Mount(std::string sVirtualPath, std::string sPath)
{
	if (sVirtualPath[0] != '/')
	{
		//OSE_DEBUG_LOG_WARNING("General", "Virtual path(%) is invalid as it doesn't begin with a '/'", sVirtualPath);
		return false;
	}

	sPath = ResolvePhysicalPath(sPath);

	sVirtualPath += "\0";
	//sPath += "\0";

	auto mountIter = m_Mounts.find(sVirtualPath);
	if (mountIter != m_Mounts.end())
	{
		//OSE_DEBUG_LOG_WARNING("General", "Physical path(%) couldn't be mounted to virtual path(%), as virtual path already has a mounted physical path!", sPath, sVirtualPath);
		return false;
	}

	if(DoesPathExist(sPath))
	{
		m_Mounts.try_emplace(sVirtualPath, sPath);
		//OSE_DEBUG_LOG_INFO("General", "Physical path(%) mounted to virtual path(%) !", sPath, sVirtualPath);
		return true;
	}

	//OSE_DEBUG_LOG_WARNING("General", "Physical path(%) couldn't be mounted to virtual path(%), as physical path doesn't exist!", sPath, sVirtualPath);
	return false;
}

bool CVirtualFileSystem::Unmount(std::string sVirtualPath)
{
	auto mountIter = m_Mounts.find(sVirtualPath);
	if (mountIter == m_Mounts.end())
		return false;

	//OSE_DEBUG_LOG_INFO("General", "Physical path(%) unmounted from virtual path(%) !", mountIter->second, sVirtualPath);
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

	return mountIter->second + sPath.erase(0, szPos + 1) + "\\";
}

bool CVirtualFileSystem::CreateFileStream(std::string sPath, std::string sFileName, std::fstream& fOut, std::ios_base::openmode openMode)
{
	std::string sPhysicalPath = ResolvePhysicalPath(sPath) + sFileName;
	fOut = std::fstream(sPhysicalPath, openMode);

	if (!fOut.is_open())
	{
		//OSE_DEBUG_LOG_WARNING("General", "Couldn't open std::fstream: % \n\tVirtual path: % \n\tFile name %", sPhysicalPath, sPath, sFileName);
		return false;
	}

	//OSE_DEBUG_LOG_INFO("General", "std::fstream opened: % \n\tVirtual path: % \n\tFile name %", sPhysicalPath, sPath, sFileName);
	return true;
}

bool CVirtualFileSystem::CreateFileStream(std::string sPath, std::string sFileName, std::ifstream& fOut, std::ios_base::openmode openMode)
{
	std::string sPhysicalPath = ResolvePhysicalPath(sPath) + sFileName;
	fOut = std::ifstream(sPhysicalPath, openMode);

	if (!fOut.is_open())
	{
		//OSE_DEBUG_LOG_WARNING("General", "Couldn't open std::ifstream: % \n\tVirtual path: % \n\tFile name %", sPhysicalPath, sPath, sFileName);
		return false;
	}

	//OSE_DEBUG_LOG_INFO("General", "std::ifstream opened: % \n\tVirtual path: % \n\tFile name %", sPhysicalPath, sPath, sFileName);
	return true;
}

bool CVirtualFileSystem::CreateFileStream(std::string sPath, std::string sFileName, std::ofstream& fOut, std::ios_base::openmode openMode)
{
	std::string sPhysicalPath = ResolvePhysicalPath(sPath) + sFileName;
	fOut = std::ofstream(sPhysicalPath, openMode);

	if (!fOut.is_open())
	{
		//OSE_DEBUG_LOG_WARNING("General", "Couldn't open std::ofstream: %\t\n\tVirtual path: %\t\n\tFile name %", sPhysicalPath, sPath, sFileName);
		return false;
	}

	//OSE_DEBUG_LOG_INFO("General", "std::ofstream opened: %\t\n Virtual path: %\t\n File name %", sPhysicalPath, sPath, sFileName);
	return true;
}

bool CVirtualFileSystem::DoesPathExist(std::string sPath) const
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