#pragma once
#ifndef ONE_SHOT_CORE_XML_READER_H
#define ONE_SHOT_CORE_CORE_HEADERS_H

#define PUGIXML_HEADER_ONLY

#include "Vendor/pugixml/src/pugiconfig.hpp"
#include "Vendor/pugixml/src/pugixml.hpp"

class ONE_SHOT_CORE_DLL CXmlReader
{
	public:
		CXmlReader();
		~CXmlReader();

		std::shared_ptr<pugi::xml_document> LoadDocument(std::string sPath);
};

#endif