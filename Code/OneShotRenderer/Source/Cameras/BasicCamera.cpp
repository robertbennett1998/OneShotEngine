#include "pch.h"
#include "Cameras/BasicCamera.h"
#include <OneShotCore/Include/Events/KeyboardInputEventManager.h>

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
		CKeyboardInputEventManager::GetInstance()->AddEventHandler(this);
		m_bInitialized = true;
		return true;
	}

	return false;
}

void CBasicCamera::Update(const double dDeltaTime)
{
	auto keyInputEventManager = CKeyboardInputEventManager::GetInstance();
	static DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	static DirectX::XMFLOAT3 right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

	static double scale = 1.0f;
	if (keyInputEventManager->IsKeyPresed(VK_SHIFT))
		scale = 10.0f;
	else
		scale = 1.0f;

	// W
	if (keyInputEventManager->IsKeyPresed(0x57))
	{
		auto translation = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_xmf3LookAtPos), -dDeltaTime * scale));
		m_xmmViewMatrix = DirectX::XMMatrixMultiply(m_xmmViewMatrix, translation);
	}

	// S
	if (keyInputEventManager->IsKeyPresed(0x53))
	{
		auto translation = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_xmf3LookAtPos), dDeltaTime * scale));
		m_xmmViewMatrix = DirectX::XMMatrixMultiply(m_xmmViewMatrix, translation);
	}

	// A
	if (keyInputEventManager->IsKeyPresed(0x41))
	{
		auto translation = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&m_xmf3LookAtPos), DirectX::XMLoadFloat3(&up)), -dDeltaTime * scale));
		m_xmmViewMatrix = DirectX::XMMatrixMultiply(m_xmmViewMatrix, translation);
	}

	// D
	if (keyInputEventManager->IsKeyPresed(0x44))
	{
		auto translation = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&m_xmf3LookAtPos), DirectX::XMLoadFloat3(&up)), dDeltaTime * scale));
		m_xmmViewMatrix = DirectX::XMMatrixMultiply(m_xmmViewMatrix, translation);
	}

	if (keyInputEventManager->IsKeyPresed(VK_RIGHT))
	{
		auto rotation = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&up), -DirectX::XM_PIDIV4 * 0.1 * scale * dDeltaTime);
		m_xmmViewMatrix = DirectX::XMMatrixMultiply(m_xmmViewMatrix, rotation);
	}

	if (keyInputEventManager->IsKeyPresed(VK_LEFT))
	{
		auto rotation = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&up), DirectX::XM_PIDIV4 * 0.1 * scale * dDeltaTime);
		m_xmmViewMatrix = DirectX::XMMatrixMultiply(m_xmmViewMatrix, rotation);
	}

	if (keyInputEventManager->IsKeyPresed(VK_UP))
	{
		auto rotation = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&right), DirectX::XM_PIDIV4 * 0.1 * scale * dDeltaTime);
		m_xmmViewMatrix = DirectX::XMMatrixMultiply(m_xmmViewMatrix, rotation);
	}

	if (keyInputEventManager->IsKeyPresed(VK_DOWN))
	{
		auto rotation = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&right), -DirectX::XM_PIDIV4 * 0.1 * scale * dDeltaTime);
		m_xmmViewMatrix = DirectX::XMMatrixMultiply(m_xmmViewMatrix, rotation);
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

void OneShotRenderer::CBasicCamera::OnKeyboardEvent(UINT uiKey, bool bUp)
{
	if (uiKey == VK_SPACE && bUp == false)
	{
		SetCameraPosition(DirectX::XMFLOAT3(0.0f, 1.8f, 0.0f));
		SetCameraLookAtPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));
	}
}