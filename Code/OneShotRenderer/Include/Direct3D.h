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
	const UINT UNSUPPORTED_MSAA_COUNT = (UINT)-1;
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

			UINT GetMsaaCount() const
			{
				return m_uiMsaaCount;
			}

			bool SetMsaaCount(const UINT uiMsaaCount)
			{
				UINT uiMsaaQuality = GetMsaaQuality(uiMsaaCount);
				if (uiMsaaQuality == UNSUPPORTED_MSAA_COUNT)
					return false;

				m_uiMsaaCount = uiMsaaCount;
				m_uiMsaaQuality = uiMsaaQuality;

				return true;
			}

			UINT GetMaxSupportedMsaaCount() const
			{
				return m_uiMsaaMaxCount;
			}

			void DetectHighestMsaaSettings()
			{
				HRESULT hRes = S_OK;
				UINT uiCount = 1;
				UINT uiQualityLevels = 0;
				UINT uiPrevQualityLevels = 0;

				do
				{
					uiPrevQualityLevels = uiQualityLevels;
					uiQualityLevels = GetMsaaQuality(uiCount);

					uiCount *= 2;
				} while (!FAILED(hRes) && uiQualityLevels != UNSUPPORTED_MSAA_COUNT); //run until it fails

				m_uiMsaaMaxCount = uiCount / 4; //div by 2 for the one it failed on and div by 2 because it was multiplied by two after it failed
			}

			UINT GetMsaaQuality(const UINT uiMsaaCount)
			{
				if (uiMsaaCount > D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT)
					return (UINT)-1;

				HRESULT hRes = S_OK;
				UINT uiQualityLevels = 0;

				hRes = m_pD3D11Device->CheckMultisampleQualityLevels(m_BackBufferFormat, uiMsaaCount, &uiQualityLevels);
				if (FAILED(hRes) || uiQualityLevels == 0)
					return (UINT)-1;

				return uiQualityLevels - 1;
			}

		private:
			bool m_bInitialized;

			inline bool LoadSettings();
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
			const DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			UINT m_uiMsaaMaxCount, m_uiMsaaCount;
			UINT m_uiMsaaQuality;
	};
};

#endif