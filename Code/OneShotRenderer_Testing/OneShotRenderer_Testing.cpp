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

//#define LOG_FPS

#include <Windowsx.h>
#include <OneShotCore/Include/MemoryManagement/MemoryManager.h>
#include <OneShotCore/Include/Logging/LoggingManager.h>
#include <OneShotCore/Include/Logging/Logger.h>
#include <OneShotCore/Include/FileUtillities/VirtualFileSystem.h>
#include <OneShotCore/Include/Utillities/GlobalVariables.h>
#include <OneShotCore/Include/FileUtillities/ConfigurationFileParser.h>
#include <OneShotCore/Include/Logging/Sinks/FileSink.h>
#include <OneShotCore/Include/Time/Stopwatch.h>
#include <OneShotCore/Include/Logging/Sinks/CoutSink.h>
#include <OneShotCore/Include/Events/KeyboardInputEventManager.h>
#include <OneShotCore/Include/Events/KeyboardInputEventHandler.h>

#include <OneShotRenderer/Include/OneShotRenderer3D.h>
#include <OneShotRenderer/Include/Direct3D.h>
#include <OneShotRenderer/Include/MaterialLibrary.h>
#include <OneShotRenderer/Include/Renderables/Interfaces/IRenderable.h>
#include <OneShotRenderer/Include/GeometryManager.h>
#include <OneShotRenderer/Include/Shaders/BasicTextureShaderProgram.h>
#include <OneShotRenderer/Include/Shaders/ShaderManager.h>

#include "OneShotCore/Include/Events/MouseInputEventManager.h"

using namespace OneShotRenderer;
OneShotRenderer::COneShotRenderer3D* g_pRenderer = nullptr;

class CLumberJack : public IRenderable, public CKeyboardInputEventHandler
{
public:
	bool Initialize() override;
	void Bind() override;
	void Shutdown() override;
	void Hide() override;
	void Show() override;
	void ToggleVisible() override;
	const CGeometry* GetGeometry() const override;
	const bool GetShouldRender() const override;
	const IShaderProgram* GetShaderProgram() const override;
	void SetWorldMatrix(DirectX::XMMATRIX xmmWorld);
private:
	const CGeometry* m_pGeometry;
	bool m_bShouldRender;
	IShaderProgram* m_pShaderProgram;
	DirectX::XMMATRIX m_xmmWorldMatrix;

	// Inherited via CKeyboardInputEventHandler
	virtual void OnKeyEvent(UINT uiKey, bool bUp) override
	{
		OSE_LOG_INFO("General", "Key pressed %% - %%", uiKey, bUp);
	}
};

bool CLumberJack::Initialize()
{
	CGeometryManager* pGeomManager = CGeometryManager::GetInstance();
	if (pGeomManager == nullptr)
		return false;

	m_pGeometry = pGeomManager->GetGeomInstance("lumberJack");
	if (m_pGeometry == nullptr)
		return false;

	m_pShaderProgram = CShaderManager::GetInstance()->GetShaderInstance<CBasicTextureShaderProgram>("BasicTexture");
	if (m_pShaderProgram == nullptr)
		return false;

	m_bShouldRender = true;
	return true;
}

void CLumberJack::Bind()
{
	((CBasicTextureShaderProgram*)m_pShaderProgram)->SetWorldMatrix(m_xmmWorldMatrix);
	m_pShaderProgram->Bind();
}

void CLumberJack::Shutdown()
{
}

void CLumberJack::Hide()
{
	m_bShouldRender = false;
}

void CLumberJack::Show()
{
	m_bShouldRender = true;
}

void CLumberJack::ToggleVisible()
{
	m_bShouldRender = !m_bShouldRender;
}

const CGeometry* CLumberJack::GetGeometry() const
{
	return m_pGeometry;
}

const bool CLumberJack::GetShouldRender() const
{
	return m_bShouldRender;
}

const IShaderProgram* CLumberJack::GetShaderProgram() const
{
	return m_pShaderProgram;
}

