#include "pch.h"
#include "OneShotRenderer3D.h"
#include "Cameras/BasicCamera.h"

using namespace OneShotRenderer;

CBasicCamera* g_pBasicCamera = nullptr;

COneShotRenderer3D::COneShotRenderer3D(IRenderer3D* pRenderer3D) :
	m_bInitialized(false),
	m_pRenderer3D(pRenderer3D)
{
}

COneShotRenderer3D::~COneShotRenderer3D()
{
}

bool COneShotRenderer3D::Initialize(HWND hWnd)
{
	if (!m_bInitialized)
	{
		if (!m_pRenderer3D)
			return false;

		g_pBasicCamera = OSE_NEW(CBasicCamera());
		g_pBasicCamera->Initialize();

		if (!m_pRenderer3D->Initialize(hWnd, g_pBasicCamera))
			return false;

		m_bInitialized = true;
		return true;
	}

	return false;
}

void COneShotRenderer3D::Update()
{
	if (m_bInitialized)
	{
		//CShaderManager::GetInstance()->Update();
		m_pRenderer3D->PreDraw();
		m_pRenderer3D->Draw();
		m_pRenderer3D->PostDraw();
	}
}

void COneShotRenderer3D::Resize()
{
	m_pRenderer3D->Resize();
	//CShaderManager::GetInstance()->Resize();
}

void COneShotRenderer3D::Shutdown()
{
	OSE_SAFE_SHUTDOWN(m_pRenderer3D);
}