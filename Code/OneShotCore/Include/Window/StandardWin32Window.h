#pragma once
#ifndef ONE_SHOT_CORE_STANDARD_WIN32_WINDOW_H
#define ONE_SHOT_CORE_STANDARD_WIN32_WINDOW_H
#include "Interfaces/IWindow.h"

class ONE_SHOT_CORE_DLL CStandardWin32Window : public IWindow
{
	public:
		CStandardWin32Window();
		~CStandardWin32Window();
	
		bool Create(UINT uiWidth, UINT uiHeight, UINT uiX, UINT uiY, bool bMaximised, bool bFullscreen) override;
		void Destroy() override;

		HWND GetHWnd() const { return m_hWnd; }
	
	private:
		HWND m_hWnd;
		UINT m_uiX, m_uiY, m_uiWidth, m_uiHeight;
};

#endif
