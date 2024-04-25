//=============================================================================
//
// レンダリング処理 [renderer.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include"model.h"
#include"camera.h"
#include"light.h"
#include"player.h"
#include"camera.h"
#include"sprite.h"

//デバッグ用画面テキスト出力を有効にする
#define DEBUG_DISP_TEXTOUT
//シェーダーデバッグ設定を有効にする
#define DEBUG_SHADER

#define ShadowMapQuarity	(1024.0f)//16348まで
#define ShadowMapHW			(1024.0f)
#define KR		(0.0025f)		//レイリー散乱定数
#define KR_4PI	(KR*4.0f*XM_PI)
#define KM		(0.0010f)		//ミー散乱定数
#define KM_4PI	(KM*4.0f*XM_PI)
#define E_SUN	(20.0f)

//*********************************************************
// 構造体
//*********************************************************

// マテリアル用定数バッファ構造体
struct MATERIAL_CBUFFER
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	int			noTexSampling;
	float		Dummy[2];				// 16byte境界用
};

// ライト用フラグ構造体
struct LIGHTFLAGS
{
	int			Type;		//ライトタイプ（enum LIGHT_TYPE）
	int         OnOff;		//ライトのオンorオフスイッチ
	int			Dummy[2];
};

// ライト用定数バッファ構造体
struct LIGHT_CBUFFER
{
	XMFLOAT4	Direction[LIGHT_MAX];	// ライトの方向
	XMFLOAT4	Position[LIGHT_MAX];	// ライトの位置
	XMFLOAT4	Diffuse[LIGHT_MAX];		// 拡散光の色
	XMFLOAT4	Ambient[LIGHT_MAX];		// 環境光の色
	XMFLOAT4	Attenuation[LIGHT_MAX];	// 減衰率
	LIGHTFLAGS	Flags[LIGHT_MAX];		// ライト種別
	int			Enable;					// ライティング有効・無効フラグ
	int			Dummy[3];				// 16byte境界用
};

// フォグ用定数バッファ構造体
struct FOG_CBUFFER
{
	XMFLOAT4	Fog;					// フォグ量
	XMFLOAT4	FogColor;				// フォグの色
	int			Enable;					// フォグ有効・無効フラグ
	float		Dummy[3];				// 16byte境界用
};

// 影用定数バッファ構造体
struct SHADOWMAP_CBUFFER
{	

	XMFLOAT4X4 SMWorldViewProj; // ワールド×ビュー×透視変換行列(シャドウマップ用)

	int			Enable;
	int			Dummy[3];				
};

// 縁取り用バッファ
struct FUCHI
{
	int			fuchi;
	int			fill[3];
};

//大気散乱用バッファ
struct SCAT_CBUFFER
{

	XMFLOAT3 Camera;			//カメラのポジション
	float  CameraHeight;		//カメラの高さ
	float  CameraHeight2;		//カメラの高さの2乗
	XMFLOAT3 LightPos;
	XMFLOAT3 InvWaveLength;		//RGBの1/pow(wavelngth,4)
	float OuterRadius;			//大気の半径
	float OuterRadius2;			//大気の半径^2
	float InnerRadius;			//惑星の半径
	float InnerRadius2;			//惑星の半径^2
	float KrESun;				//Kr*Esun
	float KmESun;				//Km*Esun
	float Kr4PI;				//Kr*4*PI
	float Km4PI;				//Km*4*PI
	float Scale;				//1/(OuterRadius-InnerRadius)
	float ScaleDepth;
	float ScaleOverScaleDepth;	//Scale/ScaleDepth
	float Dummy[2];


};



struct GaussianCBuffer {
	float weight[8];
	XMFLOAT2 texsize;
	float Dummy[2];
};




//// 定数バッファのデータ定義
//struct cbCBuffer {
//	XMFLOAT4X4 World;		// ワールド変換行列
//	XMFLOAT4X4 View;		// ビュー変換行列
//	XMFLOAT4X4 Projection;	// 透視変換行列
//	XMFLOAT4X4 SMWorldViewProj; // ワールド×ビュー×透視変換行列(シャドウマップ用)
//	XMFLOAT4   Diffuse;		// ディフューズ色
//	XMFLOAT3   Light;		// 光源座標(ワールド座標系)
//	FLOAT      dummy;		// ダミー
//};
//
//// 定数バッファのデータ
//struct cbCBuffer g_cbCBuffer;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static void SetLightBuffer(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static D3D_FEATURE_LEVEL       g_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

static ID3D11Device*           g_D3DDevice = NULL;
static ID3D11DeviceContext*    g_ImmediateContext = NULL;
static IDXGISwapChain*         g_SwapChain = NULL;
static ID3D11RenderTargetView* g_RenderTargetView = NULL;
static ID3D11DepthStencilView* g_DepthStencilView = NULL;

static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報


static ID3D11VertexShader*		g_VertexShader = NULL;
static ID3D11PixelShader*		g_PixelShader = NULL;
static ID3D11VertexShader*		g_VertexShaderSky = NULL;
static ID3D11PixelShader*		g_PixelShaderSky = NULL;
static ID3D11PixelShader*		g_PixelShaderParticle = NULL;
static ID3D11PixelShader*		g_Xpass = NULL;
static ID3D11PixelShader*		g_Ypass = NULL;
static ID3D11GeometryShader*	g_GeometryShader = NULL;
static ID3D11InputLayout*		g_VertexLayout = NULL;
static ID3D11Buffer*			g_WorldBuffer = NULL;
static ID3D11Buffer*			g_ViewBuffer = NULL;
static ID3D11Buffer*			g_ProjectionBuffer = NULL;
static ID3D11Buffer*			g_MaterialBuffer = NULL;
static ID3D11Buffer*			g_LightBuffer = NULL;
static ID3D11Buffer*			g_FogBuffer = NULL;
static ID3D11Buffer*			g_ShadowBuffer = NULL;
static ID3D11Buffer*			g_FuchiBuffer = NULL;
static ID3D11Buffer*			g_CameraBuffer = NULL;
static ID3D11Buffer*			g_SMViewBuffer = NULL;
static ID3D11Buffer*			g_SMProjectionBuffer = NULL;
static ID3D11Buffer*			g_ScatBuffer = NULL;
static ID3D11Buffer*			g_GausBuffer = NULL;

static ID3D11DepthStencilState* g_DepthStateEnable=NULL;
static ID3D11DepthStencilState* g_DepthStateDisable=NULL;
static ID3D11DepthStencilState* g_pDepthStencilState = NULL;	// 深度/ステンシル・ステート・オブジェクト


static ID3D11BlendState*		g_BlendStateNone;
static ID3D11BlendState*		g_BlendStateAlphaBlend;
static ID3D11BlendState*		g_BlendStateAdd;
static ID3D11BlendState*		g_BlendStateSubtract;
static BLEND_MODE				g_BlendStateParam;


static ID3D11RasterizerState*	g_RasterStateCullOff;
static ID3D11RasterizerState*	g_RasterStateCullCW;
static ID3D11RasterizerState*	g_RasterStateCullCCW;


static MATERIAL_CBUFFER	g_Material;
static LIGHT_CBUFFER	g_Light;
static FOG_CBUFFER		g_Fog;
static SHADOWMAP_CBUFFER	g_ShadowMap;
static SCAT_CBUFFER	g_Scat;
static GaussianCBuffer	g_Gaus;

