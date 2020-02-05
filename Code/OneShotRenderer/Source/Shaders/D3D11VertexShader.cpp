#include "pch.h"
#include "Shaders/D3D11VertexShader.h"
#include "Direct3D.h"

#include <OneShotCore/Include/FileUtillities/VirtualFileSystem.h>

using namespace OneShotRenderer;

static std::wstring StringToWideString(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = OSE_NEW_ARRAY(wchar_t, len);
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	OSE_DELETE_ARRAY(buf);
	return r;
}


CD3D11VertexShader::CD3D11VertexShader(CDirect3D * pDirect3D) :
m_bInitialized(false),
m_pDirect3D(pDirect3D),
m_pD3D11VertexShader(nullptr),
m_pD3D11InputLayout(nullptr)
{
}

CD3D11VertexShader::~CD3D11VertexShader()
{
}

bool CD3D11VertexShader::Initialize(std::string sVSFile, void* pInputLayoutDesc, uint8_t ubyNumInputElems, std::string sVSEntryPoint)
{
	if (!m_bInitialized)
	{
		HRESULT hRes = S_OK;

		sVSFile = CVirtualFileSystem::GetInstance()->ResolvePhysicalPath(sVSFile);
		ID3D10Blob* pErrBlob = nullptr;
		ID3D10Blob* pVSBlob = nullptr;
		hRes = D3DCompileFromFile(StringToWideString(sVSFile).c_str(), NULL, NULL, sVSEntryPoint.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pVSBlob, &pErrBlob);
		if (FAILED(hRes))
		{
			if (pErrBlob != nullptr)
			{
				OutputDebugStringA((char*)pErrBlob->GetBufferPointer());
				SafeComRelease(pErrBlob);
				return false;
			}
			else
			{
				return false;
			}
		}

		hRes = m_pDirect3D->GetD3D11Device()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pD3D11VertexShader);
		if (FAILED(hRes))
		{
			SafeComRelease(pVSBlob);
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC* pD3D11ElementDesc = static_cast<D3D11_INPUT_ELEMENT_DESC*>(pInputLayoutDesc);
		if (pD3D11ElementDesc == nullptr)
		{
			SafeComRelease(m_pD3D11VertexShader);
			SafeComRelease(pVSBlob);
			return false;
		}

		hRes = m_pDirect3D->GetD3D11Device()->CreateInputLayout(pD3D11ElementDesc, ubyNumInputElems, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pD3D11InputLayout);
		if (FAILED(hRes))
		{
			SafeComRelease(m_pD3D11VertexShader);
			SafeComRelease(pVSBlob);
			return false;
		}

		SafeComRelease(pVSBlob);

		m_bInitialized = true;
		return true;
	}

	return false;
}

void CD3D11VertexShader::Bind()
{
	m_pDirect3D->GetD3D11DeviceContext()->VSSetShader(m_pD3D11VertexShader, nullptr, 0);
	m_pDirect3D->GetD3D11DeviceContext()->IASetInputLayout(m_pD3D11InputLayout);
}

void CD3D11VertexShader::Shutdown()
{
	if (m_bInitialized)
	{
		SafeComRelease(m_pD3D11InputLayout);
		SafeComRelease(m_pD3D11VertexShader);

		m_bInitialized = false;
	}
}
