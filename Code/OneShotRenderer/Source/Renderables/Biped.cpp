#include "pch.h"
#include "Renderables/Biped.h"
#include "GeometryManager.h"
#include "Shaders/ShaderManager.h"
#include "Shaders/BasicColourShaderProgram.h"

using namespace DirectX;
using namespace OneShotRenderer;

CBiped::CBiped() :
	m_xmmWorldMatrix(XMMatrixIdentity())
{
}

CBiped::~CBiped()
{
}

bool CBiped::Initialize()
{
	CGeometryManager* pGeomManager = CGeometryManager::GetInstance();
	if (pGeomManager == nullptr)
		return false;

	m_pGeometry = pGeomManager->GetGeomInstance("Biped");
	if (m_pGeometry == nullptr)
		return false;

	m_pShaderProgram = CShaderManager::GetInstance()->GetShaderInstance<CBasicColourShaderProgram>("BasicColour");
	if (m_pShaderProgram == nullptr)
		return false;

	m_bShouldRender = true;

	return true;
}

void CBiped::Bind()
{
	((CBasicColourShaderProgram*)m_pShaderProgram)->SetWorldMatrix(m_xmmWorldMatrix);
	m_pShaderProgram->Bind();
}

void CBiped::Shutdown()
{
}

void CBiped::Hide()
{
	m_bShouldRender = false;
}

void CBiped::Show()
{
	m_bShouldRender = true;
}

void CBiped::ToggleVisible()
{
	m_bShouldRender = !m_bShouldRender;
}

void CBiped::SetWorldMatrix(DirectX::XMMATRIX xmmWorld)
{
	m_xmmWorldMatrix = xmmWorld;
}