// シャドウ マップ描画用
static ID3D11VertexShader* g_pVertexShaderShadow = NULL;		// 頂点シェーダ
static ID3D11PixelShader* g_pPixelShaderShadow = NULL;		// 頂点シェーダ
static ID3D11RasterizerState* g_pRasterizerStateShadow = NULL;	// ラスタライザ・ステート・オブジェクト
static ID3D11SamplerState* g_pTextureSampler[2] = { NULL, NULL };		// サンプラー
// シャドウ・マッピング用
static bool g_bShadowMappingMode = TRUE;
static ID3D11Texture2D* g_pShadowMap = NULL;			// シャドウ・マップ
static ID3D11DepthStencilView* g_pShadowMapDSView = NULL;	// 深度/ステンシル・ビュー
static ID3D11ShaderResourceView* g_pShadowMapSRView = NULL;	// シェーダ・リソース・ビュー
static D3D11_VIEWPORT            g_ViewPortShadowMap[1];       // ビューポート

static ID3D11Texture2D* g_ShadowMapingTexture = NULL;			
static ID3D11RenderTargetView* g_RenderTargetShadow = NULL;
static ID3D11ShaderResourceView* g_pShadowMapSRViewBA = NULL;	// シェーダ・リソース・ビュー

static ID3D11Texture2D* g_ShadowMapingTextureGausX = NULL;			
static ID3D11RenderTargetView* g_RenderTargetShadowGausX = NULL;
static ID3D11ShaderResourceView* g_pShadowMapSRViewGausX = NULL;	// シェーダ・リソース・ビュー

static ID3D11Texture2D* g_ShadowMapingTextureGausY = NULL;			
static ID3D11RenderTargetView* g_RenderTargetShadowGausY = NULL;
static ID3D11ShaderResourceView* g_pShadowMapSRViewGausY = NULL;	// シェーダ・リソース・ビュー


static FUCHI			g_Fuchi;

static float g_ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// 背景色

static XMFLOAT3 g_vLightPos(300.0f, 300.0f, -300.0f);   // 光源座標(ワールド座標系)


static ID3D11SamplerState* samplerState[2] = { NULL, NULL };


ID3D11Device* GetDevice( void )
{
	return g_D3DDevice;
}


ID3D11DeviceContext* GetDeviceContext( void )
{
	return g_ImmediateContext;
}


void SetDepthEnable( bool Enable )
{
	if( Enable )
		g_ImmediateContext->OMSetDepthStencilState( g_DepthStateEnable, NULL );
	else
		g_ImmediateContext->OMSetDepthStencilState( g_DepthStateDisable, NULL );

}


