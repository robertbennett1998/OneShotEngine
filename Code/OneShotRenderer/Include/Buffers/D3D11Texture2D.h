#pragma once
#ifndef ONE_SHOT_RENDERER_D3D11_TEXTURE_2D_H
#define ONE_SHOT_RENDERER_D3D11_TEXTURE_2D_H

#include <d3d11.h>

#include "Buffers/Interfaces/ITexture2D.h"

namespace OneShotRenderer
{
	class CDirect3D;
	class ONE_SHOT_RENDERER_DLL CD3D11Texture2D : public ITexture2D
	{
		public:
			CD3D11Texture2D(CDirect3D* pDirect3D);
			~CD3D11Texture2D();

			bool Initialize(std::string sTexturePath) override;
			void BindToPixelShader(uint8_t iSlot = 0) const override;
			void Shutdown() override;

		private:
			CD3D11Texture2D();

			bool m_bInitialzied;
			ID3D11Texture2D* m_pD3D11Texture2D;
			ID3D11ShaderResourceView* m_pD3D11ShaderResourceView;
			ID3D11SamplerState* m_pD3D11SamplerState;
			CDirect3D* m_pDirect3D;
	};
};

#endif