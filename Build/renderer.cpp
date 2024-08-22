//=============================================================================
//
// レンダリング処理 [Renderer.cpp]
// Author : 
//
//=============================================================================
#include "Renderer.h"

//デバッグ用画面テキスト出力を有効にする
#define DEBUG_DISP_TEXTOUT
//シェーダーデバッグ設定を有効にする
//#define DEBUG_SHADER

Renderer::Renderer(Main*main)
{
	this->main = main;


	FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	m_D3DDevice = NULL;
	m_ImmediateContext = NULL;
	SwapChain = NULL;
	RenderTargetView = NULL;
	DepthStencilView = NULL;


	m_VertexShader = NULL;
	m_PixelShader = NULL;
	VertexLayout = NULL;


	//WorldBuffer = NULL;
	//ViewBuffer = NULL;
	//ProjectionBuffer = NULL;
	//MaterialBuffer = NULL;
	//LightBuffer = NULL;
	//FogBuffer = NULL;
	//FuchiBuffer = NULL;
	//CameraBuffer = NULL;

	DepthStateEnable;
	DepthStateDisable;

	BlendStateNone;
	BlendStateAlphaBlend;
	BlendStateAdd;
	BlendStateSubtract;
	BlendStateParam;


	RasterStateCullOff;
	RasterStateCullCW;
	RasterStateCullCCW;


	Material;
	Light;
	Fog;

	Fuchi;

	ClearColor[0] = 0.3f;	// 背景色
	ClearColor[1] = 0.3f;	// 背景色
	ClearColor[2] = 0.3f;	// 背景色
	ClearColor[3] = 1.0f;	// 背景色


}

Renderer::~Renderer()
{

}


//=============================================================================
// 投影行列の取得
//=============================================================================
XMMATRIX Renderer::GetProjectionMatrix(void)
{
	return projection;
}

//=============================================================================
// 投影行列のセット
//=============================================================================
void Renderer::SetProjectionMatrix(XMMATRIX* ProjectionMatrix)
{
	projection = *ProjectionMatrix;
	projection = XMMatrixTranspose(projection);

	m_ProjectionBuffer->SetToBuffer(m_ImmediateContext, &projection);
	//GetDeviceContext()->UpdateSubresource(ProjectionBuffer, 0, NULL, &projection, 0, 0);
}

//=============================================================================
// ビュー行列の取得
//=============================================================================
XMMATRIX Renderer::GetViewMatrix(void)
{
	return view;
}

