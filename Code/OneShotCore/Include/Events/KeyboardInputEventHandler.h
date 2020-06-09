#pragma once
#ifndef ONE_SHOT_CORE_KEYBOARD_INPUT_EVENT_HANDLER_H
#define ONE_SHOT_CORE_KEYBOARD_INPUT_EVENT_HANDLER_H

#include "Interfaces/IEventHandler.h"

class ONE_SHOT_CORE_DLL CKeyboardInputEventHandler : public IEventHandler<CKeyboardInputEventHandler>
{
	public:
		virtual void OnKeyEvent(UINT uiKey, bool bUp) = 0;
};

#endif