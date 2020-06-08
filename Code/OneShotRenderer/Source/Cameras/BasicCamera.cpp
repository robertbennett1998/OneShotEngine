#include "pch.h"
#include "Cameras/BasicCamera.h"
#include <OneShotCore/Include/Events/KeyboardInputEventManager.h>

using namespace OneShotRenderer;

CBasicCamera::CBasicCamera() :
	m_bInitialized(false),
	m_xmf3Pos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
	m_xmf3Rotation(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
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
		CKeyboardInputEventManager::GetInstance()->AddEventHandler(this);
		m_bInitialized = true;
		return true;
	}

	return false;
}

void CBasicCamera::Update(const double dDeltaTime)
{
	auto* pKeyInputEventManager = CKeyboardInputEventManager::GetInstance();
	DirectX::XMVECTOR xmvLookAtPosition = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&m_xmf3LookAtPos), DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&m_xmf3Rotation)));

	static DirectX::XMVECTOR xmvForward = DirectX::XMVECTOR({ 0.0f, 1.0f, 0.0f });
	static DirectX::XMVECTOR xmvUp = DirectX::XMVECTOR({ 0.0f, 1.0f, 0.0f });
	static DirectX::XMVECTOR xmvRight = DirectX::XMVECTOR({ 1.0f, 0.0f, 0.0f });
	static double dTranslationSpeed = 1.0f;
	static double dRotationSpeedRadsPerSecond = DirectX::XM_PIDIV4;


	if (pKeyInputEventManager->IsKeyPresed(VK_SHIFT))
	{
		dTranslationSpeed = 10.0f;
		dRotationSpeedRadsPerSecond = DirectX::XM_PIDIV2;
	}
	else
	{
		dTranslationSpeed = 1.0f;
		dRotationSpeedRadsPerSecond = DirectX::XM_PIDIV4;
	}

	// W
	if (pKeyInputEventManager->IsKeyPresed(0x57))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(xmvLookAtPosition, dTranslationSpeed * dDeltaTime)));
	}

	// S
	if (pKeyInputEventManager->IsKeyPresed(0x53))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(xmvLookAtPosition, -dTranslationSpeed * dDeltaTime)));
	}

	// A
	if (pKeyInputEventManager->IsKeyPresed(0x41))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(DirectX::XMVector3Cross(xmvLookAtPosition, xmvUp), dTranslationSpeed * dDeltaTime)));
	}

	// D
	if (pKeyInputEventManager->IsKeyPresed(0x44))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(DirectX::XMVector3Cross(xmvLookAtPosition, xmvUp), -dTranslationSpeed * dDeltaTime)));
	}

	// Z
	if (pKeyInputEventManager->IsKeyPresed(0x5A))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(DirectX::XMVector3Cross(xmvLookAtPosition, xmvRight), dTranslationSpeed * dDeltaTime)));
	}

	// X
	if (pKeyInputEventManager->IsKeyPresed(0x58))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(DirectX::XMVector3Cross(xmvLookAtPosition, xmvRight), -dTranslationSpeed * dDeltaTime)));
	}

	if (pKeyInputEventManager->IsKeyPresed(VK_RIGHT))
	{
		DirectX::XMStoreFloat3(&m_xmf3Rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Rotation), DirectX::XMVectorScale(xmvUp, dRotationSpeedRadsPerSecond * dDeltaTime)));
	}

	if (pKeyInputEventManager->IsKeyPresed(VK_LEFT))
	{
		DirectX::XMStoreFloat3(&m_xmf3Rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Rotation), DirectX::XMVectorScale(xmvUp, -dRotationSpeedRadsPerSecond * dDeltaTime)));
	}

	if (pKeyInputEventManager->IsKeyPresed(VK_UP))
	{
		DirectX::XMStoreFloat3(&m_xmf3Rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Rotation), DirectX::XMVectorScale(xmvRight, -dRotationSpeedRadsPerSecond * dDeltaTime)));
	}

	if (pKeyInputEventManager->IsKeyPresed(VK_DOWN))
	{
		DirectX::XMStoreFloat3(&m_xmf3Rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Rotation), DirectX::XMVectorScale(xmvRight, dRotationSpeedRadsPerSecond * dDeltaTime)));
	}

	if (m_xmf3Rotation.x > DirectX::XM_PIDIV2 - 0.1)
	{
		m_xmf3Rotation.x = DirectX::XM_PIDIV2 - 0.1;
	}
	else if (m_xmf3Rotation.x < -DirectX::XM_PIDIV2 + 0.1)
	{
		m_xmf3Rotation.x = -DirectX::XM_PIDIV2 + 0.1;
	}
}

void CBasicCamera::Shutdown()
{
	if (m_bInitialized)
	{
		CKeyboardInputEventManager::GetInstance()->RemoveEventHandler(this);
		m_xmmViewMatrix = DirectX::XMMatrixIdentity();
		m_bInitialized = false;
	}
}

void CBasicCamera::SetCameraPosition(DirectX::XMFLOAT3 xmf3Pos)
{
	m_xmf3Pos = xmf3Pos;
}

void CBasicCamera::SetCameraRotation(DirectX::XMFLOAT3 xmf3Rotation)
{
	m_xmf3Rotation = xmf3Rotation;
}

DirectX::XMMATRIX CBasicCamera::GetViewMatrix()
{
	m_xmmViewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorAdd(DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&m_xmf3LookAtPos), DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&m_xmf3Rotation))), DirectX::XMLoadFloat3(&m_xmf3Pos)), DirectX::XMLoadFloat3(&m_xmf3UpDir));
	return m_xmmViewMatrix;
}

void CBasicCamera::OnKeyboardEvent(UINT uiKey, bool bUp)
{
	if (uiKey == VK_SPACE && bUp == false)
	{
		SetCameraPosition(DirectX::XMFLOAT3(0.0f, 1.8f, 0.0f));
		SetCameraRotation(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
}