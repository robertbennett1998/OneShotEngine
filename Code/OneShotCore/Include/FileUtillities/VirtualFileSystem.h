#pragma once
#ifndef ONE_SHOT_CORE_VIRTUAL_FILE_SYSTEM_H
#define ONE_SHOT_CORE_VIRTUAL_FILE_SYSTEM_H

class ONE_SHOT_CORE_DLL CVirtualFileSystem
{
	public:
		static CVirtualFileSystem* GetInstance();

		bool Mount(std::string sVirtualPath, std::string sPhysicalPath);
		bool Unmount(std::string sVirtualPath);
		std::string ResolvePhysicalPath(std::string sVirtualPath) const;

		bool CreateFileStream(std::string sPath, std::string sFileName, std::fstream& fOut, std::ios_base::openmode openMode = std::fstream::in | std::fstream::out);
		bool CreateFileStream(std::string sPath, std::string sFileName, std::ifstream& fOut, std::ios_base::openmode openMode = std::fstream::in);
		bool CreateFileStream(std::string sPath, std::string sFileName, std::ofstream& fOut, std::ios_base::openmode openMode = std::fstream::out);

		static bool DoesDirectoryPathExist(std::string sPath);
		static bool DoesFilePathExist(std::string sPath);

	private:
		CVirtualFileSystem() { return; }
		~CVirtualFileSystem() { return; }

		static CVirtualFileSystem* sm_pInstance;
		std::map<std::string, std::string> m_Mounts;
};

#endif