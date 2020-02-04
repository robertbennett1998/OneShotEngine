#include "stdafx.h"
#include "DirectX/Shaders/BasicTextureShaderProgram.h"

using namespace DirectX;

CBasicTextureShaderProgram::CBasicTextureShaderProgram(I3DRenderer* p3DRenderer) :
m_bInitialized(false),
m_p3DRenderer(p3DRenderer),
m_pVertexShader(nullptr),
m_pPixelShader(nullptr),
m_pMatrixBuffer(nullptr),
m_xmmWorldMatrix(XMMatrixIdentity()),
m_xmmViewMatrix(XMMatrixIdentity()),
m_xmmProjectionMatrix(XMMatrixIdentity())
{
}

CBasicTextureShaderProgram::~CBasicTextureShaderProgram()
{
}

bool CBasicTextureShaderProgram::Initialize()
{
	if (!m_bInitialized)
	{
		D3D11_INPUT_ELEMENT_DESC polygonLayout[3]; ZeroMemory(&polygonLayout, sizeof(D3D11_INPUT_ELEMENT_DESC) * 3);
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "NORMAL";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		polygonLayout[2].SemanticName = "TEXCOORD";
		polygonLayout[2].SemanticIndex = 0;
		polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[2].InputSlot = 0;
		polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[2].InstanceDataStepRate = 0;

		m_pVertexShader = m_p3DRenderer->CreateVertexShader();
		if (!m_pVertexShader->Initialize("C:\\Programming\\Game_Engine\\Shaders\\BasicTexture\\BasicTexture.vs", &polygonLayout, 3, "main"))
			return false;

		m_pPixelShader = m_p3DRenderer->CreatePixelShader();
		if (!m_pPixelShader->Initialize("C:\\Programming\\Game_Engine\\Shaders\\BasicTexture\\BasicTexture.ps", "main"))
			return false;

		m_xmmProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_p3DRenderer->GetViewportWidth() / m_p3DRenderer->GetViewportHeight(), 0.01f, 1000.0f);

		m_pMatrixBuffer = new CShaderParams<DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX>(m_p3DRenderer, 0, CONSTANT_BUFFER_USAGE::VertextShader);
		if (!m_pMatrixBuffer->Initiailze(XMMatrixTranspose(m_xmmWorldMatrix), XMMatrixTranspose(m_xmmViewMatrix), XMMatrixTranspose(m_xmmProjectionMatrix)))
			return false;

		m_bInitialized = true;
		return true;
	}

	return false;
}

void CBasicTextureShaderProgram::Update()
{
	m_xmmViewMatrix = m_p3DRenderer->GetBoundCamera()->GetViewMatrix();
	m_pMatrixBuffer->SetValues(XMMatrixTranspose(m_xmmWorldMatrix), XMMatrixTranspose(m_xmmViewMatrix), XMMatrixTranspose(m_xmmProjectionMatrix));
}

void CBasicTextureShaderProgram::Bind()
{
	m_pVertexShader->Bind();
	m_pMatrixBuffer->Bind();
	m_pPixelShader->Bind();
}

void CBasicTextureShaderProgram::Resize()
{
	m_xmmProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_p3DRenderer->GetViewportWidth() / m_p3DRenderer->GetViewportHeight(), 0.01f, 1000.0f);
	m_pMatrixBuffer->SetValues(XMMatrixTranspose(m_xmmWorldMatrix), XMMatrixTranspose(m_xmmViewMatrix), XMMatrixTranspose(m_xmmProjectionMatrix));
}

void CBasicTextureShaderProgram::Shutdown()
{
	if (m_bInitialized)
	{
		SafeShutdown(m_pPixelShader);
		SafeShutdown(m_pMatrixBuffer);
		SafeShutdown(m_pVertexShader);

		m_bInitialized = false;
	}
}

void CBasicTextureShaderProgram::SetWorldMatrix(DirectX::XMMATRIX xmmWorld)
{
	m_xmmWorldMatrix = xmmWorld;
	m_pMatrixBuffer->SetValues(XMMatrixTranspose(m_xmmWorldMatrix), XMMatrixTranspose(m_xmmViewMatrix), XMMatrixTranspose(m_xmmProjectionMatrix));
}