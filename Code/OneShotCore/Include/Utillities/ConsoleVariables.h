#pragma once
#ifndef ONE_SHOT_CORE_CONSOLE_VARIABLES_H
#define ONE_SHOT_CORE_CONSOLE_VARIABLES_H

class ONE_SHOT_CORE_DLL CConsoleVariables
{
	private:
		struct CVar
		{
			std::shared_ptr<char> pValue;
			std::vector<std::function<void()>> valueChangedCallbacks;
		};
	
	public:
		CConsoleVariables();
		~CConsoleVariables();

		template<class T>
		bool RegisterVariable(std::string sVarName, T initialValue, std::function<void()> valueChangedCallback = nullptr)
		{
			if (m_Variables.find(sVarName) != m_Variables.end())
				return false;

			auto pVal = std::make_shared<T>();
			*pVal = initialValue;

			CVar var;
			var.pValue = std::reinterpret_pointer_cast<char>(pVal);
			if (valueChangedCallback != nullptr)
				var.valueChangedCallbacks.push_back(valueChangedCallback);

			m_Variables.try_emplace(sVarName, var);
			return true;
		}

		bool AddValueChangedCallback(std::string sVarName, std::function<void()> callback)
		{
			auto iter = m_Variables.find(sVarName);
			if (iter == m_Variables.end())
			{
				OSE_LOG_WARNING("General", "Couldn't get console variable % value, as couldn't find a console variable with that name.", sVarName);
				return false;
			}

			iter->second.valueChangedCallbacks.push_back(callback);

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
			auto iter = m_Variables.find(sVarName);
			if (iter == m_Variables.end())
			{
				OSE_LOG_WARNING("General", "Couldn't set console variable % value, as couldn't find a console variable with that name.", sVarName);
				return false;
			}

			auto p = iter->second.pValue;
			if (p == nullptr)
			{
				OSE_LOG_WARNING("General", "Couldn't set console variables value as couldn't find console variable %.", sVarName);
				return false;
			}

			*p = value;

			for (auto callback : iter->second.valueChangedCallbacks)
				callback();

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

			value = *std::reinterpret_pointer_cast<T>(iter->second.pValue);
			return true;
		}

		//template<class T>
		//std::shared_ptr<T> GetVariable(std::string sVarName)
		//{
		//	auto iter = m_Variables.find(sVarName);
		//	if (iter == m_Variables.end())
		//	{
		//		OSE_LOG_WARNING("General", "Couldn't get console variable %, as couldn't find a console variable with that name.", sVarName);
		//		return std::shared_ptr<T>(nullptr);
		//	}

	private:
		std::map<std::string, CVar> m_Variables;
};

#endif