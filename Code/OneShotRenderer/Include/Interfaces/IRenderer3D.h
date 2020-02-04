#pragma once
#ifndef ONE_SHOT_RENDERER_I_RENDERER_3D_H
#define ONE_SHOT_RENDERER_I_RENDERER_3D_H

#include "Shaders/Interfaces/IVertexShader.h"
#include "Shaders/Interfaces/IPixelShader.h"
#include "Buffers/Interfaces/IConstantBuffer.h"
#include "Buffers/Interfaces/IVertexBuffer.h"
#include "Buffers/Interfaces/IIndexBuffer.h"
#include "Renderables/Interfaces/IRenderable.h"
#include "Cameras/Interfaces/ICamera.h"
#include "Buffers/Interfaces/ITexture2D.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL IRenderer3D
	{
		public:
			virtual bool Initialize(HWND hWnd, ICamera* pCamera = nullptr) = 0;
			virtual void PreDraw() = 0;
			virtual void Draw() = 0;
			virtual void PostDraw() = 0;
			virtual void Resize() = 0;
			virtual void Shutdown() = 0;

			virtual bool RegisterRenderable(IRenderable* pRenderable) = 0;
			virtual bool UnregisterRenderable(IRenderable* pRenderable) = 0;

			virtual IVertexShader* CreateVertexShader() = 0;
			virtual IPixelShader* CreatePixelShader() = 0;
			virtual IConstantBuffer* CreateConstantBuffer() = 0;
			virtual IVertexBuffer* CreateVertexBuffer() = 0;
			virtual IIndexBuffer* CreateIndexBuffer() = 0;
			virtual ITexture2D* CreateTexture2D() = 0;
			virtual float GetViewportWidth() const = 0;
			virtual float GetViewportHeight() const = 0;
			virtual void BindCamera(ICamera* pCamera) = 0;
			virtual ICamera* GetBoundCamera() const = 0;
	};
}

#endif