#include "CoreHeaders.h"
#include "FileUtillities/ConfigurationFileParser.h"

CConfigurationFileParser::CConfigurationFileParser()
{
}

CConfigurationFileParser::~CConfigurationFileParser()
{
}

bool CConfigurationFileParser::Load(std::string sFilePath)
{
	std::string sPhysicalPath = CVirtualFileSystem::GetInstance()->ResolvePhysicalPath(sFilePath);

	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_file(CVirtualFileSystem::GetInstance()->ResolvePhysicalPath(sPhysicalPath).c_str(), pugi::parse_trim_pcdata);

	if (res)
	{
		OSE_DEBUG_LOG_INFO("General", "Loaded configuration file %!", sPhysicalPath);
	}
	else
	{
		OSE_LOG_WARNING("General", "Failed to load xml file %! With error %! At position %", sPhysicalPath, res.description(), res.offset);
		return false;
	}

	std::map<std::string, std::string> keyValPairs;
	ReadDocumentNodes(doc, keyValPairs);

	if (keyValPairs.empty())
	{
		OSE_LOG_WARNING("General", "Could not load configuration file % as no key value pairs were found!", sPhysicalPath);
		return false;
	}

	std::map<std::string, std::vector<std::string>> configItemGroups;

	OSE_DEBUG_LOG_INFO("General", "Configuration loaded:");
	for (auto keyValPair : keyValPairs)
	{
		std::string sKey = keyValPair.first;
		OSE_DEBUG_LOG_INFO("General", "\t% = %", keyValPair.first, keyValPair.second);
	}

	std::vector<std::string> keys;
	for(auto keyValPair : keyValPairs)
	{
		std::string sKey = keyValPair.first;
		size_t szLastDotPos = sKey.find_last_of('.');
		if(szLastDotPos == std::string::npos)
		{
			keys.push_back(sKey);
		}

		std::string sLastPart = sKey.substr(szLastDotPos, sKey.length() - szLastDotPos);

		if (sLastPart == "cvar" || sLastPart == "type")
			continue;

		keys.push_back(sKey);
	}


	std::vector<ConfigItem> configItems;
	for (auto key : keys)
	{
		ConfigItem configItem;
		configItem.bIsCVar = keyValPairs[key + ".cvar"] == "true" ? true : false;
		configItem.sType = keyValPairs[key + ".type"];
		configItem.pValue = AutoConvertToType(keyValPairs[key], configItem.sType);
		configItem.key = key;

		if(configItem.pValue == nullptr)
		{
			OSE_LOG_WARNING("General", "Couldn't create config item for key %", key);
			continue;
		}

		configItems.push_back(configItem);
	}

	OSE_DEBUG_LOG_INFO("General", "Config created with the following items: ");
	for(auto configItem : configItems)
	{
		OSE_DEBUG_LOG_INFO("General", "% - IsCVar: % - Type: % - pValue: %", configItem.key, configItem.bIsCVar, configItem.sType, configItem.pValue);
	}

	return false;
}

void CConfigurationFileParser::ReadDocumentNodes(pugi::xml_document& doc, std::map<std::string, std::string> &vars)
{
	std::string startingKey = "";

	for (pugi::xml_node_iterator it = doc.root().children().begin(); it != doc.root().children().end(); ++it)
	{
		std::string currKey = startingKey;
		if (it->type() == pugi::xml_node_type::node_pcdata || it->type() == pugi::xml_node_type::node_cdata)
		{
			vars.try_emplace(currKey, it->value());
		}
		else
		{
			if (startingKey == "")
			{
				startingKey = it->name();
				currKey = startingKey;
			}
			else
				currKey += "." + std::string(it->name());
		}

		for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
		{
			vars.try_emplace(currKey + "." + ait->name(), it->value());
		}

		ReadChildNodes(it, vars, currKey);
	}
}

void CConfigurationFileParser::ReadChildNodes(pugi::xml_node_iterator node, std::map<std::string, std::string> &vars, std::string startingKey)
{
	for (pugi::xml_node_iterator it = node->begin(); it != node->end(); ++it)
	{
		std::string currKey = startingKey;
		if (it->type() == pugi::xml_node_type::node_pcdata || it->type() == pugi::xml_node_type::node_cdata)
		{
			vars.try_emplace(currKey, it->value());
		}
		else
		{
			currKey += "." + std::string(it->name());
		}

		for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
		{
			vars.try_emplace(currKey + "." + ait->name(), ait->value());
		}

		ReadChildNodes(it, vars, currKey);
	}
}

char* CConfigurationFileParser::AutoConvertToType(std::string value, std::string type)
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