//=============================================================================
// ビュー行列のセット
//=============================================================================
void Renderer::SetViewMatrix(XMMATRIX* ViewMatrix)
{
	view = *ViewMatrix;
	view = XMMatrixTranspose(view);

	m_ViewBuffer->SetToBuffer(GetDeviceContext(), &view);
	//GetDeviceContext()->UpdateSubresource(ViewBuffer, 0, NULL, &view, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ID3D11Device* Renderer::GetDevice( void )
{
	return m_D3DDevice;
}


ID3D11DeviceContext* Renderer::GetDeviceContext( void )
{
	return m_ImmediateContext;
}


void Renderer::SetDepthEnable( BOOL Enable )
{
	if( Enable )
		m_ImmediateContext->OMSetDepthStencilState( DepthStateEnable, NULL );
	else
		m_ImmediateContext->OMSetDepthStencilState( DepthStateDisable, NULL );

}


void Renderer::SetBlendState(BLEND_MODE bm)
{
	BlendStateParam = bm;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	switch (BlendStateParam)
	{
	case BLEND_MODE_NONE:
		m_ImmediateContext->OMSetBlendState(BlendStateNone, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ALPHABLEND:
		m_ImmediateContext->OMSetBlendState(BlendStateAlphaBlend, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ADD:
		m_ImmediateContext->OMSetBlendState(BlendStateAdd, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_SUBTRACT:
		m_ImmediateContext->OMSetBlendState(BlendStateSubtract, blendFactor, 0xffffffff);
		break;
	}
}

void Renderer::SetCullingMode(CULL_MODE cm)
{
	switch (cm)
	{
	case CULL_MODE_NONE:
		m_ImmediateContext->RSSetState(RasterStateCullOff);
		break;
	case CULL_MODE_FRONT:
		m_ImmediateContext->RSSetState(RasterStateCullCW);
		break;
	case CULL_MODE_BACK:
		m_ImmediateContext->RSSetState(RasterStateCullCCW);
		break;
	}
}

void Renderer::SetAlphaTestEnable(BOOL flag)
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	if (flag)
		blendDesc.AlphaToCoverageEnable = TRUE;
	else
		blendDesc.AlphaToCoverageEnable = FALSE;

	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	switch (BlendStateParam)
	{
	case BLEND_MODE_NONE:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_ALPHABLEND:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_ADD:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_SUBTRACT:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	}

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	if (blendState != NULL)
		blendState->Release();
}


void Renderer::SetWorldViewProjection2D( void )
{
	XMMATRIX world;
	world = XMMatrixTranspose(XMMatrixIdentity());
	m_WorldBuffer->SetToBuffer(m_ImmediateContext, &world);
	//GetDeviceContext()->UpdateSubresource(WorldBuffer, 0, NULL, &world, 0, 0);

	XMMATRIX view;
	view = XMMatrixTranspose(XMMatrixIdentity());
	m_ViewBuffer->SetToBuffer(m_ImmediateContext, &view);
	//GetDeviceContext()->UpdateSubresource(ViewBuffer, 0, NULL, &view, 0, 0);

	XMFLOAT2 screen = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);

	XMMATRIX worldViewProjection;
	worldViewProjection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	worldViewProjection = XMMatrixTranspose(worldViewProjection);

	m_ProjectionBuffer->SetToBuffer(m_ImmediateContext, &worldViewProjection);
	//GetDeviceContext()->UpdateSubresource(ProjectionBuffer, 0, NULL, &worldViewProjection, 0, 0);
}


void Renderer::SetWorldMatrix( XMMATRIX *WorldMatrix )
{
	XMMATRIX world;
	world = *WorldMatrix;
	world = XMMatrixTranspose(world);

	m_WorldBuffer->SetToBuffer(m_ImmediateContext,&world);
	//GetDeviceContext()->UpdateSubresource(WorldBuffer, 0, NULL, &world, 0, 0);
}

void Renderer::SetMaterial( MATERIAL material )
{
	Material.Diffuse = material.Diffuse;
	Material.Ambient = material.Ambient;
	Material.Specular = material.Specular;
	Material.Emission = material.Emission;
	Material.Shininess = material.Shininess;
	Material.noDiffuseTex = material.noDiffuseTex;
	Material.noNormalTex = material.noNormalTex;
	Material.phong = material.phong;

	m_MaterialBuffer->SetToBuffer(m_ImmediateContext, &Material);
	//GetDeviceContext()->UpdateSubresource( MaterialBuffer, 0, NULL, &Material, 0, 0 );
}

void Renderer::SetLightBuffer(void)
{
	m_LightBuffer->SetToBuffer(m_ImmediateContext,&Light);
	//GetDeviceContext()->UpdateSubresource(LightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetLightEnable(BOOL flag)
{
	// フラグを更新する
	Light.Enable = flag;

	SetLightBuffer();
}

void Renderer::SetLight(LightComponent* pLight)
{
	int index = pLight->GetIndex();
	Light.Position[index] = XMFLOAT4(pLight->GetPosition().x,pLight->GetPosition().y,pLight->GetPosition().z, 0.0f);
	Light.direction[index] = XMFLOAT4(pLight->GetDirection().x , pLight->GetDirection().y, pLight->GetDirection().z, 0.0f);
	Light.Diffuse[index] = pLight->GetDiffuse();
	Light.Ambient[index] = pLight->GetAmbient();
	Light.Flags[index].Type = pLight->GetType();
	Light.Flags[index].OnOff = pLight->GetEnable();
	Light.Attenuation[index].x = pLight->GetAttenuation();

	SetLightBuffer();
}

void Renderer::SetFogBuffer(void)
{
	m_FogBuffer->SetToBuffer(m_ImmediateContext, &Fog);
	//GetDeviceContext()->UpdateSubresource(FogBuffer, 0, NULL, &Fog, 0, 0);
}

void Renderer::SetFogEnable(BOOL flag)
{
	// フラグを更新する
	Fog.Enable = flag;

	SetFogBuffer();
}

void Renderer::SetFog(FOG* pFog)
{
	Fog.Fog.x = pFog->FogStart;
	Fog.Fog.y = pFog->FogEnd;
	Fog.FogColor = pFog->FogColor;

	SetFogBuffer();
}

void Renderer::SetFuchi(int flag)
{
	Fuchi.fuchi = flag;
	m_FuchiBuffer->SetToBuffer(m_ImmediateContext,&Fuchi);
	//GetDeviceContext()->UpdateSubresource(FuchiBuffer, 0, NULL, &Fuchi, 0, 0);
}


void Renderer::SetShaderCamera(XMFLOAT3 pos)
{
	XMFLOAT4 tmp = XMFLOAT4( pos.x, pos.y, pos.z, 0.0f );

	m_CameraBuffer->SetToBuffer(m_ImmediateContext, &tmp);
	//GetDeviceContext()->UpdateSubresource(CameraBuffer, 0, NULL, &tmp, 0, 0);
}


void Renderer::SetShadow(SHADOWMAP_CBUFFER* shadow)
{
	m_ShadowBuffer->SetToBuffer(m_ImmediateContext, shadow);
}



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Renderer::InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;
	XMFLOAT2 screen = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);

	// デバイス、スワップチェーン、コンテキスト生成
	DWORD deviceFlags = 0;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = bWindow;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // モード自動切り替え

	//デバッグ文字出力用設定
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;
	deviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

	hr = D3D11CreateDeviceAndSwapChain( NULL,
										D3D_DRIVER_TYPE_HARDWARE,
										NULL,
										deviceFlags,
										NULL,
										0,
										D3D11_SDK_VERSION,
										&sd,
										&SwapChain,
										&m_D3DDevice,
										&FeatureLevel,
										&m_ImmediateContext );
	if( FAILED( hr ) )
		return hr;

	//デバッグ文字出力用設定
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	hr = SwapChain->ResizeBuffers(0, SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE); // N.B. the GDI compatible flag
	if (FAILED(hr))
		return hr;
#endif

	// レンダーターゲットビュー生成、設定
	ID3D11Texture2D* pBackBuffer = NULL;
	SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	m_D3DDevice->CreateRenderTargetView( pBackBuffer, NULL, &RenderTargetView );
	pBackBuffer->Release();



	//ステンシル用テクスチャー作成
	depthTexture = NULL;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory( &td, sizeof(td) );
	td.Width			= sd.BufferDesc.Width;
	td.Height			= sd.BufferDesc.Height;
	td.MipLevels		= 1;
	td.ArraySize		= 1;
	td.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc		= sd.SampleDesc;
	td.Usage			= D3D11_USAGE_DEFAULT;
	td.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
    td.CPUAccessFlags	= 0;
    td.MiscFlags		= 0;
	m_D3DDevice->CreateTexture2D( &td, NULL, &depthTexture );

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof(dsvd) );
	dsvd.Format			= td.Format;
	dsvd.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags			= 0;
	m_D3DDevice->CreateDepthStencilView( depthTexture, &dsvd, &DepthStencilView );


	m_ImmediateContext->OMSetRenderTargets( 1, &RenderTargetView, DepthStencilView );


	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports( 1, &vp );

	this->defaultViewPort = vp;


	// ラスタライザステート作成
	D3D11_RASTERIZER_DESC rd; 
	ZeroMemory( &rd, sizeof( rd ) );
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE; 

	rd.DepthClipEnable = TRUE; 
	rd.MultisampleEnable = FALSE; 
	m_D3DDevice->CreateRasterizerState( &rd, &RasterStateCullOff);

	rd.CullMode = D3D11_CULL_FRONT;
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateCullCW);

	rd.CullMode = D3D11_CULL_BACK;
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateCullCCW);




	// ブレンドステートの作成
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState( &blendDesc, &BlendStateAlphaBlend );

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &BlendStateNone);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &BlendStateAdd);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &BlendStateSubtract);

	// アルファブレンド設定
	SetBlendState(BLEND_MODE_ALPHABLEND);




	// 深度ステンシルステート作成
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &DepthStateEnable );//深度有効ステート

	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	m_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &DepthStateDisable );//深度無効ステート

	// 深度ステンシルステート設定
	SetDepthEnable(TRUE);


	ID3D11SamplerState* samplerState[2];

	// Wrapサンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_D3DDevice->CreateSamplerState( &samplerDesc, &samplerState[0] );

	m_ImmediateContext->PSSetSamplers( 0, 1, &samplerState[0] );


	// Borderサンプラーステート設定
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState[1]);
	m_ImmediateContext->PSSetSamplers(1, 1, &samplerState[1]);



	
	// 頂点シェーダコンパイル・生成
 
	ID3DBlob* pVSBlob = this->CreateVSFile("shader.hlsl","VSmain", & m_VertexShader);

	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE( layout );

	m_D3DDevice->CreateInputLayout( layout,
		numElements,
		(pVSBlob)->GetBufferPointer(),
		(pVSBlob)->GetBufferSize(),
		&VertexLayout );

	(pVSBlob)->Release();

	// ピクセルシェーダコンパイル・生成
	this->CreatePSFile("shader.hlsl","PSmain", &m_PixelShader);


	// 定数バッファ生成
	InitConstantBuffers();

	SetShaderBuffersMode(DEFAULT_BF);


	// 入力レイアウト設定
	m_ImmediateContext->IASetInputLayout( VertexLayout );

	// シェーダ設定
	m_ImmediateContext->VSSetShader( m_VertexShader, NULL, 0 );
	m_ImmediateContext->PSSetShader( m_PixelShader, NULL, 0 );

	//ライト初期化
	ZeroMemory(&Light, sizeof(LIGHT_CBUFFER));
	Light.direction[0] = XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0f);
	Light.Diffuse[0] = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	Light.Ambient[0] = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	Light.Flags[0].Type = LIGHT_TYPE_DIRECTIONAL;
	SetLightBuffer();


	//マテリアル初期化
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	//ガウシアンブラー用バッファセット
	SetGausBuffer();

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void Renderer::UninitRenderer(void)
{
	// オブジェクト解放
	if (DepthStateEnable)		DepthStateEnable->Release();
	if (DepthStateDisable)	DepthStateDisable->Release();
	if (BlendStateNone)		BlendStateNone->Release();
	if (BlendStateAlphaBlend)	BlendStateAlphaBlend->Release();
	if (BlendStateAdd)		BlendStateAdd->Release();
	if (BlendStateSubtract)	BlendStateSubtract->Release();
	if (RasterStateCullOff)	RasterStateCullOff->Release();
	if (RasterStateCullCW)	RasterStateCullCW->Release();
	if (RasterStateCullCCW)	RasterStateCullCCW->Release();

	delete m_WorldBuffer;
	delete m_ViewBuffer;
	delete m_ProjectionBuffer;
	delete m_MaterialBuffer;
	delete m_LightBuffer;
	delete m_FogBuffer;
	delete m_FuchiBuffer;
	delete m_CameraBuffer;
	delete m_ShadowBuffer;
	delete m_GausBuffer;
	//m_WorldBuffer.ReleaseBuffer();
	//m_ViewBuffer.ReleaseBuffer();
	//m_ProjectionBuffer.ReleaseBuffer();
	//m_MaterialBuffer.ReleaseBuffer();
	//m_LightBuffer.ReleaseBuffer();
	//m_FogBuffer.ReleaseBuffer();
	//m_FuchiBuffer.ReleaseBuffer();
	//m_CameraBuffer.ReleaseBuffer();

	/*if (WorldBuffer)			WorldBuffer->Release();
	if (ViewBuffer)			ViewBuffer->Release();
	if (ProjectionBuffer)		ProjectionBuffer->Release();
	if (MaterialBuffer)		MaterialBuffer->Release();
	if (LightBuffer)			LightBuffer->Release();
	if (FogBuffer)			FogBuffer->Release();*/

	if (VertexLayout)			VertexLayout->Release();
	if (m_VertexShader)			m_VertexShader->Release();
	if (m_PixelShader)			m_PixelShader->Release();

	if (m_ImmediateContext)		m_ImmediateContext->ClearState();
	if (RenderTargetView)		RenderTargetView->Release();
	if (SwapChain)				SwapChain->Release();
	if (m_ImmediateContext)		m_ImmediateContext->Release();
	if (m_D3DDevice)				m_D3DDevice->Release();
}

