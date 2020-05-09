#pragma once
#ifndef ONE_SHOT_CORE_KEYBOARD_INPUT_EVENT_HANDLER_H
#define ONE_SHOT_CORE_KEYBOARD_INPUT_EVENT_HANDLER_H

#include "Interfaces/IEventHandler.h"
#include "KeyboardInputEventHandler.h"

class ONE_SHOT_CORE_DLL CKeyboardInputEventHandler : public IEventHandler<CKeyboardInputEventHandler, UINT, bool>
{
	public:
		virtual void OnKeyboardEvent(UINT uiKey, bool bUp) = 0;
};

#endif