#pragma once
#ifndef ONE_SHOT_CORE_CONSOLE_VARIABLES_H
#define ONE_SHOT_CORE_CONSOLE_VARIABLES_H

class ONE_SHOT_CORE_DLL CConsoleVariables
{
	public:
		CConsoleVariables();
		~CConsoleVariables();

		template<class T>
		bool RegisterVariable(std::string sVarName, T initialValue)
		{
			if (m_Variables.find(sVarName) != m_Variables.end())
				return false;

			auto pVal = std::make_shared<T>();
			*pVal = initialValue;

			m_Variables.try_emplace(sVarName, std::reinterpret_pointer_cast<char>(pVal));
			return true;
		}

		template<class T, class ...Args>
		bool RegisterVariable(std::string sVarName, Args... args)
		{
			if (m_Variables.find(sVarName) != m_Variables.end())
			{
				OSE_LOG_WARNING("General", "Couldn't register console variable % as a console variable is already registered with that name.", sVarName);
				return false;
			}

			auto pVal = std::make_shared<T>(args...);

			m_Variables.try_emplace(sVarName, std::reinterpret_pointer_cast<char>(pVal));
			OSE_LOG_INFO("General", "Registered a console variable with the name %", sVarName);
			return true;
		}

		bool DeregisterVariable(std::string sVarName)
		{
			auto iter = m_Variables.find(sVarName);
			if (iter == m_Variables.end())
			{
				OSE_LOG_WARNING("General", "Couldn't deregister console variable % as no console variable is registered with that name.", sVarName);
				return false;
			}

			m_Variables.erase(iter);
			OSE_LOG_INFO("General", "Dergistered the console variable with the name %", sVarName);
			return true;
		}

		template<class T>
		bool SetValue(std::string sVarName, T value)
		{
			auto p = GetVariable<T>(sVarName);
			if (p == nullptr)
			{
				OSE_LOG_WARNING("General", "Couldn't set console variables value as couldn't find console variable %.", sVarName);
				return false;
			}

			*p = value;
			return true;
		}

		template<class T>
		bool GetValue(std::string sVarName, T& value)
		{
			auto iter = m_Variables.find(sVarName);
			if (iter == m_Variables.end())
			{
				OSE_LOG_WARNING("General", "Couldn't get console variable % value, as couldn't find a console variable with that name.", sVarName);
				return false;
			}

			value = *std::reinterpret_pointer_cast<T>(iter->second);
			return true;
		}

		template<class T>
		std::shared_ptr<T> GetVariable(std::string sVarName)
		{
			auto iter = m_Variables.find(sVarName);
			if (iter == m_Variables.end())
			{
				OSE_LOG_WARNING("General", "Couldn't get console variable %, as couldn't find a console variable with that name.", sVarName);
				return std::shared_ptr<T>(nullptr);
			}

			return std::reinterpret_pointer_cast<T>(iter->second);
		}

	private:
		std::map<std::string, std::shared_ptr<char>> m_Variables;
};

#endif