void SetBlendState(BLEND_MODE bm)
{
	g_BlendStateParam = bm;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	switch (g_BlendStateParam)
	{
	case BLEND_MODE_NONE:
		g_ImmediateContext->OMSetBlendState(g_BlendStateNone, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ALPHABLEND:
		g_ImmediateContext->OMSetBlendState(g_BlendStateAlphaBlend, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ADD:
		g_ImmediateContext->OMSetBlendState(g_BlendStateAdd, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_SUBTRACT:
		g_ImmediateContext->OMSetBlendState(g_BlendStateSubtract, blendFactor, 0xffffffff);
		break;
	}
}

void SetCullingMode(CULL_MODE cm)
{
	switch (cm)
	{
	case CULL_MODE_NONE:
		g_ImmediateContext->RSSetState(g_RasterStateCullOff);
		break;
	case CULL_MODE_FRONT:
		g_ImmediateContext->RSSetState(g_RasterStateCullCW);
		break;
	case CULL_MODE_BACK:
		g_ImmediateContext->RSSetState(g_RasterStateCullCCW);
		break;
	}
}

void SetAlphaTestEnable(BOOL flag)
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	if (flag)
		blendDesc.AlphaToCoverageEnable = TRUE;
	else
		blendDesc.AlphaToCoverageEnable = FALSE;

	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	switch (g_BlendStateParam)
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
	g_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	g_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	if (blendState != NULL)
		blendState->Release();
}


void SetWorldViewProjection2D( void )
{
	XMMATRIX world;
	world = XMMatrixTranspose(XMMatrixIdentity());
	GetDeviceContext()->UpdateSubresource(g_WorldBuffer, 0, NULL, &world, 0, 0);

	XMMATRIX view;
	view = XMMatrixTranspose(XMMatrixIdentity());
	GetDeviceContext()->UpdateSubresource(g_ViewBuffer, 0, NULL, &view, 0, 0);

	XMMATRIX worldViewProjection;
	worldViewProjection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	worldViewProjection = XMMatrixTranspose(worldViewProjection);
	GetDeviceContext()->UpdateSubresource(g_ProjectionBuffer, 0, NULL, &worldViewProjection, 0, 0);
}


void SetWorldMatrix( XMMATRIX *WorldMatrix )
{
	XMMATRIX world;
	world = *WorldMatrix;

	GetDeviceContext()->UpdateSubresource(g_WorldBuffer, 0, NULL, &XMMatrixTranspose(world), 0, 0);


}

void SetViewMatrix(XMMATRIX *ViewMatrix )
{
	XMMATRIX view;
	view = *ViewMatrix;
	view = XMMatrixTranspose(view);

	GetDeviceContext()->UpdateSubresource(g_ViewBuffer, 0, NULL, &view, 0, 0);
}


void SetProjectionMatrix( XMMATRIX *ProjectionMatrix )
{
	XMMATRIX projection;
	projection = *ProjectionMatrix;
	projection = XMMatrixTranspose(projection);

	GetDeviceContext()->UpdateSubresource(g_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}


void SetSMViewMatrix(XMMATRIX *ViewMatrix )
{
	XMMATRIX view;
	view = *ViewMatrix;
	view = XMMatrixTranspose(view);

	GetDeviceContext()->UpdateSubresource(g_SMViewBuffer, 0, NULL, &view, 0, 0);
}


void SetSMProjectionMatrix( XMMATRIX *ProjectionMatrix )
{
	XMMATRIX projection;
	projection = *ProjectionMatrix;
	projection = XMMatrixTranspose(projection);

	GetDeviceContext()->UpdateSubresource(g_SMProjectionBuffer, 0, NULL, &projection, 0, 0);
}

void SetMaterial( MATERIAL material )
{
	g_Material.Diffuse = material.Diffuse;
	g_Material.Ambient = material.Ambient;
	g_Material.Specular = material.Specular;
	g_Material.Emission = material.Emission;
	g_Material.Shininess = material.Shininess;
	g_Material.noTexSampling = material.noTexSampling;

	GetDeviceContext()->UpdateSubresource( g_MaterialBuffer, 0, NULL, &g_Material, 0, 0 );
}

void SetLightBuffer(void)
{
	GetDeviceContext()->UpdateSubresource(g_LightBuffer, 0, NULL, &g_Light, 0, 0);
}

void SetLightEnable(BOOL flag)
{
	// フラグを更新する
	g_Light.Enable = flag;

	SetLightBuffer();
}

void SetLight(int index, LIGHT* pLight)
{
	g_Light.Position[index] = XMFLOAT4(pLight->Position.x, pLight->Position.y, pLight->Position.z, 0.0f);
	g_Light.Direction[index] = XMFLOAT4(pLight->Direction.x, pLight->Direction.y, pLight->Direction.z, 0.0f);
	g_Light.Diffuse[index] = pLight->Diffuse;
	g_Light.Ambient[index] = pLight->Ambient;
	g_Light.Flags[index].Type = pLight->Type;
	g_Light.Flags[index].OnOff = pLight->Enable;
	g_Light.Attenuation[index].x = pLight->Attenuation;

	SetLightBuffer();
}

void SetFogBuffer(void)
{
	GetDeviceContext()->UpdateSubresource(g_FogBuffer, 0, NULL, &g_Fog, 0, 0);
}

void SetShadowMapBuffer(void)
{
	GetDeviceContext()->UpdateSubresource(g_ShadowBuffer, 0, NULL, &g_ShadowMap, 0, 0);
}

void SetFogEnable(BOOL flag)
{
	// フラグを更新する
	g_Fog.Enable = flag;

	SetFogBuffer();
}

void SetFog(FOG* pFog)
{
	g_Fog.Fog.x = pFog->FogStart;
	g_Fog.Fog.y = pFog->FogEnd;
	g_Fog.FogColor = pFog->FogColor;

	SetFogBuffer();
}


//void SetShadowEnable(BOOL flag)
//{
//	// フラグを更新する
//	g_Shadow.Enable = flag;
//
//	SetShadowBuffer();
//}

//void SetShadow(SHADOW* pShadow)
//{
//	g_Shadow.ShadowColor = pShadow->ShadowColor;
//
//	SetShadowBuffer();
//}

void SetFuchi(int flag)
{
	g_Fuchi.fuchi = flag;
	GetDeviceContext()->UpdateSubresource(g_FuchiBuffer, 0, NULL, &g_Fuchi, 0, 0);
}


void SetShaderCamera(XMFLOAT3 pos)
{
	XMFLOAT4 tmp = XMFLOAT4( pos.x, pos.y, pos.z, 0.0f );

	GetDeviceContext()->UpdateSubresource(g_CameraBuffer, 0, NULL, &tmp, 0, 0);
}





void SetGausBuffer(void)
{
	GetDeviceContext()->UpdateSubresource(g_GausBuffer, 0, NULL, &g_Gaus, 0, 0);

}

void SetScatBuffer(void)
{
	GetDeviceContext()->UpdateSubresource(g_ScatBuffer, 0, NULL, &g_Scat, 0, 0);

}


void SetScat(void)
{
	CAMERA* camera = GetCamera();
	SUN sun = GetSun();
	
									//r		g		b
	XMFLOAT3 wavelength = XMFLOAT3(0.650f, 0.570f, 0.475f);
	XMFLOAT3 wavelength4 = XMFLOAT3(powf(wavelength.x,4),powf(wavelength.y,4),powf(wavelength.z,4));
	g_Scat.Camera				=camera->pos;					//カメラのポジション
	g_Scat.CameraHeight			= sqrtf(camera->pos.x*camera->pos.x+camera->pos.y*camera->pos.y+camera->pos.z*camera->pos.z);			//カメラの高さ
	g_Scat.CameraHeight2		= camera->pos.x * camera->pos.x + camera->pos.y * camera->pos.y + camera->pos.z * camera->pos.z;			//カメラの高さの2乗
	XMStoreFloat3(&g_Scat.LightPos, sun.Direction);
	g_Scat.InvWaveLength		=XMFLOAT3(1/wavelength4.x,1/wavelength4.y,1/wavelength4.z);			//RGBの1/pow(wavelngth,4)
	g_Scat.OuterRadius			=1000.25f;			//大気の半径
	g_Scat.OuterRadius2			=powf(g_Scat.OuterRadius,2.0f);			//大気の半径^2
	g_Scat.InnerRadius			=1000.0f;			//惑星の半径
	g_Scat.InnerRadius2			= powf(g_Scat.InnerRadius, 2.0f);			//惑星の半径^2
	g_Scat.KrESun				= KR * E_SUN;					//Kr*Esun
	g_Scat.KmESun				= KM * E_SUN;					//Km*Esun
	g_Scat.Kr4PI				=KR_4PI;					//Kr*4*PI
	g_Scat.Km4PI				=KM_4PI;					//Km*4*PI
	g_Scat.Scale				=1/(g_Scat.OuterRadius- g_Scat.InnerRadius);//1/(OuterRadius-InnerRadius)
	g_Scat.ScaleDepth			= 0.25f;
	g_Scat.ScaleOverScaleDepth	= g_Scat.Scale/ g_Scat.ScaleDepth;	//Scale/ScaleDepth

	
	SetScatBuffer();
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;






	float total = 0;
	constexpr float disperision = 10.0f;
	for (int i = 0; i < 8; i++) {
		float pos = 1.0f + 2.0f * (float)i;
		g_Gaus.weight[i] = expf(-0.5f * pos * pos / disperision);
		if (i == 0) {
			total += g_Gaus.weight[i];
		}
		else {
			total += 2.0f * g_Gaus.weight[i];
		}
	}

	for (int i = 0; i < 8; i++) {
		g_Gaus.weight[i] /= total;
	}
	g_Gaus.texsize.x = ShadowMapHW;
	g_Gaus.texsize.y = ShadowMapHW;


	// デバイス、スワップチェーン、コンテキスト生成
	DWORD deviceFlags = 0;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width  = SCREEN_WIDTH;
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
										&g_SwapChain,
										&g_D3DDevice,
										&g_FeatureLevel,
										&g_ImmediateContext );
	if( FAILED( hr ) )
		return hr;

	//デバッグ文字出力用設定
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	hr = g_SwapChain->ResizeBuffers(0, SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE); // N.B. the GDI compatible flag
	if (FAILED(hr))
		return hr;
#endif

	// レンダーターゲットビュー生成、設定
	ID3D11Texture2D* pBackBuffer = NULL;
	g_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	g_D3DDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_RenderTargetView );
	pBackBuffer->Release();

	//// シャドウマッピング用レンダーターゲットビュー生成、設定
	//g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&g_ShadowMapingTexture);
	//g_D3DDevice->CreateRenderTargetView(g_ShadowMapingTexture, NULL, &g_RenderTargetShadow );

	// シャドウマッピング用レンダーターゲットビュー生成、設定
	g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&g_pShadowMap);
	g_D3DDevice->CreateRenderTargetView(g_pShadowMap, NULL, &g_RenderTargetShadow );



	//ステンシル用テクスチャー作成
	ID3D11Texture2D* depthTexture = NULL;
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
	g_D3DDevice->CreateTexture2D( &td, NULL, &depthTexture );

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof(dsvd) );
	dsvd.Format			= td.Format;
	dsvd.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags			= 0;
	g_D3DDevice->CreateDepthStencilView( depthTexture, &dsvd, &g_DepthStencilView );


	g_ImmediateContext->OMSetRenderTargets( 1, &g_RenderTargetView, g_DepthStencilView );


	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_ImmediateContext->RSSetViewports( 1, &vp );

	// **********************************************************
	 //シャドウ・マップの作成
	hr = InitShadowMap();
	if (FAILED(hr))
		return hr;





	// ラスタライザステート作成
	D3D11_RASTERIZER_DESC rd; 
	ZeroMemory( &rd, sizeof( rd ) );
	rd.FillMode = D3D11_FILL_SOLID;
	//rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_NONE; 
	rd.DepthClipEnable = TRUE; 
	rd.MultisampleEnable = FALSE; 
	g_D3DDevice->CreateRasterizerState( &rd, &g_RasterStateCullOff);

	rd.CullMode = D3D11_CULL_FRONT;
	g_D3DDevice->CreateRasterizerState(&rd, &g_RasterStateCullCW);

	rd.CullMode = D3D11_CULL_BACK;
	g_D3DDevice->CreateRasterizerState(&rd, &g_RasterStateCullCCW);

	// カリングモード設定（CCW）
	SetCullingMode(CULL_MODE_BACK);



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
	g_D3DDevice->CreateBlendState( &blendDesc, &g_BlendStateAlphaBlend );

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateNone);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateAdd);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateSubtract);

	// アルファブレンド設定
	SetBlendState(BLEND_MODE_ALPHABLEND);




	// 深度ステンシルステート作成
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	g_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &g_DepthStateEnable );//深度有効ステート



	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	g_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &g_DepthStateDisable );//深度無効ステート



		// **********************************************************
	// 深度/ステンシル・ステート・オブジェクトの作成
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable = TRUE; // 深度テストあり
	DepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 書き込む
	DepthStencil.DepthFunc = D3D11_COMPARISON_LESS; // 手前の物体を描画
	DepthStencil.StencilEnable = FALSE; // ステンシル・テストなし
	DepthStencil.StencilReadMask = 0xff;     // ステンシル読み込みマスク。
	DepthStencil.StencilWriteMask = 0xff;     // ステンシル書き込みマスク。
			// 面が表を向いている場合のステンシル・テストの設定
	DepthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 常に成功
			// 面が裏を向いている場合のステンシル・テストの設定
	DepthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 常に成功
	hr = g_D3DDevice->CreateDepthStencilState(&DepthStencil,
		&g_pDepthStencilState);


	// 深度ステンシルステート設定
	SetDepthEnable(true);



	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 2;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	g_D3DDevice->CreateSamplerState( &samplerDesc, &samplerState[0] );

	g_ImmediateContext->PSSetSamplers( 0, 1, &samplerState[0] );


	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	hr = g_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState[1]);
	if (FAILED(hr))
		return hr;
	g_ImmediateContext->PSSetSamplers(1, 1, &samplerState[1]);



	// 頂点シェーダコンパイル・生成
	ID3DBlob* pErrorBlob;
	ID3DBlob* pVSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG) && defined(DEBUG_SHADER)
	shFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	hr = D3DX11CompileFromFile( 
		"shader.hlsl", 
		NULL,
		NULL,
		"VertexShaderPolygon",
		"vs_4_0",
		shFlag,
		0,
		NULL,
		&pVSBlob,
		&pErrorBlob,
		NULL );
	if( FAILED(hr) )
	{
		MessageBox( NULL , (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR );
	}

	g_D3DDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_VertexShader );



	// 空用頂点シェーダコンパイル・生成

