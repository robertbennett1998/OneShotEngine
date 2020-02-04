#pragma once
#ifndef GAME_ENGINE_RENDERER_D3D11_PIXEL_SHADER_H
#define GAME_ENGINE_RENDERER_D3D11_PIXEL_SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>

#include "Shaders/Interfaces/IPixelShader.h"

namespace OneShotRenderer
{
	class CDirect3D;
	class ONE_SHOT_RENDERER_DLL CD3D11PixelShader : public IPixelShader
	{
	public:
		CD3D11PixelShader(CDirect3D* pDirect3D);
		~CD3D11PixelShader();

		// Inherited via IPixelShader
		virtual bool Initialize(std::string sPSFile, std::string sPSEntryPoint = "main");
		virtual void Bind();
		virtual void Shutdown();

	private:
		CD3D11PixelShader();

		bool m_bInitialized;
		CDirect3D* m_pDirect3D;
		ID3D11PixelShader* m_pD3D11PixelShader;
	};
}

#endif