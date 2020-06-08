#include "pch.h"
#include "OneShotRenderer3D.h"
#include "Cameras/BasicCamera.h"
#include "MaterialLibrary.h"
#include "GeometryManager.h"
#include "Shaders/ShaderManager.h"
#include "Shaders/BasicColourShaderProgram.h"

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

		CMaterialLibrary::CreateInstance(m_pRenderer3D);
		CShaderManager::CreateInstance(m_pRenderer3D);
		
		CGeometryManager::CreateInstance(m_pRenderer3D);

		g_pBasicCamera = OSE_NEW(CBasicCamera());
		g_pBasicCamera->Initialize();

		g_pBasicCamera->SetCameraPosition(DirectX::XMFLOAT3(0.0f, 1.8f, 0.0f));
		g_pBasicCamera->SetCameraRotation(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));

		if (!m_pRenderer3D->Initialize(hWnd, g_pBasicCamera))
			return false;

		//TODO: Extend the geometry framework to allow different types...
		//if (!CShaderManager::GetInstance()->RegisterShaderType<CBasicColourShaderProgram>("BasicColour"))
		//	return false;

		m_bInitialized = true;
		return true;
	}

	return false;
}

void COneShotRenderer3D::Update(const double dDeltaTime)
{
	if (m_bInitialized)
	{
		CShaderManager::GetInstance()->Update();
		g_pBasicCamera->Update(dDeltaTime);
		m_pRenderer3D->PreDraw();
		m_pRenderer3D->Draw();
		m_pRenderer3D->PostDraw();
	}
}

void COneShotRenderer3D::Resize()
{
	m_pRenderer3D->Resize();
	CShaderManager::GetInstance()->Resize();
}

void COneShotRenderer3D::Shutdown()
{
	OSE_SAFE_SHUTDOWN(m_pRenderer3D);
	OSE_SAFE_SHUTDOWN(g_pBasicCamera);
	CShaderManager::GetInstance()->Shutdown();
	CMaterialLibrary::GetInstance()->Shutdown();
	CGeometryManager::GetInstance()->Shutdown();
}