#if defined(_DEBUG) && defined(DEBUG_SHADER)
	shFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	hr = D3DX11CompileFromFile( 
		"shader.hlsl", 
		NULL,
		NULL,
		"VertexShaderSky",
		"vs_4_0",
		shFlag,
		0,
		NULL,
		&pVSBlob,
		&pErrorBlob,
		NULL );
	if( FAILED(hr) )
	{
		MessageBox( NULL , (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR );
	}

	g_D3DDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_VertexShaderSky );

	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE( layout );

	g_D3DDevice->CreateInputLayout( layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&g_VertexLayout );

	pVSBlob->Release();


	// ピクセルシェーダコンパイル・生成
	ID3DBlob* pPSBlob = NULL;
	hr = D3DX11CompileFromFile( "shader.hlsl", NULL, NULL, "PixelShaderPolygon", "ps_4_0", shFlag, 0, NULL, &pPSBlob, &pErrorBlob, NULL );
	if( FAILED(hr) )
	{
		MessageBox( NULL , (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR );
	}

	g_D3DDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_PixelShader );
	
	pPSBlob->Release();


	// 空用ピクセルシェーダコンパイル・生成
	hr = D3DX11CompileFromFile( "shader.hlsl",
		NULL,
		NULL,
		"PixelShaderSky",
		"ps_4_0",
		shFlag,
		0,
		NULL,
		&pPSBlob,
		&pErrorBlob,
		NULL );
	if( FAILED(hr) )
	{
		MessageBox( NULL , (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR );
	}

	g_D3DDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_PixelShaderSky );

	pPSBlob->Release();



	//パーティクル用ピクセルシェーダコンパイル・生成
	hr = D3DX11CompileFromFile(
		"shader.hlsl",
		NULL,
		NULL,
		"PARTICLE_PS",
		"ps_4_0",
		shFlag,
		0,
		NULL,
		&pPSBlob,
		&pErrorBlob,
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	}


	g_D3DDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_PixelShaderParticle);
	


	pPSBlob->Release();


	//シャドウマップ用ピクセルシェーダコンパイル・生成
	hr = D3DX11CompileFromFile(
		"shader.hlsl",
		NULL,
		NULL,
		"PS_SM",
		"ps_4_0",
		shFlag,
		0,
		NULL,
		&pPSBlob,
		&pErrorBlob,
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	}


	g_D3DDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShaderShadow);
	


	pPSBlob->Release();

	//シャドウマップ用ピクセルシェーダコンパイル・生成
	hr = D3DX11CompileFromFile(
		"shader.hlsl",
		NULL,
		NULL,
		"xpass",
		"ps_4_0",
		shFlag,
		0,
		NULL,
		&pPSBlob,
		&pErrorBlob,
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	}


	g_D3DDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_Xpass);
	


	pPSBlob->Release();

	//シャドウマップ用ピクセルシェーダコンパイル・生成
	hr = D3DX11CompileFromFile(
		"shader.hlsl",
		NULL,
		NULL,
		"ypass",
		"ps_4_0",
		shFlag,
		0,
		NULL,
		&pPSBlob,
		&pErrorBlob,
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	}


	g_D3DDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_Ypass);
	


	pPSBlob->Release();

	

	//ジオメトリシェーダコンパイル・生成
	ID3DBlob* pGSBlob = NULL;
	hr = D3DX11CompileFromFile(
		"shader.hlsl",
		NULL,
		NULL,
		"GeometryShaderPolygon",
		"gs_4_0",
		shFlag,
		0,
		NULL,
		&pGSBlob,
		&pErrorBlob,
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "GS", MB_OK | MB_ICONERROR);
	}


	g_D3DDevice->CreateGeometryShader( pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), NULL, &g_GeometryShader);
	
	pGSBlob->Release();






	hr = CreateShaderShadow();
	if (FAILED(hr))
		return hr;


	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(XMMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);



	//ワールドマトリクス
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_WorldBuffer);
	g_ImmediateContext->VSSetConstantBuffers(0, 1, &g_WorldBuffer);//頂点シェーダーで使う定数バッファバッファ
	g_ImmediateContext->PSSetConstantBuffers(0, 1, &g_WorldBuffer);//ピクセルシェーダーで使う定数バッファバッファ

	//ビューマトリクス
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_ViewBuffer);
	g_ImmediateContext->VSSetConstantBuffers(1, 1, &g_ViewBuffer);
	g_ImmediateContext->PSSetConstantBuffers(1, 1, &g_ViewBuffer);

	//プロジェクションマトリクス
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_ProjectionBuffer);
	g_ImmediateContext->VSSetConstantBuffers(2, 1, &g_ProjectionBuffer);
	g_ImmediateContext->PSSetConstantBuffers(2, 1, &g_ProjectionBuffer);

	//マテリアル情報
	hBufferDesc.ByteWidth = sizeof(MATERIAL_CBUFFER);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_MaterialBuffer);
	g_ImmediateContext->VSSetConstantBuffers(3, 1, &g_MaterialBuffer);
	g_ImmediateContext->PSSetConstantBuffers(3, 1, &g_MaterialBuffer);

	//ライト情報
	hBufferDesc.ByteWidth = 544;
	//hBufferDesc.ByteWidth = sizeof(LIGHT_CBUFFER);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_LightBuffer);
	g_ImmediateContext->VSSetConstantBuffers(4, 1, &g_LightBuffer);
	g_ImmediateContext->PSSetConstantBuffers(4, 1, &g_LightBuffer);

	//フォグ情報
	hBufferDesc.ByteWidth = sizeof(FOG_CBUFFER);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_FogBuffer);
	g_ImmediateContext->VSSetConstantBuffers(5, 1, &g_FogBuffer);
	g_ImmediateContext->PSSetConstantBuffers(5, 1, &g_FogBuffer);


	//縁取り
	hBufferDesc.ByteWidth = sizeof(FUCHI);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_FuchiBuffer);
	g_ImmediateContext->VSSetConstantBuffers(6, 1, &g_FuchiBuffer);
	g_ImmediateContext->PSSetConstantBuffers(6, 1, &g_FuchiBuffer);

	//カメラ
	hBufferDesc.ByteWidth = sizeof(XMFLOAT4);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_CameraBuffer);
	g_ImmediateContext->VSSetConstantBuffers(7, 1, &g_CameraBuffer);
	g_ImmediateContext->PSSetConstantBuffers(7, 1, &g_CameraBuffer);




	//影情報
	hBufferDesc.ByteWidth = sizeof(SHADOWMAP_CBUFFER);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_ShadowBuffer);
	g_ImmediateContext->VSSetConstantBuffers(8, 1, &g_ShadowBuffer);
	g_ImmediateContext->PSSetConstantBuffers(8, 1, &g_ShadowBuffer);


	//ビューマトリクス
	hBufferDesc.ByteWidth = sizeof(XMMATRIX);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_SMViewBuffer);
	g_ImmediateContext->VSSetConstantBuffers(9, 1, &g_SMViewBuffer);
	g_ImmediateContext->PSSetConstantBuffers(9, 1, &g_SMViewBuffer);

	//プロジェクションマトリクス
	hBufferDesc.ByteWidth = sizeof(XMMATRIX);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_SMProjectionBuffer);
	g_ImmediateContext->VSSetConstantBuffers(10, 1, &g_SMProjectionBuffer);
	g_ImmediateContext->PSSetConstantBuffers(10, 1, &g_SMProjectionBuffer);



	//大気散乱
	hBufferDesc.ByteWidth = sizeof(SCAT_CBUFFER);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_ScatBuffer);
	g_ImmediateContext->VSSetConstantBuffers(11, 1, &g_ScatBuffer);
	g_ImmediateContext->PSSetConstantBuffers(11, 1, &g_ScatBuffer);



	//ガウシアンフィルタ
	hBufferDesc.ByteWidth = sizeof(GaussianCBuffer);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_GausBuffer);
	g_ImmediateContext->VSSetConstantBuffers(12, 1, &g_GausBuffer);
	g_ImmediateContext->PSSetConstantBuffers(12, 1, &g_GausBuffer);

	SetGausBuffer();




	// 入力レイアウト設定
	g_ImmediateContext->IASetInputLayout( g_VertexLayout );

	//// シェーダ設定
	//g_ImmediateContext->VSSetShader( g_VertexShader, NULL, 0 );
	//g_ImmediateContext->PSSetShader( g_PixelShader, NULL, 0 );

	//ライト初期化
	ZeroMemory(&g_Light, sizeof(LIGHT_CBUFFER));
	g_Light.Direction[0] = XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0f);
	g_Light.Diffuse[0] = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	g_Light.Ambient[0] = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	g_Light.Flags[0].Type = LIGHT_TYPE_DIRECTIONAL;
	SetLightBuffer();


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_D3DDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	//マテリアル初期化
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitRenderer(void)
{
	// オブジェクト解放
	if (g_DepthStateEnable)		g_DepthStateEnable->Release();
	if (g_DepthStateDisable)	g_DepthStateDisable->Release();
	if (g_BlendStateNone)		g_BlendStateNone->Release();
	if (g_BlendStateAlphaBlend)	g_BlendStateAlphaBlend->Release();
	if (g_BlendStateAdd)		g_BlendStateAdd->Release();
	if (g_BlendStateSubtract)	g_BlendStateSubtract->Release();
	if (g_RasterStateCullOff)	g_RasterStateCullOff->Release();
	if (g_RasterStateCullCW)	g_RasterStateCullCW->Release();
	if (g_RasterStateCullCCW)	g_RasterStateCullCCW->Release();

	if (g_WorldBuffer)			g_WorldBuffer->Release();
	if (g_ViewBuffer)			g_ViewBuffer->Release();
	if (g_ProjectionBuffer)		g_ProjectionBuffer->Release();
	if (g_MaterialBuffer)		g_MaterialBuffer->Release();
	if (g_LightBuffer)			g_LightBuffer->Release();
	if (g_FogBuffer)			g_FogBuffer->Release();
	if (g_ShadowBuffer)			g_ShadowBuffer->Release();
	if (g_SMViewBuffer)			g_SMViewBuffer->Release();
	if (g_SMProjectionBuffer)	g_SMProjectionBuffer->Release();

	if (g_VertexLayout)			g_VertexLayout->Release();
	if (g_VertexShader)			g_VertexShader->Release();
	if (g_PixelShader)			g_PixelShader->Release();
	if (g_VertexShaderSky)		g_VertexShaderSky->Release();
	if (g_PixelShaderSky)		g_PixelShaderSky->Release();
	if (g_PixelShaderParticle)	g_PixelShaderParticle->Release();
	if (g_GeometryShader)		g_GeometryShader->Release();

	if (g_ImmediateContext)		g_ImmediateContext->ClearState();
	if (g_RenderTargetView)		g_RenderTargetView->Release();
	if (g_SwapChain)			g_SwapChain->Release();
	if (g_ImmediateContext)		g_ImmediateContext->Release();
	if (g_D3DDevice)			g_D3DDevice->Release();

	if(g_pShadowMap)			g_pShadowMap->Release();
	if(g_pShadowMapDSView)		g_pShadowMapDSView->Release();
	if(g_pShadowMapSRView)		g_pShadowMapSRView->Release();
	if(g_pTextureSampler[0])	g_pTextureSampler[0]->Release();
	if(g_pTextureSampler[1])	g_pTextureSampler[1]->Release();
	if(g_pRasterizerStateShadow)g_pRasterizerStateShadow->Release();
	if(g_pVertexShaderShadow)	g_pVertexShaderShadow->Release();

	if (g_ShadowMapingTexture)	g_ShadowMapingTexture->Release();
	if (g_pDepthStencilState)	g_pDepthStencilState->Release();

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}



}



