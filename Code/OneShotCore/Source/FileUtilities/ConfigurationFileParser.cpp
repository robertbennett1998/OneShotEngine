#include "CoreHeaders.h"
#include "FileUtillities/ConfigurationFileParser.h"
#include <any>

CConfigurationFileParser::CConfigurationFileParser() :
m_pGlobalVariables(nullptr),
m_bSaveOnValueChange(false)
{
}

CConfigurationFileParser::CConfigurationFileParser(bool bSaveOnFileChange) :
m_bSaveOnValueChange(bSaveOnFileChange)
{
}

CConfigurationFileParser::CConfigurationFileParser(CGlobalVariables* pGlobalVariables, bool bSaveOnFileChange) :
m_pGlobalVariables(pGlobalVariables),
m_bSaveOnValueChange(bSaveOnFileChange)
{
}

CConfigurationFileParser::~CConfigurationFileParser()
{
	if (m_pGlobalVariables != nullptr)
	{
		for (auto configItemIter : m_ConfigItems)
		{
			if (configItemIter.second.bIsCVar)
				m_pGlobalVariables->DeregisterVariable(configItemIter.first);
		}
	}
}

bool CConfigurationFileParser::Load(std::string sFilePath)
{
	m_sPhysicalFilePath = CVirtualFileSystem::GetInstance()->ResolvePhysicalPath(sFilePath);

	pugi::xml_parse_result res = m_ConfigDoc.load_file(m_sPhysicalFilePath.c_str(), pugi::parse_trim_pcdata);

	if (res)
	{
		OSE_DEBUG_LOG_INFO("General", "Loaded configuration file %!", m_sPhysicalFilePath);
	}
	else
	{
		OSE_LOG_WARNING("General", "Failed to load xml file %! With error %! At position %", m_sPhysicalFilePath, res.description(), res.offset);
		return false;
	}

	std::map<std::string, std::string> keyValPairs;
	ReadDocumentNodes(m_ConfigDoc, keyValPairs);

	if (keyValPairs.empty())
	{
		OSE_LOG_WARNING("General", "Could not load configuration file % as no key value pairs were found!", m_sPhysicalFilePath);
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
		unsigned long long szLastDotPos = sKey.find_last_of('.') + 1;
		if(szLastDotPos == std::string::npos)
		{
			keys.push_back(sKey);
		}

		std::string sLastPart = sKey.substr(szLastDotPos, sKey.length() - szLastDotPos);

		if (sLastPart == "cvar" || sLastPart == "type")
			continue;

		keys.push_back(sKey);
	}

	for (auto key : keys)
	{
		ConfigItem configItem;
		configItem.bIsCVar = keyValPairs[key + ".cvar"] == "true" ? true : false;
		configItem.sType = keyValPairs[key + ".type"];
		configItem.pValue = AutoConvertToType(keyValPairs[key], configItem.sType);

		if(configItem.pValue == nullptr)
		{
			OSE_LOG_WARNING("General", "Couldn't create config item for key %", key);
			continue;
		}

		if (configItem.bIsCVar)
		{
			std::function<void(std::string)> callback = std::bind(&CConfigurationFileParser::GlobalVariableLinkValueChangedCallback, this, std::placeholders::_1);

			if (m_pGlobalVariables != nullptr)
				m_pGlobalVariables->RegisterVariable(key, configItem.pValue, callback);
		}

		m_ConfigItems.try_emplace(key, configItem);
	}

	#ifdef _DEBUG
	OSE_DEBUG_LOG_INFO("General", "Config created with the following items: ");
	for (auto configItem : m_ConfigItems)
	{
		OSE_DEBUG_LOG_INFO("General", "\t% - IsCVar: % - Type: % - pValue: %", configItem.first, configItem.second.bIsCVar, configItem.second.sType, &configItem.second.pValue);
	}
	#endif

	return true;
}

