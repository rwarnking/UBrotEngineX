///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: direct3d.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../../header/graphics/direct3d.h"

using namespace DirectX;

namespace ubrot
{
namespace graphics
{

Direct3D::Direct3D()
{
	m_swapChain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_renderTargetView = nullptr;

	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_depthDisabledStencilState = nullptr;
	m_depthStencilView = nullptr;

	m_rasterState = nullptr;
	m_rasterStateWireframe = nullptr;
	m_rasterStateNoCulling = nullptr;

	m_alphaEnableBlendingState = nullptr;
	m_alphaDisableBlendingState = nullptr;
	m_alphaToCoverageBlendingState = nullptr;
}


Direct3D::Direct3D(const Direct3D& other)
{
}


Direct3D::~Direct3D()
{
}


void* Direct3D::operator new(size_t n) throw()
{
	return _aligned_malloc(n, 16);
}


void Direct3D::operator delete(void *p) throw()
{
	_aligned_free(p);
}


bool Direct3D::Initialize(
	HWND hwnd,
	const float screenDepth, const float screenNear,
	const int screenWidth, const int screenHeight,
	const bool vsync, const	bool fullscreen
)
{
	int error;
	unsigned int numModes = 0, numerator = 0, denominator = 0;
	float fieldOfView, screenAspect;
	size_t stringLength;
	HRESULT result;

	D3D11_VIEWPORT viewport;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ID3D11Texture2D* backBufferPtr;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_BLEND_DESC blendStateDescription;


	m_vsyncEnabled = vsync;

	// In the first steps, the refresh rate of the monitor is determined by getting and then
	// querying the adapter for the refresh rate numerator and denominator corresponding to
	// the desired window dimensions.

	// Create a factory which can be used to create other DXGI objects
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Construct an adapter for the graphics card currently in use
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Determine the primary monitor
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// TODO:
	// docs.microsoft.com/en-us/windows/desktop/api/dxgi/nf-dxgi-idxgioutput-getdisplaymodelist
	// TODO: write this such it is only 1 call?
	// Enumerate all display modes which satisfy the given parameters and
	// store their number numModes
	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		NULL
	);
	if (FAILED(result))
	{
		return false;
	}

	// Construct an array where all display modes can be stored
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Same function as above, this time we store the display modes in displayModeList
	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		displayModeList
	);
	if (FAILED(result))
	{
		return false;
	}

