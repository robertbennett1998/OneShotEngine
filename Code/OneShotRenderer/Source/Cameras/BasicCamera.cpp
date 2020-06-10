#include "pch.h"
#include "Cameras/BasicCamera.h"
#include <OneShotCore/Include/Events/KeyboardInputEventManager.h>

#include "OneShotCore/Include/Events/MouseInputEventManager.h"

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
		CMouseInputEventManager::GetInstance()->AddEventHandler(this);
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
	static float fTranslationSpeed = 1.0f;
	static float fRotationSpeedRadsPerSecond = DirectX::XM_PIDIV4;


	if (pKeyInputEventManager->IsKeyPressed(VK_SHIFT))
	{
		fTranslationSpeed = 10.0f;
		fRotationSpeedRadsPerSecond = DirectX::XM_PIDIV2;
	}
	else
	{
		fTranslationSpeed = 1.0f;
		fRotationSpeedRadsPerSecond = DirectX::XM_PIDIV4;
	}

	// W
	if (pKeyInputEventManager->IsKeyPressed(0x57))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(xmvLookAtPosition, fTranslationSpeed * dDeltaTime)));
	}

	// S
	if (pKeyInputEventManager->IsKeyPressed(0x53))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(xmvLookAtPosition, -fTranslationSpeed * dDeltaTime)));
	}

	// A
	if (pKeyInputEventManager->IsKeyPressed(0x41))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(DirectX::XMVector3Cross(xmvLookAtPosition, xmvUp), fTranslationSpeed * dDeltaTime)));
	}

	// D
	if (pKeyInputEventManager->IsKeyPressed(0x44))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(DirectX::XMVector3Cross(xmvLookAtPosition, xmvUp), -fTranslationSpeed * dDeltaTime)));
	}

	// Z
	if (pKeyInputEventManager->IsKeyPressed(0x5A))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(xmvUp, fTranslationSpeed * dDeltaTime)));
	}

	// X
	if (pKeyInputEventManager->IsKeyPressed(0x58))
	{
		DirectX::XMStoreFloat3(&m_xmf3Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Pos), DirectX::XMVectorScale(xmvUp, -fTranslationSpeed * dDeltaTime)));
	}

	//E
	if (pKeyInputEventManager->IsKeyPressed(VK_RIGHT) || pKeyInputEventManager->IsKeyPressed(0x45))
	{
		DirectX::XMStoreFloat3(&m_xmf3Rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Rotation), DirectX::XMVectorScale(xmvUp, fRotationSpeedRadsPerSecond * dDeltaTime)));
	}

	//Q
	if (pKeyInputEventManager->IsKeyPressed(VK_LEFT) || pKeyInputEventManager->IsKeyPressed(0x51))
	{
		DirectX::XMStoreFloat3(&m_xmf3Rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Rotation), DirectX::XMVectorScale(xmvUp, -fRotationSpeedRadsPerSecond * dDeltaTime)));
	}

	if (pKeyInputEventManager->IsKeyPressed(VK_UP))
	{
		DirectX::XMStoreFloat3(&m_xmf3Rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Rotation), DirectX::XMVectorScale(xmvRight, -fRotationSpeedRadsPerSecond * dDeltaTime)));
	}

	if (pKeyInputEventManager->IsKeyPressed(VK_DOWN))
	{
		DirectX::XMStoreFloat3(&m_xmf3Rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmf3Rotation), DirectX::XMVectorScale(xmvRight, fRotationSpeedRadsPerSecond * dDeltaTime)));
	}

	if (m_xmf3Rotation.x > DirectX::XM_PIDIV2 - 0.1f)
	{
		m_xmf3Rotation.x = DirectX::XM_PIDIV2 - 0.1f;
	}
	else if (m_xmf3Rotation.x < -DirectX::XM_PIDIV2 + 0.1f)
	{
		m_xmf3Rotation.x = -DirectX::XM_PIDIV2 + 0.1f;
	}
}

void CBasicCamera::Shutdown()
{
	if (m_bInitialized)
	{
		CKeyboardInputEventManager::GetInstance()->RemoveEventHandler(this);
		CMouseInputEventManager::GetInstance()->RemoveEventHandler(this);
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

void CBasicCamera::OnKeyEvent(UINT uiKey, bool bUp)
{
	if (uiKey == VK_SPACE && bUp == false)
	{
		SetCameraPosition(DirectX::XMFLOAT3(0.0f, 1.8f, 0.0f));
		SetCameraRotation(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
}

void CBasicCamera::OnMouseMoveEvent(UINT uiNewX, UINT uiNewY, UINT uiPrevX, UINT uiPrevY)
{
	if (CMouseInputEventManager::GetInstance()->GetRightMouseButtonDown())
	{
		OSE_LOG_INFO("General", "Mouse Moved - New Pos (%, %) - Old Pos (%, %)", uiNewX, uiNewY, uiPrevX, uiPrevY);
	}
}