#include "pch.h"
#include "Shaders/D3D11PixelShader.h"
#include "Direct3D.h"

using namespace OneShotRenderer;

static std::wstring StringToWideString(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

CD3D11PixelShader::CD3D11PixelShader(CDirect3D * pDirect3D) :
m_bInitialized(false),
m_pDirect3D(pDirect3D),
m_pD3D11PixelShader(nullptr)
{
}

CD3D11PixelShader::~CD3D11PixelShader()
{
}

bool CD3D11PixelShader::Initialize(std::string sPSFile, std::string sPSEntryPoint)
{
	if (!m_bInitialized)
	{
		HRESULT hRes = S_OK;

		ID3D10Blob* pErrBlob = nullptr;
		ID3D10Blob* pPSBlob = nullptr;
		hRes = D3DCompileFromFile(StringToWideString(sPSFile).c_str(), NULL, NULL, sPSEntryPoint.c_str(), "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pPSBlob, &pErrBlob);
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

		hRes = m_pDirect3D->GetD3D11Device()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pD3D11PixelShader);
		if (FAILED(hRes))
		{
			SafeComRelease(pPSBlob);
			return false;
		}

		SafeComRelease(pPSBlob);

		m_bInitialized = true;
		return true;
	}

	return false;
}

void CD3D11PixelShader::Bind()
{
	m_pDirect3D->GetD3D11DeviceContext()->PSSetShader(m_pD3D11PixelShader, nullptr, 0);
}

void CD3D11PixelShader::Shutdown()
{
	if (m_bInitialized)
	{
		SafeComRelease(m_pD3D11PixelShader);

		m_bInitialized = false;
	}
}