//バッファの初期化処理
void Renderer::InitConstantBuffers(void) {

	m_WorldBuffer = new Buffer<XMMATRIX>(GetDevice());
	m_ViewBuffer = new Buffer<XMMATRIX>(GetDevice());
	m_ProjectionBuffer = new Buffer<XMMATRIX>(GetDevice());
	m_MaterialBuffer = new Buffer<MATERIAL_CBUFFER>(GetDevice());
	m_LightBuffer = new Buffer<LIGHT_CBUFFER>(GetDevice());
	m_FogBuffer = new Buffer<FOG_CBUFFER>(GetDevice());
	m_FuchiBuffer = new Buffer<FUCHI>(GetDevice());
	m_CameraBuffer = new Buffer<XMFLOAT4>(GetDevice());
	m_ShadowBuffer = new Buffer<SHADOWMAP_CBUFFER>(GetDevice());
	m_GausBuffer = new Buffer<GaussianCBuffer>(GetDevice());

}

void Renderer::SetShaderBuffersMode(ShaderBF_MODE bfMode) {
	switch (bfMode) {
	case DEFAULT_BF:
		{//バッファ再設定
				//ワールドマトリクス
			m_WorldBuffer->SetVS(m_ImmediateContext, 0);
			m_WorldBuffer->SetPS(m_ImmediateContext, 0);

			//ビューマトリクス
			m_ViewBuffer->SetVS(m_ImmediateContext, 1);
			m_ViewBuffer->SetPS(m_ImmediateContext, 1);

			//プロジェクションマトリクス
			m_ProjectionBuffer->SetVS(m_ImmediateContext, 2);
			m_ProjectionBuffer->SetPS(m_ImmediateContext, 2);

			//マテリアル情報
			m_MaterialBuffer->SetVS(m_ImmediateContext, 3);
			m_MaterialBuffer->SetPS(m_ImmediateContext, 3);

			//ライト情報
			m_LightBuffer->SetVS(m_ImmediateContext, 4);
			m_LightBuffer->SetPS(m_ImmediateContext, 4);

			//フォグ情報
			m_FogBuffer->SetVS(m_ImmediateContext, 5);
			m_FogBuffer->SetPS(m_ImmediateContext, 5);

			//縁取り
			m_FuchiBuffer->SetVS(m_ImmediateContext, 6);
			m_FuchiBuffer->SetPS(m_ImmediateContext, 6);

			//カメラ
			m_CameraBuffer->SetVS(m_ImmediateContext, 7);
			m_CameraBuffer->SetPS(m_ImmediateContext, 7);

			//影
			m_ShadowBuffer->SetVS(m_ImmediateContext, 8);
			m_ShadowBuffer->SetPS(m_ImmediateContext, 8);
			//影
			m_GausBuffer->SetVS(m_ImmediateContext, 9);
			m_GausBuffer->SetPS(m_ImmediateContext, 9);

		}
		break;
	case PARTICAL_BF:
		{

		}
		break;
	default:
		{

		}
		break;

	}
}

