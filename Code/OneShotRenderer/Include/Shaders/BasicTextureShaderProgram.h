#pragma once
#ifndef ONE_SHOT_RENDERER_BASIC_TEXTURE_SHADER_PROGRAM_H
#define ONE_SHOT_RENDERER_BASIC_TEXTURE_SHADER_PROGRAM_H

#include <d3d11.h>
#include <DirectXMath.h>

#include "Shaders/Interfaces/IShaderProgram.h"
#include "Shaders/Interfaces/IVertexShader.h"
#include "Shaders/Interfaces/IPixelShader.h"
#include "Interfaces/IRenderer3D.h"
#include "Shaders/ShaderParams.h"

class ONE_SHOT_RENDERER_DLL CBasicTextureShaderProgram : public OneShotRenderer::IShaderProgram
{
	public:
		CBasicTextureShaderProgram(OneShotRenderer::IRenderer3D* p3DRenderer);
		~CBasicTextureShaderProgram();

		bool Initialize() override;
		void Update() override;
		void Bind() override;
		void Resize() override;
		void Shutdown() override;

		void SetWorldMatrix(DirectX::XMMATRIX xmmWorld);

	private:
		bool m_bInitialized;
		OneShotRenderer::IRenderer3D* m_p3DRenderer;
		OneShotRenderer::IVertexShader* m_pVertexShader;
		OneShotRenderer::IPixelShader* m_pPixelShader;
		DirectX::XMMATRIX m_xmmWorldMatrix;
		DirectX::XMMATRIX m_xmmViewMatrix;
		DirectX::XMMATRIX m_xmmProjectionMatrix;
		OneShotRenderer::CShaderParams<DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX>* m_pMatrixBuffer;
};

#endif