//=============================================================================
// バックバッファクリア
//=============================================================================
void Clear(void)
{
	// バックバッファクリア
	g_ImmediateContext->ClearRenderTargetView( g_RenderTargetView, g_ClearColor );
	g_ImmediateContext->ClearDepthStencilView( g_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void SetClearColor(float* color4)
{
	g_ClearColor[0] = color4[0];
	g_ClearColor[1] = color4[1];
	g_ClearColor[2] = color4[2];
	g_ClearColor[3] = color4[3];
}


//=============================================================================
// プレゼント
//=============================================================================
void Present(void)
{
	g_SwapChain->Present( 0, 0 );
}


//=============================================================================
// デバッグ用テキスト出力
//=============================================================================
void DebugTextOut(char* text, int x, int y)
{
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	HRESULT hr;

	//バックバッファからサーフェスを取得する
	IDXGISurface1* pBackSurface = NULL;
	hr = g_SwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackSurface);

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
		g_ImmediateContext->OMSetRenderTargets(1, &g_RenderTargetView, g_DepthStencilView);
	}
#endif
}
HRESULT InitShadowMap(void) 
{
	HRESULT hr;

	// シャドウ マップの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = ShadowMapQuarity;   // 幅
	descDepth.Height = ShadowMapQuarity;  // 高さ
	descDepth.MipLevels = 1;       // ミップマップ レベル数
	descDepth.ArraySize = 1;       // 配列サイズ
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;  // フォーマット
	descDepth.SampleDesc.Count = 1;  // マルチサンプリングの設定
	descDepth.SampleDesc.Quality = 0;  // マルチサンプリングの品質
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; // 深度/ステンシル、シェーダ リソース ビューとして使用
	descDepth.CPUAccessFlags = 0;   // CPUからはアクセスしない
	descDepth.MiscFlags = 0;   // その他の設定なし
	hr = g_D3DDevice->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		NULL,               // 
		&g_pShadowMap);     // 作成したテクスチャを受け取る変数
	if (FAILED(hr))
		return hr;  // 失敗

	// 深度/ステンシル ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;            // ビューのフォーマット
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_D3DDevice->CreateDepthStencilView(
		g_pShadowMap,         // 深度/ステンシル・ビューを作るテクスチャ
		&descDSV,             // 深度/ステンシル・ビューの設定
		&g_pShadowMapDSView); // 作成したビューを受け取る変数
	if (FAILED(hr))
		return hr;  // 失敗

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル

	// シェーダ リソース ビューの作成
	hr = g_D3DDevice->CreateShaderResourceView(
		g_pShadowMap,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&g_pShadowMapSRView);  // 受け取る変数
	if (FAILED(hr))
		return hr;


	/////////////////////////////////////
	

	// シャドウ マップの作成
	descDepth.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;  // フォーマット
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; //シェーダ リソース ビューとして使用
	hr = g_D3DDevice->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		NULL,               // 
		&g_ShadowMapingTexture);     // 作成したテクスチャを受け取る変数
	if (FAILED(hr))
		return hr;  // 失敗



	 	// レンダーターゲットビューの設定
	 D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	 	memset(&rtvDesc, 0, sizeof(rtvDesc));
	 	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	 	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	
	  	// レンダーターゲットビューの生成
	 hr = g_D3DDevice->CreateRenderTargetView(g_ShadowMapingTexture, &rtvDesc, &g_RenderTargetShadow);

	// シェーダ リソース ビューの作成
	srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル

	// シェーダ リソース ビューの作成
	hr = g_D3DDevice->CreateShaderResourceView(
		g_ShadowMapingTexture,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&g_pShadowMapSRViewBA);  // 受け取る変数
	if (FAILED(hr))
		return hr;






	// Xシャドウ マップの作成
	descDepth.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;  // フォーマット
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; //シェーダ リソース ビューとして使用
	hr = g_D3DDevice->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		NULL,               // 
		&g_ShadowMapingTextureGausX);     // 作成したテクスチャを受け取る変数
	if (FAILED(hr))
		return hr;  // 失敗



	
	  	// レンダーターゲットビューの生成
	 hr = g_D3DDevice->CreateRenderTargetView(g_ShadowMapingTextureGausX, &rtvDesc, &g_RenderTargetShadowGausX);


	// シェーダ リソース ビューの作成
	hr = g_D3DDevice->CreateShaderResourceView(
		g_ShadowMapingTextureGausX,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&g_pShadowMapSRViewGausX);  // 受け取る変数
	if (FAILED(hr))
		return hr;

	// ビューポートの設定
	g_ViewPortShadowMap[0].TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
	g_ViewPortShadowMap[0].TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
	g_ViewPortShadowMap[0].Width = ShadowMapQuarity;	// ビューポート領域の幅
	g_ViewPortShadowMap[0].Height = ShadowMapQuarity;	// ビューポート領域の高さ
	g_ViewPortShadowMap[0].MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
	g_ViewPortShadowMap[0].MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値







	// Yシャドウ マップの作成
	descDepth.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;  // フォーマット
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; //シェーダ リソース ビューとして使用
	hr = g_D3DDevice->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		NULL,               // 
		&g_ShadowMapingTextureGausY);     // 作成したテクスチャを受け取る変数
	if (FAILED(hr))
		return hr;  // 失敗



	  	// レンダーターゲットビューの生成
	 hr = g_D3DDevice->CreateRenderTargetView(g_ShadowMapingTextureGausY, &rtvDesc, &g_RenderTargetShadowGausY);


	// シェーダ リソース ビューの作成
	hr = g_D3DDevice->CreateShaderResourceView(
		g_ShadowMapingTextureGausY,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&g_pShadowMapSRViewGausY);  // 受け取る変数
	if (FAILED(hr))
		return hr;

	// ビューポートの設定
	g_ViewPortShadowMap[0].TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
	g_ViewPortShadowMap[0].TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
	g_ViewPortShadowMap[0].Width = ShadowMapQuarity;	// ビューポート領域の幅
	g_ViewPortShadowMap[0].Height = ShadowMapQuarity;	// ビューポート領域の高さ
	g_ViewPortShadowMap[0].MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
	g_ViewPortShadowMap[0].MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値

	return S_OK;
}


