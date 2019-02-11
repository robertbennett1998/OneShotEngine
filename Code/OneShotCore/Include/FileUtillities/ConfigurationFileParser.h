#pragma once
#ifndef ONE_SHOT_CORE_CONFIGURATION_FILE_PARSER_H
#define ONE_SHOT_CORE_CONFIGURATION_FILE_PARSER_H

#include "Vendor/pugixml/src/pugixml.hpp"

class ONE_SHOT_CORE_DLL CConfigurationFileParser
{
	private:
		struct ConfigItem
		{
			bool bIsCVar;
			std::string sType;
			char* pValue;
		};

	public:
		CConfigurationFileParser();
		~CConfigurationFileParser();

		bool Load(std::string sFilePath);

	private:
		pugi::xml_document configDoc;

		inline void ReadDocumentNodes(pugi::xml_document& doc, std::map<std::string, std::string> &vars);
		inline void ReadChildNodes(pugi::xml_node_iterator node, std::map<std::string, std::string> &vars, std::string startingKey);

		char* AutoConvertToType(std::string value, std::string type)
		{
			char* pValue = nullptr;

			if (type == "bool")
			{
				bool* p = new bool;
				*p = (char*)(value == "true" ? true : false);
				pValue = (char*)p;
			}
			else if (type == "int")
			{
				int* p = new int;
				*p = std::stoi(value);
				pValue = (char*)p;
			}
			else if (type == "long")
			{
				long* p = new long;
				*p = std::stol(value);
				pValue = (char*)p;
			}
			else if (type == "longlong")
			{
				long long* p = new long long;
				*p = std::stoll(value);
				pValue = (char*)p;
			}
			else if (type == "ulong")
			{
				unsigned long* p = new unsigned long;
				*p = std::stoul(value);
				pValue = (char*)p;
			}
			else if (type == "ulonglong")
			{
				unsigned long long* p = new unsigned long long;
				*p = std::stoull(value);
				pValue = (char*)p;
			}
			else if (type == "float")
			{
				float* p = new float;
				*p = std::stof(value);
				pValue = (char*)p;
			}
			else if (type == "double")
			{
				double* p = new double;
				*p = std::stod(value);
				pValue = (char*)p;
			}
			else if (type == "longdouble")
			{
				long double* p = new long double;
				*p = std::stold(value);
				pValue = (char*)p;
			}

			return pValue;
		}
};

#endif