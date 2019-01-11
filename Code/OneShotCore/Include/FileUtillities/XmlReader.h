#pragma once
#ifndef ONE_SHOT_CORE_XML_READER_H
#define ONE_SHOT_CORE_CORE_HEADERS_H

#include "Vendor/pugixml/src/pugixml.hpp"

static class CXmlReader
{
	public:
		CXmlReader();
		~CXmlReader();

		static std::shared_ptr<pugi::xml_document> LoadDocument(std::string sPath);
};

#endif