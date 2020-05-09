#pragma once
#ifndef ONE_SHOT_CORE_KEYBOARD_INPUT_EVENT_MANAGER_H
#define ONE_SHOT_CORE_KEYBOARD_INPUT_EVENT_MANAGER_H

#include "Interfaces/IEventManager.h"
#include "KeyboardInputEventHandler.h"

class ONE_SHOT_CORE_DLL CKeyboardInputEventManager : public IEventManager<CKeyboardInputEventHandler, UINT, bool>
{
	public:
		static CKeyboardInputEventManager* GetInstance()
		{
			if (s_pInstance == nullptr)
				s_pInstance = new CKeyboardInputEventManager();

			return s_pInstance;
		}

		virtual bool IsKeyPresed(UINT uiKeyCode);

		// Inherited via IEventManager
		virtual void RaiseEvent(UINT uiKey, bool bUp) override;
		virtual bool AddEventHandler(CKeyboardInputEventHandler* pEventHandler) override;
		virtual bool RemoveEventHandler(CKeyboardInputEventHandler* pEventHandler) override;

	private:
		CKeyboardInputEventManager();
		~CKeyboardInputEventManager();
		
		std::vector<CKeyboardInputEventHandler*> m_pEventHandlers;
		std::map<UINT, bool> m_Keys; //Keys that are currently down
		static CKeyboardInputEventManager* s_pInstance;
};

#endif