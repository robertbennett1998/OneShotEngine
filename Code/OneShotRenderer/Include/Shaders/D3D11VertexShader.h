#pragma once
#ifndef GAME_ENGINE_RENDERER_D3D11_VERTEX_SHADER_H
#define GAME_ENGINE_RENDERER_D3D11_VERTEX_SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>

#include "Shaders/Interfaces/IVertexShader.h"

namespace OneShotRenderer
{
	class CDirect3D;
	class ONE_SHOT_RENDERER_DLL CD3D11VertexShader : public IVertexShader
	{
		public:
			CD3D11VertexShader(CDirect3D* pDirect3D);
			~CD3D11VertexShader();

			// Inherited via IVertexShader
			bool Initialize(std::string sVSFile, void* pInputLayoutDesc, uint8_t ubyNumInputElems, std::string sVSEntryPoint = "main") override;
			void Bind() override;
			void Shutdown() override;

		private:
			CD3D11VertexShader();

			bool m_bInitialized;
			CDirect3D* m_pDirect3D;
			ID3D11VertexShader* m_pD3D11VertexShader;
			ID3D11InputLayout* m_pD3D11InputLayout;
	};
};

#endif