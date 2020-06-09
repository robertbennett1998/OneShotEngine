#pragma once
#ifndef ONE_SHOT_CORE_MOUSE_INPUT_EVENT_HANDLER_H
#define ONE_SHOT_CORE_MOUSE_INPUT_EVENT_HANDLER_H

#include "Interfaces/IEventHandler.h"

class ONE_SHOT_CORE_DLL CMouseInputEventHandler : public IEventHandler<CMouseInputEventHandler>
{
	public:
		virtual void OnMouseMoveEvent(UINT uiNewX, UINT uiNewY, UINT uiPrevX, UINT uiPrevY) = 0;
		virtual void OnLeftMouseButtonEvent(UINT uiX, UINT uiY, bool bDown) = 0;
		virtual void OnMiddleMouseButtonEvent(UINT uiX, UINT uiY, bool bDown) = 0;
		virtual void OnRightMouseButtonEvent(UINT uiX, UINT uiY, bool bDown) = 0;
};

#endif