/*-------------------------------------------
	シェーダのコンパイル(シャドウ マップ描画)
--------------------------------------------*/
HRESULT CreateShaderShadow(void)
{
	HRESULT hr;
	// シェーダのコンパイル オプション
#if defined(DEBUG) || defined(_DEBUG)
	UINT g_flagCompile = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION
		| D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#else
	UINT g_flagCompile = D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#endif

	// **********************************************************
	// 頂点シェーダのコードをコンパイル
	ID3DBlob* pBlobVS = NULL;
	hr = D3DX11CompileFromFile(
		"shader.hlsl",  // ファイル名
		NULL,          // マクロ定義(なし)
		NULL,          // インクルード・ファイル定義(なし)
		"VS_SM",       // 「VS_SM関数」がシェーダから実行される
		"vs_4_0",      // 頂点シェーダ
		g_flagCompile,// コンパイル・オプション
		0,             // エフェクトのコンパイル・オプション(なし)
		NULL,          // 直ぐにコンパイルしてから関数を抜ける。
		&pBlobVS,      // コンパイルされたバイト・コード
		NULL,          // エラーメッセージは不要
		NULL);         // 戻り値
	if (FAILED(hr))
		return hr;

	// 頂点シェーダの作成
	hr = g_D3DDevice->CreateVertexShader(
		pBlobVS->GetBufferPointer(), // バイト・コードへのポインタ
		pBlobVS->GetBufferSize(),    // バイト・コードの長さ
		NULL,
		&g_pVertexShaderShadow); // 頂点シェーダを受け取る変数
	pBlobVS->Release();
	if (FAILED(hr))
		return hr;

	// **********************************************************
	// ラスタライザ・ステート・オブジェクトの作成
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;   // 普通に描画する
	RSDesc.CullMode = D3D11_CULL_FRONT;   // 表面を描画する
	RSDesc.FrontCounterClockwise = FALSE; // 時計回りが表面
	RSDesc.DepthBias = 0;
	RSDesc.DepthBiasClamp = 0;
	RSDesc.SlopeScaledDepthBias = 0;
	RSDesc.DepthClipEnable = TRUE;
	RSDesc.ScissorEnable = FALSE;
	RSDesc.MultisampleEnable = FALSE;
	RSDesc.AntialiasedLineEnable = FALSE;
	hr = g_D3DDevice->CreateRasterizerState(&RSDesc, &g_pRasterizerStateShadow);
	if (FAILED(hr))
		return hr;

	return hr;
}

