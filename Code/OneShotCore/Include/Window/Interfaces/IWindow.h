#pragma once
#ifndef ONE_SHOT_CORE_IWINDOW_H
#define ONE_SHOT_CORE_IWINDOW_H

class ONE_SHOT_CORE_DLL IWindow
{
	public:
		virtual ~IWindow() = default;
	private:
		virtual bool Create(UINT uiWidth, UINT uiHeight, UINT uiX, UINT uiY, bool bMaximised, bool bFullscreen=false) = 0;
		virtual void Destroy() = 0;
};

#endif