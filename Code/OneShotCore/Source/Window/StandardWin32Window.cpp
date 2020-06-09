#include "CoreHeaders.h"
#include "Window/StandardWin32Window.h"

CStandardWin32Window::CStandardWin32Window() :
m_uiX(0),
m_uiY(0),
m_uiWidth(0),
m_uiHeight(0),
m_hWnd(nullptr)
{
}

CStandardWin32Window::~CStandardWin32Window()
{
	Destroy();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (uiMsg == WM_DESTROY)
			PostQuitMessage(0);
	
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

bool CStandardWin32Window::Create(UINT uiWidth, UINT uiHeight, UINT uiX, UINT uiY, bool bMaximised, bool bFullscreen)
{
	if (m_hWnd == nullptr)
	{
		m_uiWidth = uiWidth;
		m_uiHeight = uiHeight;
		m_uiX = uiX;
		m_uiY = uiY;
		
		HINSTANCE hCurrInstance = GetModuleHandleA(NULL);
		WNDCLASS wndCls; ZeroMemory(&wndCls, sizeof(WNDCLASS));
		wndCls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndCls.hInstance = hCurrInstance;
		wndCls.hIcon = LoadIcon(hCurrInstance, IDC_ARROW);
		wndCls.lpfnWndProc = WndProc;
		wndCls.lpszClassName = "renderertest";
		wndCls.style = CS_HREDRAW | CS_VREDRAW;

		if (FAILED(RegisterClass(&wndCls)))
		{
			OSE_LOG_ERROR("General", "Failed to create a Standard Win32 Window! Couldn't register the WNDCLASS %.", wndCls.lpszClassName);
			return false;
		}

		m_hWnd = CreateWindow("renderertest", "Renderer Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE, m_uiX, m_uiY, m_uiWidth, m_uiHeight, NULL, NULL, hCurrInstance, NULL);
		if (!m_hWnd)
		{
			OSE_LOG_ERROR("General", "Failed to create a Standard Win32 Window!");
			return false;
		}

		return true;
	}
	
	return false;
}

void CStandardWin32Window::Destroy()
{
	if (m_hWnd != nullptr)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}
