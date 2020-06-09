#include "CoreHeaders.h"
#include "Events/MouseInputEventManager.h"
#include "Events/MouseInputEventHandler.h"

CMouseInputEventManager* CMouseInputEventManager::s_pInstance = nullptr;

CMouseInputEventManager::CMouseInputEventManager() :
m_uiY(0),
m_uiX(0),
m_uiPrevX(0),
m_uiPrevY(0),
m_bLeftMouseButtonDown(false),
m_bMiddleMouseButtonDown(false),
m_bRightMouseButtonDown(false)
{
}

CMouseInputEventManager::~CMouseInputEventManager()
{
}

void CMouseInputEventManager::RaiseMouseMoveEvent(UINT uiNewX, UINT uiNewY)
{
	for (auto pEventHandler : m_pEventHandlers)
		pEventHandler->OnMouseMoveEvent(uiNewX, uiNewY, m_uiPrevX, m_uiPrevY);

	m_uiPrevX = m_uiX;
	m_uiPrevY = m_uiY;
	m_uiX = uiNewX;
	m_uiY = uiNewY;
}

void CMouseInputEventManager::RaiseLeftMouseButtonEvent(UINT uiX, UINT uiY, bool bDown)
{
	m_bLeftMouseButtonDown = bDown;
	
	for (auto pEventHandler : m_pEventHandlers)
		pEventHandler->OnLeftMouseButtonEvent(uiX, uiY, bDown);
}

void CMouseInputEventManager::RaiseMiddleMouseButtonEvent(UINT uiX, UINT uiY, bool bDown)
{
	m_bMiddleMouseButtonDown = bDown;
	
	for (auto pEventHandler : m_pEventHandlers)
		pEventHandler->OnMiddleMouseButtonEvent(uiX, uiY, bDown);
}

void CMouseInputEventManager::RaiseRightMouseButtonEvent(UINT uiX, UINT uiY, bool bDown)
{
	m_bRightMouseButtonDown = bDown;
	
	for (auto pEventHandler : m_pEventHandlers)
		pEventHandler->OnRightMouseButtonEvent(uiX, uiY, bDown);
}

bool CMouseInputEventManager::AddEventHandler(CMouseInputEventHandler* pEventHandler)
{
	if (pEventHandler == nullptr)
		return false;

	for (auto pExistingEventHandler : m_pEventHandlers)
		if (pExistingEventHandler == pEventHandler)
			return false;

	m_pEventHandlers.push_back(pEventHandler);
	return true;
}

bool CMouseInputEventManager::RemoveEventHandler(CMouseInputEventHandler* pEventHandler)
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