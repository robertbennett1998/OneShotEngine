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