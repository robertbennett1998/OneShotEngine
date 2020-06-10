#include "pch.h"
#include "Direct3D.h"
#include <OneShotCore/Include/FileUtillities/ConfigurationFileParser.h>

using namespace OneShotRenderer;

CDirect3D::CDirect3D() :
	m_bInitialized(false),
	m_pD3D11Device(nullptr),
	m_pD3D11DeviceContext(nullptr),
	m_pDXGISwapChain(nullptr),
	m_pD3D11RenderTargetView(nullptr),
	m_fViewPortWidth(0.0f),
	m_fViewPortHeight(0.0f),
	m_pBoundCamera(nullptr)
{
}

CDirect3D::~CDirect3D()
{
}

bool CDirect3D::Initialize(HWND hWnd, ICamera* pCamera)
{
	if (!m_bInitialized)
	{
		m_hWnd = hWnd;
		m_pBoundCamera = pCamera;

		if (!CreateD3D11Device())
			return false;

		IDXGIDevice* pDxgiDevice = nullptr;
		if (FAILED(m_pD3D11Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDxgiDevice)))
			return false;

		DetectHighestMsaaSettings();
		
		if (!LoadSettings())
			return false;

		if (!CreateDXGISwapchain(pDxgiDevice))
			return false;

		SafeComRelease(pDxgiDevice);

		if (!CreateD3D11RenderTargetView())
			return false;

		if (!CreateD3D11DepthStencilBuffer())
			return false;

		if (!CreateD3D11DepthStencilState())
			return false;

		m_pD3D11DeviceContext->OMSetDepthStencilState(m_pD3D11DepthStencilState, 1);

		if (!CreateD3D11DepthStencilView())
			return false;

		m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pD3D11RenderTargetView, m_pD3D11DepthStencilView);

		RECT rect; ZeroMemory(&rect, sizeof(RECT));
		GetClientRect(m_hWnd, &rect);

		D3D11_VIEWPORT vp; ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
		m_fViewPortHeight = rect.bottom - rect.top;
		vp.Height = m_fViewPortHeight;
		m_fViewPortWidth = rect.right - rect.left;
		vp.Width = m_fViewPortWidth;
		vp.MaxDepth = 1.0f;
		vp.MinDepth = 0.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		m_pD3D11DeviceContext->RSSetViewports(1, &vp);

		m_bInitialized = true;
		return true;
	}

	return false;
}

void CDirect3D::PreDraw()
{
	OSE_LOG_INFO("Renderer", "Frame Start");
	static float rgba[4] = { 0, 1, 0, 1 };
	m_pD3D11DeviceContext->ClearRenderTargetView(m_pD3D11RenderTargetView, rgba);
	m_pD3D11DeviceContext->ClearDepthStencilView(m_pD3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CDirect3D::Draw()
{
	for (uint64_t i = 0; i < m_pRenderables.size(); i++)
	{
		OSE_LOG_INFO("Renderer", "Drawing %", m_pRenderables[i]->GetGeometry()->GetGeometryName());
		if (m_pRenderables[i]->GetShouldRender())
		{
			m_pRenderables[i]->Bind();
			m_pRenderables[i]->GetGeometry()->Bind();
			for (uint16_t j = 0; j < m_pRenderables[i]->GetGeometry()->GetSubGeometries().size(); j++)
			{
				m_pRenderables[i]->GetGeometry()->GetSubGeometries()[j]->Bind();
				m_pD3D11DeviceContext->DrawIndexed(m_pRenderables[i]->GetGeometry()->GetSubGeometries()[j]->ullNumberOfIndicies, 0, 0);
			}
		}
	}
}

void CDirect3D::PostDraw()
{
	m_pDXGISwapChain->Present(0, 0);
	OSE_LOG_INFO("Renderer", "Frame End\n-----------------------------");
}

void CDirect3D::Resize()
{
	if (!m_pD3D11Device || !m_pD3D11DeviceContext || !m_pDXGISwapChain)
		return;

	HRESULT hRes = S_OK;

	m_pD3D11DeviceContext->OMSetRenderTargets(0, 0, 0);
	SafeComRelease(m_pD3D11RenderTargetView);

	RECT rc; ZeroMemory(&rc, sizeof(RECT));
	GetClientRect(m_hWnd, &rc);
	hRes = m_pDXGISwapChain->ResizeBuffers(0, rc.right - rc.left, rc.bottom - rc.top, DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(hRes))
		throw;

	if (!CreateD3D11RenderTargetView())
		throw;

	SafeComRelease(m_pD3D11DepthStencilView);
	SafeComRelease(m_pD3D11DepthStencilState);
	SafeComRelease(m_pD3D11DepthStencilBuffer);

	if (!CreateD3D11DepthStencilBuffer())
		throw;

	if (!CreateD3D11DepthStencilState())
		throw;

	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pD3D11DepthStencilState, 1);

	if (!CreateD3D11DepthStencilView())
		throw;

	m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pD3D11RenderTargetView, m_pD3D11DepthStencilView);

	D3D11_VIEWPORT vp; ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	m_fViewPortHeight = rc.bottom - rc.top;
	vp.Height = m_fViewPortHeight;
	m_fViewPortWidth = rc.right - rc.left;
	vp.Width = m_fViewPortWidth;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_pD3D11DeviceContext->RSSetViewports(1, &vp);
}