	// Iterate over all display modes and compare their values to the current screen width and
	// height. If a display mode has the same dimensions, the refresh rate numerator and
	// denominator are stored to determine the refresh rate later on.
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth &&
			displayModeList[i].Height == (unsigned int)screenHeight)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	// Query the graphics card description and store it in adapterDesc
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the memory size of the graphics card
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	// Copy the graphics card description
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Free all temporary variables
	delete[] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;


	// Initialize the swap chain, which is needed to swap between the back and front buffer,
	// by creating a description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set the swap chain up so that the back buffer has the same size as the window and
	// color values are stored using 32 bits.
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// If vSync is enabled, set the refresh rate numerator and denominator previously
	// read from the display mode.
	if (m_vsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	// If vSnyc is not enabled, the refresh rate is set to 0
	// (this may result in tearing: https://de.wikipedia.org/wiki/Screen_Tearing)
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the window handle and disable multi-sampling
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// TODO: better explanation or remove the comment?
	// Set the scan line ordering and scaling to unspecified and discard the back buffer
	// contents after presenting the rendered image.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Specify the DirectX version to be used (11)
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// TODO: let the user choose the graphics card (e.g. laptop with internal card)
	// TODO: use D3D11_CREATE_DEVICE_DEBUG at 4th position for debug output
	// Create the swap chain, Direct3D device and Direct3D device context using the swap chain
	// description. The latter serve as interfaces for all DirectX functions.
	result = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, m_swapChain.GetAddressOf(),
		m_device.GetAddressOf(), NULL, m_deviceContext.GetAddressOf()
	);
	if (FAILED(result))
	{
		return false;
	}


	// Set the swap chain's backbuffer
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}
	// Create the necessary render target view
	result = m_device->CreateRenderTargetView(
		backBufferPtr,
		NULL,
		m_renderTargetView.GetAddressOf()
	);
	if (FAILED(result))
	{
		return false;
	}

	// Clean up the back buffer pointer
	backBufferPtr->Release();
	backBufferPtr = nullptr;


	// Initialize the depth buffer description
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT; // TODO is this nessassary
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Since the depth buffer is just a texture, create a texture for the depth buffer and
	// the depth stencil buffer.
	result = m_device->CreateTexture2D(
		&depthBufferDesc,
		NULL,
		m_depthStencilBuffer.GetAddressOf()
	);
	if (FAILED(result))
	{
		return false;
	}


	// Initialize the depth stencil description, where the z-test is enabled.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Fix settings for front-facing and back-facing pixels
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(
		&depthStencilDesc,
		m_depthStencilState.GetAddressOf()
	);
	if (FAILED(result))
	{
		return false;
	}

	// Give the device context the created depth stencil state
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);


	// Initialize the second depth stencil description, which is used for 2D rendering, where
	// the z-buffer is disabled (set DepthEnable to false).
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(
		&depthDisabledStencilDesc,
		m_depthDisabledStencilState.GetAddressOf()
	);
	if (FAILED(result))
	{
		return false;
	}


	// In order to use the depth buffer as a depth stencil texture, we need a corresponding
	// view, which needs a description to be created.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_device->CreateDepthStencilView(
		m_depthStencilBuffer.Get(),
		&depthStencilViewDesc,
		m_depthStencilView.GetAddressOf()
	);
	if (FAILED(result))
	{
		return false;
	}


	// Set the render target to the depth stencil view
	m_deviceContext->OMSetRenderTargets(
		1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get()
	);


	// Initialie a blend state which enables alpha blending to render transparent objects
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = m_device->CreateBlendState(
		&blendStateDescription,
		m_alphaEnableBlendingState.GetAddressOf()
	);
	if (FAILED(result))
	{
		return false;
	}

	// Create a blend state description where alpha blending is disabled to easily switch
	// between the two different modes.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	result = m_device->CreateBlendState(
		&blendStateDescription,
		m_alphaDisableBlendingState.GetAddressOf()
	);
	if (FAILED(result))
	{
		return false;
	}


	// TODO: what does alpha coverage do and how does it work EXACTLY?!
	// Adjustment of the blend state description, where AlphaToCoverage is disabled, which
	// is important for fonts that use signed distance fields.
	blendStateDescription.AlphaToCoverageEnable = TRUE;
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;

	result = m_device->CreateBlendState(
		&blendStateDescription,
		m_alphaToCoverageBlendingState.GetAddressOf()
	);
	if (FAILED(result))
	{
		return false;
	}


	// In the following section, several raster states are created to allow different render-modes.

	// Normal mode
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterDesc, m_rasterState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	m_deviceContext->RSSetState(m_rasterState.Get());


	// Wireframe mode
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterDesc, m_rasterStateWireframe.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	// Mode with disabled back face culling
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterDesc, m_rasterStateNoCulling.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	// Set the viewport which is needed so that Direct3D can map the clip space coordinates
	// to render target space coordinates.
	// TODO: shitty explanation
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_deviceContext->RSSetViewports(1, &viewport);


	// Compute the projection martix which is used to map the 3D scence to the 2D viewport.
	// Store all matrices so that shaders can easily copy them on demand.
	fieldOfView = (float)XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;
	m_projectionMatrix = XMMatrixPerspectiveFovLH(
		fieldOfView, screenAspect,
		screenNear, screenDepth
	);

	// Compute the world matrix which maps an objects vertices to the 3D scene. To start with,
	// it is initialized with the identity matrix since rotation, scale and translation of
	// the object are unknown at this point.
	m_worldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix used for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH(
		(float)screenWidth, (float)screenHeight,
		screenNear, screenDepth
	);

	return true;
}


