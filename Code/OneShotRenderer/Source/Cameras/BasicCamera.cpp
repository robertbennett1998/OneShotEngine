#include "pch.h"
#include "Cameras/BasicCamera.h"

using namespace OneShotRenderer;

CBasicCamera::CBasicCamera() :
	m_bInitialized(false),
	m_xmf3Pos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
	m_xmf3LookAtPos(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
	m_xmf3UpDir(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
	m_xmmViewMatrix(DirectX::XMMatrixIdentity())
{
}

CBasicCamera::~CBasicCamera()
{
}

bool CBasicCamera::Initialize()
{
	if (!m_bInitialized)
	{
		m_bInitialized = true;
		return true;
	}

	return false;
}

void CBasicCamera::Update()
{
}

void CBasicCamera::Shutdown()
{
	if (m_bInitialized)
	{
		m_xmmViewMatrix = DirectX::XMMatrixIdentity();
		m_bInitialized = false;
	}
}

void CBasicCamera::SetCameraPosition(DirectX::XMFLOAT3 xmf3Pos)
{
	m_xmf3Pos = xmf3Pos;
	m_xmmViewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3LookAtPos), DirectX::XMLoadFloat3(&m_xmf3Pos)), DirectX::XMLoadFloat3(&m_xmf3UpDir));
}

void CBasicCamera::SetCameraLookAtPosition(DirectX::XMFLOAT3 xmf3LookatPos)
{
	m_xmf3LookAtPos = xmf3LookatPos;
	m_xmmViewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3LookAtPos), DirectX::XMLoadFloat3(&m_xmf3Pos)), DirectX::XMLoadFloat3(&m_xmf3UpDir));
}

DirectX::XMMATRIX CBasicCamera::GetViewMatrix() const
{
	return m_xmmViewMatrix;
}
