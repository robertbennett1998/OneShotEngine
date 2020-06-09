#pragma once
#ifndef ONE_SHOT_CORE_KEYBOARD_INPUT_EVENT_MANAGER_H
#define ONE_SHOT_CORE_KEYBOARD_INPUT_EVENT_MANAGER_H

#include "Interfaces/IEventManager.h"

class CKeyboardInputEventHandler;

class ONE_SHOT_CORE_DLL CKeyboardInputEventManager : public IEventManager<CKeyboardInputEventHandler>
{
	public:
		static CKeyboardInputEventManager* GetInstance()
		{
			if (s_pInstance == nullptr)
				s_pInstance = new CKeyboardInputEventManager();

			return s_pInstance;
		}
	
		virtual bool IsKeyPressed(UINT uiKeyCode);
		virtual void RaiseKeyEvent(UINT uiKey, bool bUp);

		// Inherited via IEventManager
		bool AddEventHandler(CKeyboardInputEventHandler* pEventHandler) override;
		bool RemoveEventHandler(CKeyboardInputEventHandler* pEventHandler) override;

	private:
		CKeyboardInputEventManager();
		~CKeyboardInputEventManager();
		
		std::vector<CKeyboardInputEventHandler*> m_pEventHandlers;
		std::map<UINT, bool> m_Keys; //Keys that are currently down
		static CKeyboardInputEventManager* s_pInstance;
};

#endif