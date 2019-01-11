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
	pugi::xml_parse_result res = doc->load_file(CVirtualFileSystem::GetInstance()->ResolvePhysicalPath(sPath).c_str());



	return doc;
}