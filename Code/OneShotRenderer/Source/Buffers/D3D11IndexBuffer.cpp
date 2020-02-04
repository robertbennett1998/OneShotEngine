#include "pch.h"
#include "Buffers/D3D11IndexBuffer.h"
#include "Direct3D.h"
#include <sstream>

using namespace OneShotRenderer;

CD3D11IndexBuffer::CD3D11IndexBuffer(CDirect3D * pDirect3D) :
m_bInitialized(false),
m_pDirect3D(pDirect3D),
m_pD3D11IndexBuffer(nullptr)
{
}

CD3D11IndexBuffer::~CD3D11IndexBuffer()
{
}

bool CD3D11IndexBuffer::Initialize(void * pIndexData, size_t szIndexByteWidth, UINT uiNumIndicies)
{
	if (!m_bInitialized)
	{
		HRESULT hRes = S_OK;

		D3D11_BUFFER_DESC ibd; ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.ByteWidth = szIndexByteWidth * uiNumIndicies;
		ibd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA srd; ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));
		srd.pSysMem = pIndexData;

		hRes = m_pDirect3D->GetD3D11Device()->CreateBuffer(&ibd, &srd, &m_pD3D11IndexBuffer);
		if (FAILED(hRes))
			return false;

		m_bInitialized = true;
		return true;
	}

	return false;
}

void CD3D11IndexBuffer::Bind()
{
	m_pDirect3D->GetD3D11DeviceContext()->IASetIndexBuffer(m_pD3D11IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void CD3D11IndexBuffer::Shutdown()
{
	if (m_bInitialized)
	{
		SafeComRelease(m_pD3D11IndexBuffer);

		m_bInitialized = false;
	}
}