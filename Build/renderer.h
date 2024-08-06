//=============================================================================
//
// レンダリング処理 [Renderer.h]
// Author : 
//
//=============================================================================
#pragma once
#include "CoreMinimal.h"
#include "Main.h"
#include "Buffer.h"
#include "LightComponent.h"
#include "ShadowMapping.h"
//*********************************************************
// マクロ定義
//*********************************************************

enum LIGHT_TYPE
{
	LIGHT_TYPE_NONE,		//ライト無し
	LIGHT_TYPE_DIRECTIONAL,	//ディレクショナルライト
	LIGHT_TYPE_POINT,		//ポイントライト

	LIGHT_TYPE_NUM
};

enum BLEND_MODE
{
	BLEND_MODE_NONE,		//ブレンド無し
	BLEND_MODE_ALPHABLEND,	//αブレンド
	BLEND_MODE_ADD,			//加算ブレンド
	BLEND_MODE_SUBTRACT,	//減算ブレンド

	BLEDD_MODE_NUM
};

enum CULL_MODE
{
	CULL_MODE_NONE,			//カリング無し
	CULL_MODE_FRONT,		//表のポリゴンを描画しない(CW)
	CULL_MODE_BACK,			//裏のポリゴンを描画しない(CCW)

	CULL_MODE_NUM
};


//*********************************************************
// 構造体
//*********************************************************

// 頂点構造体
struct VERTEX_3D
{
    XMFLOAT3	Position;
    XMFLOAT3	Normal;
    XMFLOAT4	Diffuse;
    XMFLOAT2	TexCoord;
};

// マテリアル構造体
struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	int			noDiffuseTex;
	int			noNormalTex;
	int			phong;				//0=lambart,1=phong
};


// フォグ構造体
struct FOG {
	float		FogStart;	// フォグの開始距離
	float		FogEnd;		// フォグの最大距離
	XMFLOAT4	FogColor;	// フォグの色
};

// マテリアル用定数バッファ構造体
struct MATERIAL_CBUFFER
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	int			noDiffuseTex;
	int			noNormalTex;
	int			phong;					//0=lambart,1=phong
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
	XMFLOAT4	direction[LIGHT_MAX];	// ライトの方向
	XMFLOAT4	Position[LIGHT_MAX];	// ライトの位置
	XMFLOAT4	Diffuse[LIGHT_MAX];		// 拡散光の色
	XMFLOAT4	Ambient[LIGHT_MAX];		// 環境光の色
	XMFLOAT4	Attenuation[LIGHT_MAX];	// 減衰率
	LIGHTFLAGS	Flags[LIGHT_MAX];		// ライト種別
	int			Enable;					// ライティング有効・無効フラグ
	int			Dummy[15];				// 16byte境界用
};

// フォグ用定数バッファ構造体
struct FOG_CBUFFER
{
	XMFLOAT4	Fog;					// フォグ量
	XMFLOAT4	FogColor;				// フォグの色
	int			Enable;					// フォグ有効・無効フラグ
	float		Dummy[3];				// 16byte境界用
};

// 縁取り用バッファ
struct FUCHI
{
	int			fuchi;
	int			fill[3];
};

struct GaussianCBuffer {
	float weight[8];
};



class Main;

class Renderer
{
public:

	enum ShaderBF_MODE
	{
		DEFAULT_BF,
		PARTICAL_BF,
	};

	Renderer(Main*main);
	~Renderer();

	ID3DBlob* CreateVSFile(const char* shaderName, char* fName , ID3D11VertexShader** VS);
	void CreatePSFile(char* shaderName, char* fName , ID3D11PixelShader** PS);
	void CreateCSFile(char* shaderName, char* fName , ID3D11ComputeShader** CS);

	XMMATRIX GetViewMatrix(void);
	XMMATRIX GetProjectionMatrix(void);

	HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void UninitRenderer(void);

	void Clear(void);
	void Present(void);

	ID3D11Device *GetDevice( void );
	ID3D11DeviceContext *GetDeviceContext( void );

	void SetDepthEnable( BOOL Enable );
	void SetBlendState(BLEND_MODE bm);
	void SetCullingMode(CULL_MODE cm);
	void SetAlphaTestEnable(BOOL flag);

	void SetWorldViewProjection2D( void );
	void SetWorldMatrix( XMMATRIX *WorldMatrix );
	void SetViewMatrix( XMMATRIX *ViewMatrix );
	void SetProjectionMatrix( XMMATRIX *ProjectionMatrix );

	void SetMaterial( MATERIAL material );

	void SetShaderBuffersMode(ShaderBF_MODE bfMode);

	void SetLightBuffer(void); // static
	void SetLightEnable(BOOL flag);
	void SetLight(LightComponent* light);

	void SetFogBuffer(void);
	void SetFogEnable(BOOL flag);
	void SetFog(FOG* fog);

	void DebugTextOut(char* text, int x, int y);

	void SetFuchi(int flag);
	void SetShaderCamera(XMFLOAT3 pos);

	void SetShadow(SHADOWMAP_CBUFFER* shadow);
	
	void SetGausBuffer(void);
	void SetClearColor(float* color4);

	ID3D11InputLayout** GetVertexLayout(void);

	void SetShaderDefault(void);


	IDXGISwapChain* GetSwapChain(void);

private:

	Main* main;

	D3D_FEATURE_LEVEL       FeatureLevel;

	ID3D11Device* m_D3DDevice ;
	ID3D11DeviceContext* m_ImmediateContext;
	IDXGISwapChain* SwapChain;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11DepthStencilView* DepthStencilView;
	ID3D11Texture2D* depthTexture;

	D3D11_VIEWPORT defaultViewPort;

	void	InitConstantBuffers(void);



	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* VertexLayout;

	Buffer<XMMATRIX>			*m_WorldBuffer;
	Buffer<XMMATRIX>			*m_ViewBuffer;
	Buffer<XMMATRIX>			*m_ProjectionBuffer;
	Buffer<MATERIAL_CBUFFER>	*m_MaterialBuffer;
	Buffer<LIGHT_CBUFFER>		*m_LightBuffer;
	Buffer<FOG_CBUFFER>			*m_FogBuffer;
	Buffer<FUCHI>				*m_FuchiBuffer;
	Buffer<XMFLOAT4>			*m_CameraBuffer;
	Buffer<SHADOWMAP_CBUFFER>	*m_ShadowBuffer;
	Buffer<GaussianCBuffer>		*m_GausBuffer;
	

	ID3D11DepthStencilState* DepthStateEnable;
	ID3D11DepthStencilState* DepthStateDisable;

	ID3D11BlendState* BlendStateNone;
	ID3D11BlendState* BlendStateAlphaBlend;
	ID3D11BlendState* BlendStateAdd;
	ID3D11BlendState* BlendStateSubtract;
	BLEND_MODE				BlendStateParam;


	ID3D11RasterizerState* RasterStateCullOff;
	ID3D11RasterizerState* RasterStateCullCW;
	ID3D11RasterizerState* RasterStateCullCCW;

	MATERIAL_CBUFFER	Material;
	LIGHT_CBUFFER		Light;
	FOG_CBUFFER			Fog;

	FUCHI				Fuchi;

	GaussianCBuffer		Gaus;

	float ClearColor[4] ;	// 背景色

	//////////////////////////////////////////
	// 変更しました							//
	//////////////////////////////////////////
	//仮
	XMMATRIX projection;
	XMMATRIX view;


};