void CDirect3D::Shutdown()
{
	SafeComRelease(m_pD3D11DepthStencilView);
	SafeComRelease(m_pD3D11DepthStencilState);
	SafeComRelease(m_pD3D11DepthStencilBuffer);
	SafeComRelease(m_pD3D11RenderTargetView);
	SafeComRelease(m_pDXGISwapChain);
	SafeComRelease(m_pD3D11DeviceContext);
	SafeComRelease(m_pD3D11Device);
}

bool CDirect3D::RegisterRenderable(IRenderable* pRenderable)
{
	if (m_pRenderables.size() > 0)
	{
		for (UINT i = 0; i < m_pRenderables.size(); i++)
		{
			if (m_pRenderables[i] == pRenderable)
				return false;
		}

		m_pRenderables.push_back(pRenderable);
		return true;
	}
	else
	{
		m_pRenderables.push_back(pRenderable);
		return true;
	}

	return false;
}

bool CDirect3D::UnregisterRenderable(IRenderable* pRenderable)
{
	if (m_pRenderables.size() > 0)
	{
		for (UINT i = 0; i < m_pRenderables.size(); i++)
		{
			if (m_pRenderables[i] == pRenderable)
			{
				m_pRenderables.erase(m_pRenderables.begin() + i);
				return true;
			}
		}

		return false;
	}

	return false;
}

IVertexShader* CDirect3D::CreateVertexShader()
{
	return OSE_NEW(CD3D11VertexShader(this));
}

IPixelShader* CDirect3D::CreatePixelShader()
{
	return OSE_NEW(CD3D11PixelShader(this));
}

IConstantBuffer* CDirect3D::CreateConstantBuffer()
{
	return OSE_NEW(CD3D11ConstantBuffer(this));
}

IVertexBuffer* CDirect3D::CreateVertexBuffer()
{
	return OSE_NEW(CD3D11VertexBuffer(this));
}

IIndexBuffer* CDirect3D::CreateIndexBuffer()
{
	return OSE_NEW(CD3D11IndexBuffer(this));
}

ITexture2D* CDirect3D::CreateTexture2D()
{
	return OSE_NEW(CD3D11Texture2D(this));
}

inline bool CDirect3D::LoadSettings()
{
	CGlobalVariables globalEnviroment; //TODO: Get global instance
	CConfigurationFileParser configFile(&globalEnviroment);
	if (!configFile.Load("/AppDataStore/Engine.config"))
		return false;
	
	m_uiMsaaCount = (UINT)configFile.Get<int>("Configuration.Renderer.MsaaCount");
	if (m_uiMsaaCount == 0 || m_uiMsaaCount > m_uiMsaaMaxCount)
	{
		m_uiMsaaCount = m_uiMsaaMaxCount;
		configFile.Set<int>("Configuration.Renderer.MsaaCount", m_uiMsaaCount);
	}

	m_uiMsaaQuality = GetMsaaQuality(m_uiMsaaCount);

	return true;
}

inline bool CDirect3D::CreateD3D11Device()
{
	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &m_pD3D11Device, NULL, &m_pD3D11DeviceContext)))
		return false;

	return true;
}

