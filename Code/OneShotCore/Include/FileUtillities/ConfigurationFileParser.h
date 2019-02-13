#pragma once
#ifndef ONE_SHOT_CORE_CONFIGURATION_FILE_PARSER_H
#define ONE_SHOT_CORE_CONFIGURATION_FILE_PARSER_H

#include "Vendor/pugixml/src/pugixml.hpp"

class ONE_SHOT_CORE_DLL CConfigurationFileParser
{
	private:
		struct ConfigItem
		{
			std::string key;
			bool bIsCVar;
			std::string sType;
			char* pValue;
		};

	public:
		CConfigurationFileParser();
		~CConfigurationFileParser();

		bool Load(std::string sFilePath);

	private:
		pugi::xml_document m_ConfigDoc;

		inline void ReadDocumentNodes(pugi::xml_document& doc, std::map<std::string, std::string> &vars);
		inline void ReadChildNodes(pugi::xml_node_iterator node, std::map<std::string, std::string> &vars, std::string startingKey);

		char* AutoConvertToType(std::string value, std::string type);
};

#endif