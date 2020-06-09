#pragma once
#ifndef ONE_SHOT_CORE_MOUSE_INPUT_EVENT_MANAGER_H
#define ONE_SHOT_CORE_MOUSE_INPUT_EVENT_MANAGER_H

#include "Interfaces/IEventManager.h"

class CMouseInputEventHandler;

class ONE_SHOT_CORE_DLL CMouseInputEventManager : public IEventManager<CMouseInputEventHandler>
{
	public:
		static CMouseInputEventManager* GetInstance()
		{
			if (s_pInstance == nullptr)
				s_pInstance = new CMouseInputEventManager();

			return s_pInstance;
		}

		void RaiseMouseMoveEvent(UINT uiNewX, UINT uiNewY);
		void RaiseLeftMouseButtonEvent(UINT uiX, UINT uiY, bool bDown);
		void RaiseMiddleMouseButtonEvent(UINT uiX, UINT uiY, bool bDown);
		void RaiseRightMouseButtonEvent(UINT uiX, UINT uiY, bool bDown);
	
		// Inherited via IEventManager
		bool AddEventHandler(CMouseInputEventHandler* pEventHandler) override;
		bool RemoveEventHandler(CMouseInputEventHandler* pEventHandler) override;

		bool GetLeftMouseButtonDown() const { return m_bLeftMouseButtonDown;  }
		bool GetMiddleMouseButtonDown() const { return m_bMiddleMouseButtonDown; }
		bool GetRightMouseButtonDown() const { return m_bRightMouseButtonDown; }
	
	private:
		CMouseInputEventManager();
		~CMouseInputEventManager();

		std::vector<CMouseInputEventHandler*> m_pEventHandlers;
		bool m_bLeftMouseButtonDown, m_bMiddleMouseButtonDown, m_bRightMouseButtonDown;
		UINT m_uiY, m_uiX, m_uiPrevX, m_uiPrevY;
		static CMouseInputEventManager* s_pInstance;
};

#endif