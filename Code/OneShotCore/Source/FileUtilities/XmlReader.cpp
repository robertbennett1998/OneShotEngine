#include "CoreHeaders.h"
#include "FileUtillities/XmlReader.h"

CXmlReader::CXmlReader()
{
}

CXmlReader::~CXmlReader()
{
}

std::shared_ptr<pugi::xml_document> CXmlReader::LoadDocument(std::string sPath)
{
	std::shared_ptr<pugi::xml_document> doc = std::make_shared<pugi::xml_document>();
	std::string sPhysicalPath = CVirtualFileSystem::GetInstance()->ResolvePhysicalPath(sPath).c_str();
	pugi::xml_parse_result res = doc->load_file(sPhysicalPath.c_str());

	if(res)
	{
		OSE_LOG_INFO("General", "Loaded xml file %!", sPhysicalPath);
	}
	else
	{
		OSE_LOG_WARNING("General", "Failed to load xml file %! With error %! At position %", sPhysicalPath, res.description(), res.offset);
	}

	return doc;
}