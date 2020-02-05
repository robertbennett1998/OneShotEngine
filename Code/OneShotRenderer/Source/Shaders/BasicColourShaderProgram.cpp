#include "pch.h"
#include "Shaders/BasicColourShaderProgram.h"

using namespace DirectX;
using namespace OneShotRenderer;

CBasicColourShaderProgram::CBasicColourShaderProgram(OneShotRenderer::IRenderer3D* p3DRenderer) :
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

CBasicColourShaderProgram::~CBasicColourShaderProgram()
{
}

bool CBasicColourShaderProgram::Initialize()
{
	if (!m_bInitialized)
	{
		D3D11_INPUT_ELEMENT_DESC polygonLayout[2]; ZeroMemory(&polygonLayout, sizeof(D3D11_INPUT_ELEMENT_DESC) * 2);
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "COLOR";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		m_pVertexShader = m_p3DRenderer->CreateVertexShader();
		if (!m_pVertexShader->Initialize("/Shaders/BasicColour/BasicColour.vs", &polygonLayout, 2, "main"))
			return false;

		m_pPixelShader = m_p3DRenderer->CreatePixelShader();
		if (!m_pPixelShader->Initialize("/Shaders/BasicColour/BasicColour.ps", "main"))
			return false;

		static XMMATRIX world = XMMatrixIdentity();
		static XMMATRIX view = XMMatrixIdentity();
		static XMMATRIX proj = XMMatrixIdentity();
		XMFLOAT3 pos(0, 0, -5);
		XMFLOAT3 up(0, 1, 0);
		XMFLOAT3 look(0, 0, 1);

		//world = XMMatrixTranslation(0, 0, 5);
		view = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMVectorAdd(XMLoadFloat3(&look), XMLoadFloat3(&pos)), XMLoadFloat3(&up));
		proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_p3DRenderer->GetViewportWidth() / m_p3DRenderer->GetViewportHeight(), 0.01f, 1000.0f);
		
		world = XMMatrixTranspose(world);
		view = XMMatrixTranspose(view);
		proj = XMMatrixTranspose(proj);

		m_pMatrixBuffer = OSE_NEW_SUB OneShotRenderer::CShaderParams<DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX>(m_p3DRenderer, 0, CONSTANT_BUFFER_USAGE::VertextShader);
		if (!m_pMatrixBuffer->Initiailze(world, view, proj))
			return false;

		m_bInitialized = true;
		return true;
	}

	return false;
}

void CBasicColourShaderProgram::Bind()
{
	m_pVertexShader->Bind();
	m_pMatrixBuffer->Bind();
	m_pPixelShader->Bind();
}

void CBasicColourShaderProgram::Resize()
{
	m_xmmProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_p3DRenderer->GetViewportWidth() / m_p3DRenderer->GetViewportHeight(), 0.01f, 1000.0f);
	m_pMatrixBuffer->SetValues(XMMatrixTranspose(m_xmmWorldMatrix), XMMatrixTranspose(m_xmmViewMatrix), XMMatrixTranspose(m_xmmProjectionMatrix));
}

void CBasicColourShaderProgram::Shutdown()
{
	if (m_bInitialized)
	{
		OSE_SAFE_SHUTDOWN(m_pPixelShader);
		OSE_SAFE_SHUTDOWN(m_pMatrixBuffer);
		OSE_SAFE_SHUTDOWN(m_pVertexShader);

		m_bInitialized = false;
	}
}

void CBasicColourShaderProgram::SetWorldMatrix(DirectX::XMMATRIX xmmWorld)
{
	m_xmmWorldMatrix = xmmWorld;
	m_pMatrixBuffer->SetValues(XMMatrixTranspose(m_xmmWorldMatrix), XMMatrixTranspose(m_xmmViewMatrix), XMMatrixTranspose(m_xmmProjectionMatrix));
}

void CBasicColourShaderProgram::Update()
{
	m_xmmViewMatrix = m_p3DRenderer->GetBoundCamera()->GetViewMatrix();
	m_pMatrixBuffer->SetValues(XMMatrixTranspose(m_xmmWorldMatrix), XMMatrixTranspose(m_xmmViewMatrix), XMMatrixTranspose(m_xmmProjectionMatrix));
}