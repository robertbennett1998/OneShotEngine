#pragma once
#ifndef ONE_SHOT_RENDERER_BASIC_CAMERA_H
#define ONE_SHOT_RENDERER_BASIC_CAMERA_H

#include <DirectXMath.h>

#include "Cameras/Interfaces/ICamera.h"
#include <OneShotCore/Include/Events/KeyboardInputEventHandler.h>
#include "OneShotCore/Include/Events/MouseInputEventHandler.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CBasicCamera : public ICamera, public CKeyboardInputEventHandler, public CMouseInputEventHandler
	{
	public:
		CBasicCamera();
		~CBasicCamera();

		bool Initialize() override;
		void Update(const double dDeltaTime) override;
		void Shutdown() override;

		void SetCameraPosition(DirectX::XMFLOAT3 xmf3Pos);
		void SetCameraRotation(DirectX::XMFLOAT3 xmf3Rotation);

		DirectX::XMMATRIX GetViewMatrix() override;

	private:
		bool m_bInitialized;
		DirectX::XMFLOAT3 m_xmf3Pos;
		DirectX::XMFLOAT3 m_xmf3Rotation;
		DirectX::XMFLOAT3 m_xmf3LookAtPos;
		DirectX::XMFLOAT3 m_xmf3UpDir;
		DirectX::XMMATRIX m_xmmViewMatrix;

		// Inherited via CKeyboardInputEventHandler
		void OnKeyEvent(UINT uiKey, bool bUp) override;
		void OnMouseMoveEvent(UINT uiNewX, UINT uiNewY, UINT uiPrevX, UINT uiPrevY) override;
		void OnMiddleMouseButtonEvent(UINT uiX, UINT uiY, bool bDown) override {};
		void OnRightMouseButtonEvent(UINT uiX, UINT uiY, bool bDown) override {};
		void OnLeftMouseButtonEvent(UINT uiX, UINT uiY, bool bDown) override {};
	};
};

#endif