#pragma once
#ifndef ONE_SHOT_RENDERER_BASIC_CAMERA_H
#define ONE_SHOT_RENDERER_BASIC_CAMERA_H

#include <DirectXMath.h>

#include "Cameras/Interfaces/ICamera.h"
#include <OneShotCore/Include/Events/KeyboardInputEventHandler.h>

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CBasicCamera : public ICamera, public CKeyboardInputEventHandler
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
		virtual void OnKeyboardEvent(UINT uiKey, bool bUp) override;
	};
};

#endif