void SetShadowEnable(BOOL flag)
{
	// フラグを更新する
	g_ShadowMap.Enable = flag;
	SetShadowBuffer();

}
void SetShadowBuffer(void)
{
	GetDeviceContext()->UpdateSubresource(g_ShadowBuffer, 0, NULL, &g_ShadowMap, 0, 0);
}

void ICClear(void)
{
	g_ImmediateContext->ClearState();
}



//シャドウマッピング用
void DrawShadowMap()
{

	XMMATRIX mtxWorld;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();
	PLAYER* player = GetPlayer();
	SUN sun=GetSun();
	//XMFLOAT3 g_SunPos = XMFLOAT3(player[0].pos.x+3000.0f, player[0].pos.y +3000.0f, player[0].pos.z -3000.0f);
	XMFLOAT3 g_SunPos = XMFLOAT3(player[0].pos.x+sun.pos.x, player[0].pos.y + sun.pos.y, player[0].pos.z +sun.pos.z);
	XMFLOAT3 g_SunUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//XMFLOAT3 g_SunUp = sun.up;
	
	XMFLOAT3 g_Opos = XMFLOAT3(player[0].pos.x , player[0].pos.y, player[0].pos.z);
	//XMFLOAT3 g_SunDirection = XMStoreFloat3(sun.Direction);

	//// 移動を反映
	//mtxTranslate = XMMatrixTranslation(g_SunPos.x, g_SunPos.y, g_SunPos.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


	XMMATRIX matShadowMapView;
	matShadowMapView = XMMatrixLookAtLH(XMLoadFloat3(&g_SunPos), XMLoadFloat3(&g_Opos), XMLoadFloat3(&g_SunUp));
	//matShadowMapView = sun.SunView;
	//matShadowMapView = XMMatrixLookToLH(XMLoadFloat3(&g_SunPos), XMLoadFloat3(&g_SunDirection), XMLoadFloat3(&g_SunUp));
	SetViewMatrix(&matShadowMapView);
	SetSMViewMatrix(&matShadowMapView);



	XMMATRIX matShadowMapProj;
	//matShadowMapProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(120.0f), g_ViewPortShadowMap[0].Width / g_ViewPortShadowMap[0].Height,  1.0f, 100000.0f);
	matShadowMapProj = XMMatrixOrthographicLH(ShadowMapHW, ShadowMapHW, 1.0f, 10000.0f);
	SetProjectionMatrix(&matShadowMapProj);
	//SetSMProjectionMatrix(&matShadowMapProj);
	//matShadowMapProj = XMMatrixTranspose(matShadowMapProj);
	//XMStoreFloat4x4(&g_ShadowMap.SMProj, matShadowMapProj);

	//SetShaderCamera(g_SunPos);

		// 描画ターゲットのクリア
	g_ImmediateContext->ClearRenderTargetView(
		g_RenderTargetShadow, // クリアする描画ターゲット
		g_ClearColor);         // クリアする値


	// 深度/ステンシルのクリア
	g_ImmediateContext->ClearDepthStencilView(g_pShadowMapDSView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// PSにサンプラーを設定
	g_ImmediateContext->PSSetSamplers(0, 2, samplerState);

	// RSにビューポートを設定
	g_ImmediateContext->RSSetViewports(1, g_ViewPortShadowMap);

	// OMに描画ターゲット ビューと深度/ステンシル・ビューを設定
	ID3D11RenderTargetView* pRender[1] = { g_RenderTargetShadow};
	g_ImmediateContext->OMSetRenderTargets(1, pRender, g_pShadowMapDSView);

	// IAに入力レイアウト・オブジェクトを設定
	g_ImmediateContext->IASetInputLayout(g_VertexLayout);

	// VSに頂点シェーダを設定
	g_ImmediateContext->VSSetShader(g_pVertexShaderShadow, NULL, 0);

	// RSにラスタライザ・ステート・オブジェクトを設定
	g_ImmediateContext->RSSetState(g_pRasterizerStateShadow);

	// PSにピクセル・シェーダを設定
	g_ImmediateContext->PSSetShader(g_pPixelShaderShadow, NULL, 0);

	//OMにブレンド・ステート・オブジェクトを設定
	FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_ImmediateContext->OMSetBlendState(g_BlendStateAlphaBlend, BlendFactor, 0xffffffff);
	// OMに深度/ステンシル・ステート・オブジェクトを設定
	g_ImmediateContext->OMSetDepthStencilState(g_pDepthStencilState, 0);

	//g_cbCBuffer.Diffuse.x = pMtl->Kd[0];
	//g_cbCBuffer.Diffuse.y = pMtl->Kd[1];
	//g_cbCBuffer.Diffuse.z = pMtl->Kd[2];
	//g_cbCBuffer.Diffuse.w = pMtl->d;



	//// 定数バッファのマップ取得
	//D3D11_MAPPED_SUBRESOURCE MappedResource;
	//HRESULT hr = g_ImmediateContext->Map(
	//	g_ShadowBuffer,              // マップするリソース
	//	0,                       // サブリソースのインデックス番号
	//	D3D11_MAP_WRITE_DISCARD, // 書き込みアクセス
	//	0,                       //
	//	&MappedResource);        // データの書き込み先ポインタ
	//if (FAILED(hr)) {
	//	return;
	//}
	//// データ書き込み
	//CopyMemory(MappedResource.pData, &g_ShadowMap, sizeof(g_ShadowMap));
	// マップ解除
	//g_ImmediateContext->Unmap(g_ShadowBuffer, 0);


	ID3D11ShaderResourceView* srv[1];
	srv[0] = NULL;

	// PSにシェーダ・リソース・ビューを設定
	g_ImmediateContext->PSSetShaderResources(
		1,              // 設定する最初のスロット番号
		1,              // 設定するシェーダ・リソース・ビューの数
		srv);			// 設定するシェーダ・リソース・ビューの配列


	


	// シャドウマップの設定

	XMMATRIX mat = XMMatrixTranspose(mtxWorld*matShadowMapView * matShadowMapProj);
	XMStoreFloat4x4(&g_ShadowMap.SMWorldViewProj, mat);

	SetShadowBuffer();


}

//シャドウマッピングした後に戻す
void DrawShadowMap2(void)
{

	DrawShadowTex();


	SetCameraAT();
	SetCamera();




	// 描画ターゲットのクリア
	g_ImmediateContext->ClearRenderTargetView(
		g_RenderTargetView, // クリアする描画ターゲット
		g_ClearColor);         // クリアする値

// 深度/ステンシルのクリア
	g_ImmediateContext->ClearDepthStencilView(
		g_DepthStencilView, // クリアする深度/ステンシル・ビュー
		D3D11_CLEAR_DEPTH,   // 深度値だけをクリアする
		1.0f,                // 深度バッファをクリアする値
		0);                  // ステンシル・バッファをクリアする値(この場合、無関係)


	// PSにサンプラーを設定
	g_ImmediateContext->PSSetSamplers(0, 2, samplerState);

	// RSにビューポートを設定
	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_ImmediateContext->RSSetViewports(1, &vp);

	// OMに描画ターゲット ビューと深度/ステンシル・ビューを設定
	g_ImmediateContext->OMSetRenderTargets(1, &g_RenderTargetView, g_DepthStencilView);


	// **********************************************************


	// IAに入力レイアウト・オブジェクトを設定
	g_ImmediateContext->IASetInputLayout(g_VertexLayout);


	// VSに頂点シェーダを設定
	g_ImmediateContext->VSSetShader(g_VertexShader, NULL, 0);

	// RSにラスタライザ・ステート・オブジェクトを設定
	g_ImmediateContext->RSSetState(g_RasterStateCullCCW);

	// PSにピクセル・シェーダを設定
	g_ImmediateContext->PSSetShader(g_PixelShader, NULL, 0);

	//OMにブレンド・ステート・オブジェクトを設定
	FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_ImmediateContext->OMSetBlendState(g_BlendStateAlphaBlend, BlendFactor, 0xffffffff);

	// OMに深度/ステンシル・ステート・オブジェクトを設定
	g_ImmediateContext->OMSetDepthStencilState(g_pDepthStencilState, 0);

	ID3D11ShaderResourceView* srv[1];
	srv[0] = g_pShadowMapSRView;
	ID3D11ShaderResourceView* srvs[1];
	srvs[0] = g_pShadowMapSRViewGausY;

	// PSにシェーダ・リソース・ビューを設定
	g_ImmediateContext->PSSetShaderResources(
		1,              // 設定する最初のスロット番号
		1,              // 設定するシェーダ・リソース・ビューの数
		srv);			// 設定するシェーダ・リソース・ビューの配列
	// PSにシェーダ・リソース・ビューを設定
	g_ImmediateContext->PSSetShaderResources(
		2,              // 設定する最初のスロット番号
		1,              // 設定するシェーダ・リソース・ビューの数
		srvs);			// 設定するシェーダ・リソース・ビューの配列




}

void SetSkyShader(void)
{
	// VSに頂点シェーダを設定
	g_ImmediateContext->VSSetShader(g_VertexShader, NULL, 0);

	g_ImmediateContext->GSSetShader(NULL, NULL, 0);

	// PSにピクセル・シェーダを設定
	g_ImmediateContext->PSSetShader(g_PixelShader, NULL, 0);

}
void SetParticleShader(void)
{
	// VSに頂点シェーダを設定
	g_ImmediateContext->VSSetShader(g_VertexShader, NULL, 0);

	g_ImmediateContext->GSSetShader(NULL, NULL, 0);

	// PSにピクセル・シェーダを設定
	g_ImmediateContext->PSSetShader(g_PixelShader, NULL, 0);

}
void SetPolygonShader(void)
{
	// VSに頂点シェーダを設定
	g_ImmediateContext->VSSetShader(g_VertexShader, NULL, 0);

	g_ImmediateContext->GSSetShader(NULL, NULL, 0);


	// PSにピクセル・シェーダを設定
	g_ImmediateContext->PSSetShader(g_PixelShader, NULL, 0);

}
void SetFieldShader(void)
{
	// VSに頂点シェーダを設定
	g_ImmediateContext->VSSetShader(g_VertexShader, NULL, 0);

	g_ImmediateContext->GSSetShader(g_GeometryShader, NULL, 0);

	// PSにピクセル・シェーダを設定
	g_ImmediateContext->PSSetShader(g_PixelShader, NULL, 0);

}

void DrawShadowTex(void)
{




	// PSにサンプラーを設定
	g_ImmediateContext->PSSetSamplers(0, 2, samplerState);

	// RSにビューポートを設定
	g_ImmediateContext->RSSetViewports(1, g_ViewPortShadowMap);


	// IAに入力レイアウト・オブジェクトを設定
	g_ImmediateContext->IASetInputLayout(g_VertexLayout);

	// VSに頂点シェーダを設定
	g_ImmediateContext->VSSetShader(g_pVertexShaderShadow, NULL, 0);

	// RSにラスタライザ・ステート・オブジェクトを設定
	g_ImmediateContext->RSSetState(g_pRasterizerStateShadow);

	// PSにピクセル・シェーダを設定
	g_ImmediateContext->PSSetShader(g_pPixelShaderShadow, NULL, 0);

	//OMにブレンド・ステート・オブジェクトを設定
	FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_ImmediateContext->OMSetBlendState(g_BlendStateAlphaBlend, BlendFactor, 0xffffffff);
	// OMに深度/ステンシル・ステート・オブジェクトを設定
	g_ImmediateContext->OMSetDepthStencilState(g_pDepthStencilState, 0);



	ID3D11ShaderResourceView* srv[1];
	srv[0] = g_pShadowMapSRView;
	ID3D11ShaderResourceView* srvs[1];
	srvs[0] = g_pShadowMapSRViewBA;








	// VSに頂点シェーダを設定
	g_ImmediateContext->VSSetShader(g_VertexShader, NULL, 0);

	g_ImmediateContext->GSSetShader(NULL, NULL, 0);

	// OMに描画ターゲット ビューと深度/ステンシル・ビューを設定
	ID3D11RenderTargetView* pRender[1] = { g_RenderTargetShadowGausX };
	g_ImmediateContext->OMSetRenderTargets(1, pRender, NULL);


	//PSにピクセル・シェーダを設定
	g_ImmediateContext->PSSetShader(g_Xpass, NULL, 0);

	{


		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_pShadowMapSRView);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, ShadowMapHW, ShadowMapHW, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	//PSにピクセル・シェーダを設定
	g_ImmediateContext->PSSetShader(g_Ypass, NULL, 0);

	{
		// OMに描画ターゲット ビューと深度/ステンシル・ビューを設定
		ID3D11RenderTargetView* pRender[1] = { g_RenderTargetShadowGausY };
		g_ImmediateContext->OMSetRenderTargets(1, pRender, NULL);


		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_pShadowMapSRViewGausX);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, ShadowMapHW, ShadowMapHW, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}



}	
void SetSrvYpass(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_pShadowMapSRViewGausY);
}