void CConfigurationFileParser::Save()
{
	for (auto configItemIter : m_ConfigItems)
	{
		pugi::xml_node xmlNode = GetDocumentNodeFromKey(configItemIter.first).first_child();
		xmlNode.set_value(AutoConvertFromType(configItemIter.second.pValue, configItemIter.second.sType).c_str());
	}

	if (!m_ConfigDoc.save_file(m_sPhysicalFilePath.c_str(), "\t"))
		OSE_DEBUG_LOG_WARNING("General", "Couldn't save config file with the file path %", m_sPhysicalFilePath);

	OSE_DEBUG_LOG_INFO("General", "Successfully saved settings file!");
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

pugi::xml_node CConfigurationFileParser::GetDocumentNodeFromKey(std::string sKey)
{
	unsigned long long szDotPos = 0;
	pugi::xml_node currNode = m_ConfigDoc.root();
	while (szDotPos != std::string::npos)
	{
		szDotPos = sKey.find_first_of('.');

		std::string sNodeName = sKey.substr(0, szDotPos);

		currNode = currNode.child(sNodeName.c_str());

		sKey = sKey.erase(0, (szDotPos + 1));
	}

	return currNode;
}

void CConfigurationFileParser::GlobalVariableLinkValueChangedCallback(std::string sKey)
{
	if(m_bSaveOnValueChange)
		Save();
}

std::shared_ptr<char> CConfigurationFileParser::AutoConvertToType(std::string value, std::string type)
{
	std::shared_ptr<char> pValue = nullptr;

	if (type == "bool")
	{
		std::shared_ptr<bool> p = std::make_shared<bool>();
		*p = (char*)(value == "true" ? true : false);
		pValue = std::reinterpret_pointer_cast<char>(p);
	}
	else if (type == "int")
	{
		std::shared_ptr<int> p = std::make_shared<int>();
		*p = std::stoi(value);
		pValue = std::reinterpret_pointer_cast<char>(p);
	}
	else if (type == "long")
	{
		std::shared_ptr<long> p = std::make_shared<long>();
		*p = std::stol(value);
		pValue = std::reinterpret_pointer_cast<char>(p);;
	}
	else if (type == "longlong")
	{
		std::shared_ptr<long long> p = std::make_shared<long long>();
		*p = std::stoll(value);
		pValue = std::reinterpret_pointer_cast<char>(p);
	}
	else if (type == "ulong")
	{
		std::shared_ptr<unsigned long> p = std::make_shared<unsigned long>();
		*p = std::stoul(value);
		pValue = std::reinterpret_pointer_cast<char>(p);
	}
	else if (type == "ulonglong")
	{
		std::shared_ptr<unsigned long long> p = std::make_shared<unsigned long long>();
		*p = std::stoull(value);
		pValue = std::reinterpret_pointer_cast<char>(p);
	}
	else if (type == "float")
	{
		std::shared_ptr<float> p = std::make_shared<float>();
		*p = std::stof(value);
		pValue = std::reinterpret_pointer_cast<char>(p);
	}
	else if (type == "double")
	{
		std::shared_ptr<double> p = std::make_shared<double>();
		*p = std::stod(value);
		pValue = std::reinterpret_pointer_cast<char>(p);
	}
	else if (type == "longdouble")
	{
		std::shared_ptr<double> p = std::make_shared<double>();
		*p = std::stold(value);
		pValue = std::reinterpret_pointer_cast<char>(p);
	}
	else if (type == "string")
	{
		std::shared_ptr<std::string> p = std::make_shared<std::string>();
		*p = value;
		pValue = std::reinterpret_pointer_cast<char>(p);
	}

	return pValue;
}

std::basic_string<char> CConfigurationFileParser::AutoConvertFromType(std::shared_ptr<char> value, std::basic_string<char> type)
{
	std::string sValue = "Type Not Found";

	if (type == "bool")
	{
		std::shared_ptr<bool> p = std::reinterpret_pointer_cast<bool>(value);
		sValue = *p == true ? "true" : "false";
	}
	else if (type == "int")
	{
		std::shared_ptr<int> p = std::reinterpret_pointer_cast<int>(value);
		std::stringstream ss;
		ss << *p;
		sValue = ss.str();
	}
	else if (type == "long")
	{
		std::shared_ptr<long> p = std::reinterpret_pointer_cast<long>(value);
		std::stringstream ss;
		ss << *p;
		sValue = ss.str();
	}
	else if (type == "longlong")
	{
		std::shared_ptr<long long> p = std::reinterpret_pointer_cast<long long>(value);
		std::stringstream ss;
		ss << *p;
		sValue = ss.str();
	}
	else if (type == "ulong")
	{
		std::shared_ptr<unsigned long> p = std::reinterpret_pointer_cast<unsigned long>(value);
		std::stringstream ss;
		ss << *p;
		sValue = ss.str();
	}
	else if (type == "ulonglong")
	{
		std::shared_ptr<unsigned long long> p = std::reinterpret_pointer_cast<unsigned long long>(value);
		std::stringstream ss;
		ss << *p;
		sValue = ss.str();
	}
	else if (type == "float")
	{
		std::shared_ptr<float> p = std::reinterpret_pointer_cast<float>(value);
		std::stringstream ss;
		ss << *p;
		sValue = ss.str();
	}
	else if (type == "double")
	{
		std::shared_ptr<double> p = std::reinterpret_pointer_cast<double>(value);
		std::stringstream ss;
		ss << *p;
		sValue = ss.str();
	}
	else if (type == "longdouble")
	{
		std::shared_ptr<long double> p = std::reinterpret_pointer_cast<long double>(value);
		std::stringstream ss;
		ss << *p;
		sValue = ss.str();
	}
	else if (type == "string")
	{
		std::shared_ptr<std::string> p = std::reinterpret_pointer_cast<std::string>(value);
		sValue = *p;
	}

	return sValue;
}