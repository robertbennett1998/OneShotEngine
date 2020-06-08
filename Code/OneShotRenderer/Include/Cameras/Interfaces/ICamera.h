#pragma once
#ifndef ONE_SHOT_RENDERER_I_CAMERA_H
#define ONE_SHOT_RENDERER_I_CAMERA_H

#include <DirectXMath.h>

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL ICamera
	{
	public:
		virtual bool Initialize() = 0;
		virtual void Update(const double dDeltaTime) = 0;
		virtual void Shutdown() = 0;

		virtual DirectX::XMMATRIX GetViewMatrix() = 0;
	};
};

#endif