inline bool CDirect3D::CreateDXGISwapchain(IDXGIDevice* pDxgiDevice)
{
	if (pDxgiDevice == nullptr)
		return false;

	HRESULT hRes = S_OK;

	IDXGIAdapter* pDxgiAdapter = nullptr;
	hRes = pDxgiDevice->GetAdapter(&pDxgiAdapter);
	if (FAILED(hRes))
	{
		pDxgiDevice->Release();
		pDxgiDevice = nullptr;

		return false;
	}

	pDxgiDevice->Release();
	pDxgiDevice = nullptr;

	IDXGIFactory* pDxgiFactory = nullptr;
	hRes = pDxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDxgiFactory);
	if (FAILED(hRes))
	{
		pDxgiAdapter->Release();
		pDxgiAdapter = nullptr;

		return false;
	}

	RECT rect; ZeroMemory(&rect, sizeof(RECT));
	GetClientRect(m_hWnd, &rect);

	OSE_LOG_INFO("General", "MSAA settings detected Count: % - Quality: %", m_uiMsaaCount, m_uiMsaaQuality);

	DXGI_SWAP_CHAIN_DESC scd; ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferDesc.Format = m_BackBufferFormat;
	scd.BufferDesc.Height = rect.bottom - rect.top;
	scd.BufferDesc.Width = rect.right - rect.left;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferCount = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = m_hWnd;
	scd.SampleDesc.Count = m_uiMsaaCount;
	scd.SampleDesc.Quality = m_uiMsaaQuality;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = true;

	hRes = pDxgiFactory->CreateSwapChain(m_pD3D11Device, &scd, &m_pDXGISwapChain);
	if (FAILED(hRes))
	{
		SafeComRelease(pDxgiFactory);
		SafeComRelease(pDxgiDevice);
		return false;
	}

	SafeComRelease(pDxgiFactory);
	return true;
}

inline bool CDirect3D::CreateD3D11RenderTargetView()
{
	if (m_pDXGISwapChain == nullptr)
		return false;

	HRESULT hRes = S_OK;

	SafeComRelease(m_pD3D11RenderTargetView);

	ID3D11Texture2D* pD3D11BackBuffer = nullptr;
	hRes = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pD3D11BackBuffer);
	if (FAILED(hRes))
		return false;

	hRes = m_pD3D11Device->CreateRenderTargetView(pD3D11BackBuffer, nullptr, &m_pD3D11RenderTargetView);
	if (FAILED(hRes))
	{
		pD3D11BackBuffer->Release();
		pD3D11BackBuffer = nullptr;
		return false;
	}

	pD3D11BackBuffer->Release();
	pD3D11BackBuffer = nullptr;

	return true;
}

inline bool CDirect3D::CreateD3D11DepthStencilBuffer()
{
	if (m_pD3D11Device == nullptr)
		return false;

	HRESULT hRes = S_OK;

	RECT rc; ZeroMemory(&rc, sizeof(RECT));
	GetClientRect(m_hWnd, &rc);

	D3D11_TEXTURE2D_DESC dsbd; ZeroMemory(&dsbd, sizeof(D3D11_TEXTURE2D_DESC));
	dsbd.Width = rc.right - rc.left;
	dsbd.Height = rc.bottom - rc.top;
	dsbd.MipLevels = 1;
	dsbd.ArraySize = 1;
	dsbd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsbd.SampleDesc.Count = m_uiMsaaCount;
	dsbd.SampleDesc.Quality = m_uiMsaaQuality;
	dsbd.Usage = D3D11_USAGE_DEFAULT;
	dsbd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsbd.CPUAccessFlags = 0;
	dsbd.MiscFlags = 0;

	hRes = m_pD3D11Device->CreateTexture2D(&dsbd, nullptr, &m_pD3D11DepthStencilBuffer);

	if (FAILED(hRes))
		return false;

	return true;
}

inline bool CDirect3D::CreateD3D11DepthStencilState()
{
	if (m_pD3D11Device == nullptr)
		return false;

	HRESULT hRes = S_OK;

	D3D11_DEPTH_STENCIL_DESC dsd; ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	// Set up the description of the stencil state.
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;

	dsd.StencilEnable = true;
	dsd.StencilReadMask = 0xFF;
	dsd.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hRes = m_pD3D11Device->CreateDepthStencilState(&dsd, &m_pD3D11DepthStencilState);

	if (FAILED(hRes))
		return false;

	return true;
}

inline bool CDirect3D::CreateD3D11DepthStencilView()
{
	if (m_pD3D11Device == nullptr || m_pD3D11DepthStencilState == nullptr)
		return false;

	HRESULT hRes = S_OK;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd; ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvd.Texture2D.MipSlice = 0;

	hRes = m_pD3D11Device->CreateDepthStencilView(m_pD3D11DepthStencilBuffer, &dsvd, &m_pD3D11DepthStencilView);

	if (FAILED(hRes))
		return false;

	return true;
}
