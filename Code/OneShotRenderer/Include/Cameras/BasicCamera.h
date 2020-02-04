#pragma once
#ifndef ONE_SHOT_RENDERER_BASIC_CAMERA_H
#define ONE_SHOT_RENDERER_BASIC_CAMERA_H

#include <DirectXMath.h>

#include "Cameras/Interfaces/ICamera.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CBasicCamera : public ICamera
	{
		public:
			CBasicCamera();
			~CBasicCamera();

			bool Initialize() override;
			void Update() override;
			void Shutdown() override;

			void SetCameraPosition(DirectX::XMFLOAT3 xmf3Pos) override;
			void SetCameraLookAtPosition(DirectX::XMFLOAT3 xmf3LookatPos) override;

			DirectX::XMMATRIX GetViewMatrix() const override;

		private:
			bool m_bInitialized;
			DirectX::XMFLOAT3 m_xmf3Pos;
			DirectX::XMFLOAT3 m_xmf3LookAtPos;
			DirectX::XMFLOAT3 m_xmf3UpDir;
			DirectX::XMMATRIX m_xmmViewMatrix;
	};
};

#endif