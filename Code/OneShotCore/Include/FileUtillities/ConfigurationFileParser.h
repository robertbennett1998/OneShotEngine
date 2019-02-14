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
			std::shared_ptr<char> pValue;
		};

	public:
		CConfigurationFileParser();
		CConfigurationFileParser(bool bSaveOnFileChange);
		CConfigurationFileParser(CGlobalVariables* pGlobalVariables, bool bSaveOnFileChange = false);
		~CConfigurationFileParser();

		bool Load(std::string sFilePath);

		template<class T>
		void Set(std::string key, T value)
		{
			if(m_ConfigItems.find(key) == m_ConfigItems.end())
			{
				OSE_LOG_WARNING("General", "Couldn't find config item with the key %", key);
				return;
			}

			*std::reinterpret_pointer_cast<T>(m_ConfigItems[key].pValue) = value;

			if(m_bSaveOnValueChange)
				Save();

			return;
		}

		void Save();

		template<class T>
		T Get(std::string key)
		{
			return *std::reinterpret_pointer_cast<T>(m_ConfigItems[key].pValue);
		}

		bool IsSaveOnChangeEnabled() const
		{
			return m_bSaveOnValueChange;
		}

		void SetSaveOnChangedEnabled(const bool bEnabled)
		{
			m_bSaveOnValueChange = bEnabled;
		}

	private:
		CGlobalVariables* m_pGlobalVariables;

		std::map<std::string, ConfigItem> m_ConfigItems;
		bool m_bSaveOnValueChange;

		std::string m_sPhysicalFilePath;
		pugi::xml_document m_ConfigDoc;

		inline void ReadDocumentNodes(pugi::xml_document& doc, std::map<std::string, std::string> &vars);
		inline void ReadChildNodes(pugi::xml_node_iterator node, std::map<std::string, std::string> &vars, std::string startingKey);
		inline pugi::xml_node GetDocumentNodeFromKey(std::string sKey);
		void GlobalVariableLinkValueChangedCallback(std::string sKey);

		std::shared_ptr<char> AutoConvertToType(std::string value, std::string type);
		std::string AutoConvertFromType(std::shared_ptr<char> value, std::string type);
};

#endif