//=============================================================================
// バックバッファクリア
//=============================================================================
void Renderer::Clear(void)
{
	// バックバッファクリア
	m_ImmediateContext->ClearRenderTargetView( RenderTargetView, ClearColor );
	m_ImmediateContext->ClearDepthStencilView( DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void Renderer::SetGausBuffer(void)
{
	GaussianCBuffer gaus;
	ZeroMemory(&gaus, sizeof(gaus));
	float total = 0;
	constexpr float disperision = 40.0f;
	for (int i = 0; i < 8; i++) {
		float pos = 1.0f + 2.0f * (float)i;
		gaus.weight[i] = expf(-0.5f * pos * pos / disperision);
		if (i == 0) {
			total += gaus.weight[i];
		}
		else {
			total += 2.0f * gaus.weight[i];
		}
	}

	for (int i = 0; i < 8; i++) {
		gaus.weight[i] /= total;
	}


	m_GausBuffer->SetToBuffer(m_ImmediateContext, &gaus);
}

void Renderer::SetClearColor(float* color4)
{
	ClearColor[0] = color4[0];
	ClearColor[1] = color4[1];
	ClearColor[2] = color4[2];
	ClearColor[3] = color4[3];
}

ID3D11InputLayout** Renderer::GetVertexLayout(void)
{
	return &this->VertexLayout;
}

void Renderer::SetShaderDefault(void)
{
	m_ImmediateContext->VSSetShader(m_VertexShader,NULL,0);
	m_ImmediateContext->PSSetShader(m_PixelShader,NULL,0);
	// RSにビューポートを設定
	m_ImmediateContext->RSSetViewports(1, &defaultViewPort);
	// OMに描画ターゲット ビューと深度/ステンシル・ビューを設定
	m_ImmediateContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

}


IDXGISwapChain* Renderer::GetSwapChain(void)
{
	return this->SwapChain;
}


//=============================================================================
// プレゼント
//=============================================================================
void Renderer::Present(void)
{
	SwapChain->Present( 0, 0 );
}


//=============================================================================
// デバッグ用テキスト出力
//=============================================================================
void Renderer::DebugTextOut(char* text, int x, int y)
{
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	HRESULT hr;

	//バックバッファからサーフェスを取得する
	IDXGISurface1* pBackSurface = NULL;
	hr = SwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackSurface);

	if (SUCCEEDED(hr))
	{
		//取得したサーフェスからデバイスコンテキストを取得する
		HDC hdc;
		hr = pBackSurface->GetDC(FALSE, &hdc);

		if (SUCCEEDED(hr))
		{
			//文字色を白に変更
			SetTextColor(hdc, RGB(255, 255, 255));
			//背景を透明に変更
			SetBkMode(hdc, TRANSPARENT);

			XMFLOAT2 screen = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);

			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = SCREEN_WIDTH;
			rect.bottom = SCREEN_HEIGHT;

			//テキスト出力
			DrawText(hdc, text, (int)strlen(text), &rect, DT_LEFT);

			//デバイスコンテキストを解放する
			pBackSurface->ReleaseDC(NULL);
		}
		//サーフェスを解放する
		pBackSurface->Release();

		//レンダリングターゲットがリセットされるのでセットしなおす
		m_ImmediateContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
	}
#endif
}






