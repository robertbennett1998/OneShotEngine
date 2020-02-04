#include "pch.h"
#include "Buffers/D3D11VertexBuffer.h"
#include "Direct3D.h"

using namespace OneShotRenderer;

CD3D11VertexBuffer::CD3D11VertexBuffer(CDirect3D * pDirect3D) :
m_bInitialized(false),
m_pDirect3D(pDirect3D),
m_pD3D11VertexBuffer(nullptr)
{
}

CD3D11VertexBuffer::~CD3D11VertexBuffer()
{
}

bool CD3D11VertexBuffer::Initialize(void * pVertexData, size_t szVertexSize, UINT uiNumIndicies)
{
	if (!m_bInitialized)
	{
		HRESULT hRes = S_OK;
		m_uiVertexSize = szVertexSize;

		D3D11_BUFFER_DESC vbd; ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.ByteWidth = szVertexSize * uiNumIndicies;
		vbd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA srd; ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));
		srd.pSysMem = pVertexData;

		hRes = m_pDirect3D->GetD3D11Device()->CreateBuffer(&vbd, &srd, &m_pD3D11VertexBuffer);
		if (FAILED(hRes))
			return false;

		m_bInitialized = true;
		return true;
	}

	return false;
}

void CD3D11VertexBuffer::Bind()
{
	UINT uiOffset = 0;
	m_pDirect3D->GetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_pD3D11VertexBuffer, &m_uiVertexSize, &uiOffset);
}

void CD3D11VertexBuffer::Shutdown()
{
	if (m_bInitialized)
	{
		SafeComRelease(m_pD3D11VertexBuffer);

		m_bInitialized = false;
	}
}