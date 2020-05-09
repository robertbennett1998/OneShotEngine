#include "CoreHeaders.h"
#include "Events/KeyboardInputEventManager.h"

CKeyboardInputEventManager* CKeyboardInputEventManager::s_pInstance = nullptr;

CKeyboardInputEventManager::CKeyboardInputEventManager()
{
}

CKeyboardInputEventManager::~CKeyboardInputEventManager()
{
}

bool CKeyboardInputEventManager::IsKeyPresed(UINT uiKeyCode)
{
	if (m_Keys.contains(uiKeyCode))
		return m_Keys[uiKeyCode] == false;

	return false;
}

void CKeyboardInputEventManager::RaiseEvent(UINT uiKey, bool bUp)
{
	m_Keys[uiKey] = bUp;

	for (auto pEventHandler : m_pEventHandlers)
		pEventHandler->OnKeyboardEvent(uiKey, bUp);
}

bool CKeyboardInputEventManager::AddEventHandler(CKeyboardInputEventHandler* pEventHandler)
{
	if (pEventHandler == nullptr)
		return false;

	for (auto pExistingEventHandler : m_pEventHandlers)
		if (pExistingEventHandler == pEventHandler)
			return false;

	m_pEventHandlers.push_back(pEventHandler);
	return true;
}

bool CKeyboardInputEventManager::RemoveEventHandler(CKeyboardInputEventHandler* pEventHandler)
{
	if (pEventHandler == nullptr)
		return false;

	for (UINT i = 0; i < m_pEventHandlers.size(); i++)
	{
		if (m_pEventHandlers[i] == pEventHandler)
		{
			m_pEventHandlers.erase(m_pEventHandlers.begin() + i);
			return true;
		}
	}

	m_pEventHandlers.push_back(pEventHandler);
	return false;
}