//Create Vertex Shader ファイル
ID3DBlob* Renderer::CreateVSFile(const char* shaderName, char* fName, ID3D11VertexShader** VS){

	// 頂点シェーダコンパイル・生成
	ID3DBlob* pErrorBlob = NULL;;
	ID3DBlob* pVSBlob = NULL;//this->CreateVSFile("shader.hlsl", m_VertexShader);
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG) && defined(DEBUG_SHADER)
	shFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DX11CompileFromFile("shader.hlsl", NULL, NULL, fName, "vs_4_0", shFlag, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
	}

	m_D3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, VS);

	//HRESULT hr = S_OK;
	//ID3DBlob* pErrorBlob = NULL;
	//ID3DBlob* pVSBlob = NULL;
	//DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

	//hr = D3DX11CompileFromFile(shaderName, NULL, NULL, "VSmain", "vs_4_0", shFlag, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
	//if (FAILED(hr)){
	//	if (pErrorBlob){
	//		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
	//		pErrorBlob->Release();	//解放
	//	}
	//}
	//GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &VS);
	////解放してはいけない
	//pVSBlob->Release();

	return pVSBlob;
}

//Create P Shader ファイル
void Renderer::CreatePSFile(char* shaderName, char* fName, ID3D11PixelShader** PS) {

	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pPSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT hr = D3DX11CompileFromFile("shader.hlsl", NULL, NULL, fName, "ps_4_0", shFlag, 0, NULL, &pPSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	}

	m_D3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, PS);
	//解放
	pPSBlob->Release();

}
//Create Compute Shader ファイル
void Renderer::CreateCSFile(char* shaderName, char* fName, ID3D11ComputeShader** CS) {

	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* csBlob = NULL;

	hr = D3DX11CompileFromFile(shaderName, 0, 0, "CSFunc", "cs_5_0", 0, 0, 0, &csBlob, &pErrorBlob, 0);
	if (FAILED(hr)){
		if (pErrorBlob){
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
	}
	GetDevice()->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), NULL, CS);
	//解放
	csBlob->Release();
}