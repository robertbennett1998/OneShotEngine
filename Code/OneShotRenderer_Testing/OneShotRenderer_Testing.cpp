#include <iostream>
#include <Windows.h>

#ifdef _ONE_SHOT_CORE_DLL
#define ONE_SHOT_CORE_DLL __declspec(dllexport)
#else
#define ONE_SHOT_CORE_DLL __declspec(dllimport)
#endif

#ifdef _ONE_SHOT_RENDERER_DLL
#define ONE_SHOT_RENDERER_DLL __declspec(dllexport)
#else
#define ONE_SHOT_RENDERER_DLL __declspec(dllimport)
#endif

#include <OneShotCore/Include/MemoryManagement/MemoryManager.h>
#include <OneShotCore/Include/Logging/LoggingManager.h>
#include <OneShotCore/Include/Logging/Logger.h>
#include <OneShotCore/Include/FileUtillities/VirtualFileSystem.h>
#include <OneShotCore/Include/Utillities/GlobalVariables.h>
#include <OneShotCore/Include/FileUtillities/ConfigurationFileParser.h>
#include <OneShotCore/Include/Logging/Sinks/FileSink.h>

#include <OneShotRenderer/Include/OneShotRenderer3D.h>
#include <OneShotRenderer/Include/Direct3D.h>
#include <OneShotRenderer/Include/MaterialLibrary.h>

OneShotRenderer::COneShotRenderer3D* g_pRenderer = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	default:
		break;

	case WM_EXITSIZEMOVE:
		if (g_pRenderer)
			g_pRenderer->Resize();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

int main()
{
	CGlobalVariables globalEnviroment;
	CConfigurationFileParser configFile(&globalEnviroment);
	configFile.Load("/AppDataStore/Engine.config");
	std::string sRootVirtualPath = configFile.Get<std::string>("Configuration.VirtualPaths.Root");
	CVirtualFileSystem::GetInstance()->Mount("/Root", sRootVirtualPath);

	std::vector<std::pair<std::string, std::string>> virtualPaths = configFile.GetAll<std::string>("Configuration.VirtualPaths.*");

	for (int i = 0; i < virtualPaths.size(); i++)
	{
		size_t szLastDotPos = virtualPaths[i].first.find_last_of('.') + 1;
		if (szLastDotPos != std::string::npos)
		{
			OSE_LOG_INFO("General", "Retrived virtual path % - %", virtualPaths[i].first, virtualPaths[i].second);
			std::string sVirtualPathName = "/" + virtualPaths[i].first.erase(0, szLastDotPos);
			CVirtualFileSystem::GetInstance()->Mount(sVirtualPathName, virtualPaths[i].second);
		}
	}

	OSE_ADD_SINK("General", CFileSink::CreateFileSink("/Root/Logs/General.log"));

	HINSTANCE hCurrInstance = GetModuleHandleA(NULL);
	WNDCLASS wndCls; ZeroMemory(&wndCls, sizeof(WNDCLASS));
	wndCls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndCls.hInstance = hCurrInstance;
	wndCls.hIcon = LoadIcon(hCurrInstance, IDC_ARROW);
	wndCls.lpfnWndProc = WndProc;
	wndCls.lpszClassName = L"renderertest";
	wndCls.style = CS_HREDRAW | CS_VREDRAW;

	if (FAILED(RegisterClass(&wndCls)))
		return -1;

	HWND hWnd = CreateWindow(L"renderertest", L"Renderer Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, configFile.Get<int>("Configuration.Window.Width"), configFile.Get<int>("Configuration.Window.Height"), NULL, NULL, hCurrInstance, NULL);
	if (!hWnd)
		return -2;

	g_pRenderer = OSE_NEW(OneShotRenderer::COneShotRenderer3D(new OneShotRenderer::CDirect3D()));

	if (!g_pRenderer->Initialize(hWnd))
		return -3;

	if (!OneShotRenderer::CMaterialLibrary::GetInstance()->RegisterMaterials("/Geometry/LumberJack/lumberJack.mtl"))
		return -4;

	OSE_LOG_INFO("Memory", CMemoryManager::GetInstance()->GetRootHeap()->WriteHeapDetailsToString());

	MSG msg; ZeroMemory(&msg, sizeof(MSG));
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_pRenderer->Update();
		}

	} while (msg.message != WM_QUIT);

	OSE_SAFE_SHUTDOWN(g_pRenderer);

	OSE_LOG_INFO("Memory", CMemoryManager::GetInstance()->GetRootHeap()->WriteHeapDetailsToString());
}