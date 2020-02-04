#include "CoreHeaders.h"
#include "Logging/Sinks/FileSink.h"
#include "FileUtillities/VirtualFileSystem.h"

CFileSink::CFileSink()
{
}

CFileSink::~CFileSink()
{
	if (m_ofLogStream.is_open())
		m_ofLogStream.close();
}

std::shared_ptr<CFileSink> CFileSink::CreateFileSink(std::string sFilePath)
{
	std::shared_ptr<CFileSink> sink = std::make_shared<CFileSink>();
	if (!sink->Initialize(sFilePath))
	{
		OSE_LOG_WARNING("General", "Couldn't create file sink with the following parameters:\n\tFile Path: % \n\tFileName: %", sFilePath);
		return nullptr;
	}

	OSE_DEBUG_LOG_INFO("General", "File sink created with the following parameters:\n\tFile Path: % \n\tFileName: %", sFilePath);
	return sink;
}

bool CFileSink::Initialize(std::string sFilePath)
{
	//Open the file and make sure it is cleared when the file sink is created
	if (!CVirtualFileSystem::GetInstance()->CreateFileStream(sFilePath, m_ofLogStream))
		return false;

	return true;
}

void CFileSink::Output(std::string sOut)
{
	m_ofLogStream << sOut;
	m_ofLogStream.flush();
}