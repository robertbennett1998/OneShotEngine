#pragma once
#ifndef ONE_SHOT_RENDERER_DIRECT_3D_H
#define ONE_SHOT_RENDERER_DIRECT_3D_H

#include <d3d11.h>

#include "Interfaces/IRenderer3D.h"
#include "Buffers/Interfaces/IConstantBuffer.h"
#include "Buffers/Interfaces/IVertexBuffer.h"
#include "Buffers/Interfaces/IIndexBuffer.h"
#include "Renderables/Interfaces/IRenderable.h"
#include "Cameras/Interfaces/ICamera.h"

#include "Shaders/D3D11PixelShader.h"
#include "Shaders/D3D11VertexShader.h"
#include "Buffers/D3D11VertexBuffer.h"
#include "Buffers/D3D11IndexBuffer.h"
#include "Buffers/D3D11ConstantBuffer.h"
#include "Buffers/D3D11Texture2D.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CDirect3D : public IRenderer3D
	{
		public:
			CDirect3D();
			virtual ~CDirect3D();

			bool Initialize(HWND hWnd, ICamera* pCamera = nullptr) override;
			void PreDraw() override;
			void Draw() override;
			void PostDraw() override;
			void Resize() override;
			void Shutdown() override;

			bool RegisterRenderable(IRenderable* pRenderable) override;
			bool UnregisterRenderable(IRenderable* pRenderable) override;

			IVertexShader* CreateVertexShader() override;
			IPixelShader* CreatePixelShader() override;
			IConstantBuffer* CreateConstantBuffer() override;
			IVertexBuffer* CreateVertexBuffer() override;
			IIndexBuffer* CreateIndexBuffer() override;
			ITexture2D* CreateTexture2D() override;

			float GetViewportWidth() const override
			{
				return m_fViewPortWidth;
			}

			float GetViewportHeight() const override
			{
				return m_fViewPortHeight;
			}

			void BindCamera(ICamera* pCamera) override
			{
				if (pCamera == nullptr)
					return;

				m_pBoundCamera = pCamera;
			}

			ICamera* GetBoundCamera() const override
			{
				return m_pBoundCamera;
			}

			ID3D11Device* GetD3D11Device() const
			{
				return m_pD3D11Device;
			}

			ID3D11DeviceContext* GetD3D11DeviceContext() const
			{
				return m_pD3D11DeviceContext;
			}

		private:
			bool m_bInitialized;

			inline bool CreateD3D11Device();
			inline bool CreateDXGISwapchain(IDXGIDevice* pDxgiDevice);
			inline bool CreateD3D11RenderTargetView();
			inline bool CreateD3D11DepthStencilBuffer();
			inline bool CreateD3D11DepthStencilState();
			inline bool CreateD3D11DepthStencilView();

			HWND m_hWnd;
			ID3D11Device* m_pD3D11Device;
			ID3D11DeviceContext* m_pD3D11DeviceContext;
			IDXGISwapChain* m_pDXGISwapChain;
			ID3D11RenderTargetView* m_pD3D11RenderTargetView;
			ID3D11Texture2D* m_pD3D11DepthStencilBuffer;
			ID3D11DepthStencilState* m_pD3D11DepthStencilState;
			ID3D11DepthStencilView* m_pD3D11DepthStencilView;
			ICamera* m_pBoundCamera;
			float m_fViewPortWidth, m_fViewPortHeight;
			std::vector<IRenderable*> m_pRenderables;
	};
};

#endif