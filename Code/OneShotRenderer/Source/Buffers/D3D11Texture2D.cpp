#include "pch.h"
#include "Buffers/D3D11Texture2D.h"
#include "Direct3D.h"
#include "Interfaces/IImageLoader.h"
#include "ImageLoaderFactory.h"
#include <minwinbase.h>

using namespace OneShotRenderer;

static std::string GetFileExtensionFromFilepath(std::string sFilePath)
{
	uint64_t ullLastSlashPos = sFilePath.find_last_of('/');
	if (ullLastSlashPos == std::string::npos)
	{
		ullLastSlashPos = sFilePath.find_last_of('\\');

		if (ullLastSlashPos == std::string::npos)
			return NULL;
	}

	ullLastSlashPos++;
	std::string sFileExtension;
	sFileExtension = sFilePath.substr(ullLastSlashPos, sFilePath.length() - ullLastSlashPos);
	size_t szFileExtenstionPos = sFileExtension.find_first_of('.');
	sFileExtension = sFileExtension.erase(0, szFileExtenstionPos + 1);

	return sFileExtension;
}

CD3D11Texture2D::CD3D11Texture2D(CDirect3D* pDirect3D) :
m_bInitialzied(false),
m_pD3D11Texture2D(nullptr),
m_pD3D11ShaderResourceView(nullptr),
m_pD3D11SamplerState(nullptr),
m_pDirect3D(pDirect3D)
{
}

CD3D11Texture2D::~CD3D11Texture2D()
{
}

bool CD3D11Texture2D::Initialize(std::string sTexturePath)
{
	if(!m_bInitialzied)
	{
		HRESULT hRes = S_OK;

		//Load image
		IImageLoader* pImgLoader = CImageLoaderFactory::GetInstance()->GetImageLoader(GetFileExtensionFromFilepath(sTexturePath));
		if (pImgLoader == nullptr)
			return false;

		IImage* pImage = pImgLoader->Load(sTexturePath);
		if (pImage == nullptr)
			return false;

		OSE_SAFE_DELETE(pImgLoader);

		D3D11_TEXTURE2D_DESC t2ddesc; ZeroMemory(&t2ddesc, sizeof(D3D11_TEXTURE2D_DESC));
		t2ddesc.Width = pImage->GetWidth();
		t2ddesc.Height = pImage->GetHeight();
		t2ddesc.ArraySize = 1;
		t2ddesc.MipLevels = 0;
		t2ddesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		t2ddesc.Usage = D3D11_USAGE_DEFAULT;
		t2ddesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		t2ddesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		t2ddesc.SampleDesc.Count = 1;
		t2ddesc.SampleDesc.Quality = 0;

		D3D11_SUBRESOURCE_DATA srd; ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));
		srd.pSysMem = pImage->GetPixelData();
		srd.SysMemPitch = pImage->GetRowPitchInBytes();

		hRes = m_pDirect3D->GetD3D11Device()->CreateTexture2D(&t2ddesc, NULL, &m_pD3D11Texture2D);
		if (FAILED(hRes))
			return false;

		m_pDirect3D->GetD3D11DeviceContext()->UpdateSubresource(m_pD3D11Texture2D, 0, NULL, pImage->GetPixelData(), pImage->GetRowPitchInBytes(), 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC srsvd; ZeroMemory(&srsvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srsvd.Format = t2ddesc.Format;
		srsvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srsvd.Texture2D.MostDetailedMip = 0;
		srsvd.Texture2D.MipLevels = -1;
		
		hRes = m_pDirect3D->GetD3D11Device()->CreateShaderResourceView(m_pD3D11Texture2D, &srsvd, &m_pD3D11ShaderResourceView);
		if (FAILED(hRes))
			return false;

		m_pDirect3D->GetD3D11DeviceContext()->GenerateMips(m_pD3D11ShaderResourceView);

		D3D11_SAMPLER_DESC samplerDesc; ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		hRes = m_pDirect3D->GetD3D11Device()->CreateSamplerState(&samplerDesc, &m_pD3D11SamplerState);
		if (FAILED(hRes))
		{
			return false;
		}

		OSE_SAFE_SHUTDOWN(pImage);

		m_bInitialzied = true;
		return true;
	}

	return false;
}

void CD3D11Texture2D::BindToPixelShader(uint8_t iSlot) const
{
	m_pDirect3D->GetD3D11DeviceContext()->PSSetShaderResources(iSlot, 1, &m_pD3D11ShaderResourceView);
	m_pDirect3D->GetD3D11DeviceContext()->PSSetSamplers(0, 1, &m_pD3D11SamplerState);
}

void CD3D11Texture2D::Shutdown()
{
	SafeComRelease(m_pD3D11ShaderResourceView);
	SafeComRelease(m_pD3D11Texture2D);
}