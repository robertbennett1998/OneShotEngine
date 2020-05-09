#pragma once
#ifndef ONE_SHOT_I_EVENT_MANAGER_H
#define ONE_SHOT_I_EVENT_MANAGER_H

#include "IEventHandler.h"

template <class T, class ...U>
class ONE_SHOT_CORE_DLL IEventManager
{
	public:
		virtual void RaiseEvent(U... params) = 0;
		virtual bool AddEventHandler(T* pEventHandler) = 0;
		virtual bool RemoveEventHandler(T* pEventHandler) = 0;
};

#endif