void CLumberJack::SetWorldMatrix(DirectX::XMMATRIX xmmWorld)
{
	m_xmmWorldMatrix = xmmWorld;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
		default:
			break;

		case WM_MOUSEMOVE:
		{
			CMouseInputEventManager::GetInstance()->RaiseMouseMoveEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		}

		case WM_LBUTTONDOWN:
		{
			CMouseInputEventManager::GetInstance()->RaiseLeftMouseButtonEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
			break;
		}

		case WM_LBUTTONUP:
		{
			CMouseInputEventManager::GetInstance()->RaiseLeftMouseButtonEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
			break;
		}

		case WM_MBUTTONDOWN:
		{
			CMouseInputEventManager::GetInstance()->RaiseMiddleMouseButtonEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
			break;
		}

		case WM_MBUTTONUP:
		{
			CMouseInputEventManager::GetInstance()->RaiseMiddleMouseButtonEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
			break;
		}

		case WM_RBUTTONDOWN:
		{
			CMouseInputEventManager::GetInstance()->RaiseRightMouseButtonEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
			break;
		}

		case WM_RBUTTONUP:
		{
			CMouseInputEventManager::GetInstance()->RaiseRightMouseButtonEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
			break;
		}
		
		case WM_KEYUP:
		{
			CKeyboardInputEventManager::GetInstance()->RaiseKeyEvent(wParam, true);
			break;
		}

		case WM_KEYDOWN:
		{
			CKeyboardInputEventManager::GetInstance()->RaiseKeyEvent(wParam, false);
			break;
		}

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
	//OSE_ADD_SINK("General", CCoutSink::CreateCoutSink());

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

	HWND hWnd = CreateWindow(L"renderertest", L"Renderer Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE, configFile.Get<int>("Configuration.Window.PosX"), configFile.Get<int>("Configuration.Window.PosY"), configFile.Get<int>("Configuration.Window.Width"), configFile.Get<int>("Configuration.Window.Height"), NULL, NULL, hCurrInstance, NULL);
	if (!hWnd)
		return -2;

	auto pDirect3D = OSE_NEW(OneShotRenderer::CDirect3D());
	g_pRenderer = OSE_NEW(OneShotRenderer::COneShotRenderer3D(pDirect3D));

	if (!g_pRenderer->Initialize(hWnd))
		return -3;

	if (!CShaderManager::GetInstance()->RegisterShaderType<CBasicTextureShaderProgram>("BasicTexture"))
		return -4;

	if (!OneShotRenderer::CMaterialLibrary::GetInstance()->RegisterMaterials("/Geometry/LumberJack/lumberJack.mtl"))
		return -5;

	if (!CGeometryManager::GetInstance()->RegisterGeometry("/Geometry/LumberJack/lumberJack.obj"))
		return -6;

	CLumberJack lumberJack;
	if (!lumberJack.Initialize())
		return -7;

	lumberJack.SetWorldMatrix(DirectX::XMMatrixTranslation(0.0f, 0.0f, 10.0f));

	if (!g_pRenderer->Get3DRenderer()->RegisterRenderable(&lumberJack))
		return -8;

	//TODO: Extend the geometry framework to allow different types...
	if (!CGeometryManager::GetInstance()->RegisterGeometry("/Geometry/Biped/Biped.obj"))
		return -9;

	//CBiped biped;
	//if (!biped.Initialize())
	//	return -10;

	//biped.SetWorldMatrix(DirectX::XMMatrixTranslation(3.0f, 0.0f, 0.0f));

	//if (!g_pRenderer->Get3DRenderer()->RegisterRenderable(&biped))
	//	return -11;

	//CBiped biped2;
	//if (!biped2.Initialize())
	//	return -12;

	//biped2.SetWorldMatrix(DirectX::XMMatrixTranslation(-3.0f, 0.0f, 0.0f));

	//if (!g_pRenderer->Get3DRenderer()->RegisterRenderable(&biped2))
	//	return -13;

	OSE_LOG_INFO("Memory", CMemoryManager::GetInstance()->GetRootHeap()->WriteHeapDetailsToString());
	//OSE_LOG_INFO("Memory", CMemoryManager::GetInstance()->GetHeap("lifetime")->WriteHeapDetailsToString());

	MSG msg; ZeroMemory(&msg, sizeof(MSG));
	CStopwatch stopwatch;
	stopwatch.Start();
	double dPrevTime = stopwatch.GetTimeElapsed();
	double dPrevSecond = dPrevTime;
	long long fps = 0;
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			double dCurrTime = stopwatch.GetTimeElapsed();
			double dt = dCurrTime - dPrevTime;
			if (dt < 1.0f / 60.0f)
				continue;

			g_pRenderer->Update(dt);

			dPrevTime = dCurrTime;
			if (dCurrTime - dPrevSecond > 1)
			{
				std::stringstream ss;
				ss << "FPS: " << fps << " Frame Time (s): " << dt;
				#ifdef LOG_FPS
					OSE_DEBUG_LOG_INFO("General", ss.str());
				#endif
				SetWindowTextA(hWnd, ss.str().c_str());
				fps = 0;
				dPrevSecond = dCurrTime;
			}

			fps++;
		}

	} while (msg.message != WM_QUIT);

	OSE_SAFE_SHUTDOWN(g_pRenderer);
	
	OSE_LOG_INFO("Memory", CMemoryManager::GetInstance()->GetRootHeap()->WriteHeapDetailsToString());
	//OSE_LOG_INFO("Memory", CMemoryManager::GetInstance()->GetHeap("lifetime")->WriteHeapDetailsToString());
}