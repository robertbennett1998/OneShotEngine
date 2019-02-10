#pragma once
#ifndef ONE_SHOT_CORE_CONFIGURATION_FILE_PARSER_H
#define ONE_SHOT_CORE_CONFIGURATION_FILE_PARSER_H

#include "Vendor/pugixml/src/pugixml.hpp"

class ONE_SHOT_CORE_DLL CConfigurationFileParser
{
	public:
		CConfigurationFileParser();
		~CConfigurationFileParser();

		bool Load(std::string sFilePath);

	private:
		pugi::xml_document configDoc;

		inline void ReadDocumentNodes(pugi::xml_document& doc, std::map<std::string, std::string> &vars);
		inline void ReadChildNodes(pugi::xml_node_iterator node, std::map<std::string, std::string> &vars, std::string startingKey);
		template<class T>
		void AutoConvertToType(std::string value, std::string type, T* outValue)
		{
			if (type == "bool")
			{
				outValue = value == "true" ? true : false;
			}
			else if (type == "int")
			{
				outValue = std::stoi(value);
			}
			else if (type == "long")
			{
				outValue = std::stol(value);
			}
			else if (type == "longlong")
			{
				outValue = std::stoll(value);
			}
			else if (type == "ulong")
			{
				outValue = std::stoul(value);
			}
			else if (type == "ulonglong")
			{
				outValue = std::stoull(value);
			}
			else if (type == "float")
			{
				outValue = std::stof(value);
			}
			else if (type == "double")
			{
				outValue = std::stod(value);
			}
			else if (type == "longdouble")
			{
				outValue = std::stold(value);
			}
		}
};

#endif