void Direct3D::Shutdown()
{
	//#include <d3d11sdklayers.h> // TODO: remove
	//ID3D11Debug *d3dDebug = nullptr;
	//m_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	//d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);

	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	return;
}


void Direct3D::BeginScene(
	const float red, const float green,
	const float blue, const float alpha
)
{
	float color[4];


	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void Direct3D::EndScene() const
{
	// Present the rendered image, either with or without vSnyc depending on the settings.
	if (m_vsyncEnabled)
	{
		m_swapChain->Present(1, 0);
	}
	else
	{
		m_swapChain->Present(0, 0);
	}

	return;
}


void Direct3D::RefreshMatrix(
	const float screenNear, const float screenDepth,
	const int screenWidth, const int screenHeight
)
{
	//int screenWidth = 1920;
	//int screenHeight = 1080;
	float fieldOfView = (float)XM_PI / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	m_projectionMatrix = XMMatrixPerspectiveFovLH(
		fieldOfView, screenAspect,
		screenNear, screenDepth
	);

	m_worldMatrix = XMMatrixIdentity();

	//m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight,
	// screenNear, screenDepth);
}


ID3D11Device* Direct3D::GetDevice() const
{
	return m_device.Get();
}


ID3D11DeviceContext* Direct3D::GetDeviceContext() const
{
	return m_deviceContext.Get();
}


IDXGISwapChain* Direct3D::GetSwapChain() const
{
	return m_swapChain.Get();
}


void Direct3D::GetProjectionMatrix(XMMATRIX &projectionMatrix) const
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void Direct3D::GetWorldMatrix(XMMATRIX &worldMatrix) const
{
	worldMatrix = m_worldMatrix;
	return;
}


void Direct3D::GetOrthoMatrix(XMMATRIX &orthoMatrix) const
{
	orthoMatrix = m_orthoMatrix;
	return;
}


void Direct3D::GetVideoCardInfo(char *cardName, int &memory) const
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}


ID3D11DepthStencilView* Direct3D::GetDepthStencilView() const
{
	return m_depthStencilView.Get();
}


void Direct3D::SetBackBufferRenderTarget()
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView.Get());

	return;
}


void Direct3D::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
	return;
}


void Direct3D::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState.Get(), 1);
	return;
}


void Direct3D::TurnOnAlphaBlending()
{
	float blendFactor[4];


	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState.Get(), blendFactor, 0xffffffff);

	return;
}


void Direct3D::TurnOffAlphaBlending()
{
	float blendFactor[4];


	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState.Get(), blendFactor, 0xffffffff);

	return;
}


void Direct3D::TurnOnAlphaBlendingCoverage()
{
	float blend_factor[4];


	blend_factor[0] = 0.0f;
	blend_factor[1] = 0.0f;
	blend_factor[2] = 0.0f;
	blend_factor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(m_alphaToCoverageBlendingState.Get(), blend_factor, 0xffffffff);

	return;
}


void Direct3D::TurnOnCulling()
{
	m_deviceContext->RSSetState(m_rasterState.Get());

	return;
}


void Direct3D::TurnOffCulling()
{
	m_deviceContext->RSSetState(m_rasterStateNoCulling.Get());

	return;
}


void Direct3D::TurnOnWireframe()
{
	m_deviceContext->RSSetState(m_rasterStateWireframe.Get());
}


void Direct3D::TurnOffWireframe()
{
	m_deviceContext->RSSetState(m_rasterState.Get());
}


void Direct3D::ResetViewport()
{
	// TODO: !important! 1 does not work in release, but 2 does
	m_deviceContext->RSSetViewports(2, &m_viewport);

	return;
}

}
}
