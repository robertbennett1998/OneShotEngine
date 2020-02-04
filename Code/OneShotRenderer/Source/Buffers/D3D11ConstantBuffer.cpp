#include "pch.h"
#include "Buffers/D3D11ConstantBuffer.h"
#include "Direct3D.h"

#include <DirectXMath.h>
#include <stdlib.h>
#include <sstream>

using namespace OneShotRenderer;

CD3D11ConstantBuffer::CD3D11ConstantBuffer(CDirect3D * pDirect3D) :
m_bInitialized(false),
m_pData(nullptr),
m_szDataSize(0),
m_pDirect3D(pDirect3D),
m_pD3D11Buffer(nullptr)
{
}

CD3D11ConstantBuffer::~CD3D11ConstantBuffer()
{
}

bool CD3D11ConstantBuffer::Initialize(void * pData, size_t szDataSize)
{
	if (!m_bInitialized && pData != nullptr)
	{
		m_pData = pData;
		m_szDataSize = szDataSize;

		HRESULT hRes = S_OK;
		D3D11_BUFFER_DESC bd; ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = szDataSize;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hRes = m_pDirect3D->GetD3D11Device()->CreateBuffer(&bd, NULL, &m_pD3D11Buffer);
		if (FAILED(hRes))
		{
			return false;
		}

		m_bInitialized = true;

		UpdateBuffer();

		return true;	
	}

	return false;
}

void CD3D11ConstantBuffer::UpdateBuffer()
{
	if (m_bInitialized)
	{
		D3D11_MAP d3d11MapType = D3D11_MAP_WRITE_DISCARD; UINT d3d11MapFlags = 0;
		D3D11_MAPPED_SUBRESOURCE msr; ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HRESULT hRes = m_pDirect3D->GetD3D11DeviceContext()->Map(m_pD3D11Buffer, 0, d3d11MapType, d3d11MapFlags, &msr);
		if (FAILED(hRes))
		{
			return;
		}

		memcpy_s(msr.pData, m_szDataSize, m_pData, m_szDataSize);

		DirectX::XMMATRIX xmm = *((DirectX::XMMATRIX*)(((char*)msr.pData) + sizeof(DirectX::XMMATRIX) * 2));

		m_pDirect3D->GetD3D11DeviceContext()->Unmap(m_pD3D11Buffer, 0);
	}
}

void CD3D11ConstantBuffer::Bind(UINT uiSlot, UINT uiNumberOfBuffers, CONSTANT_BUFFER_USAGE buffUsage)
{
	switch (buffUsage)
	{
		default:
			break;

		case CONSTANT_BUFFER_USAGE::VertextShader:
			m_pDirect3D->GetD3D11DeviceContext()->VSSetConstantBuffers(uiSlot, uiNumberOfBuffers, &m_pD3D11Buffer);
			break;

		case CONSTANT_BUFFER_USAGE::PixelShader:
			m_pDirect3D->GetD3D11DeviceContext()->PSSetConstantBuffers(uiSlot, uiNumberOfBuffers, &m_pD3D11Buffer);
			break;
	}
}

void CD3D11ConstantBuffer::Shutdown()
{
	if (m_bInitialized)
	{
		SafeComRelease(m_pD3D11Buffer);

		m_bInitialized = false;
	}
}