//=============================================================================
//
// 木処理 [sun.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "sun.h"
#include"light.h"
#include"debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(5)				// テクスチャの数

#define	SUN_WIDTH			(500.0f)			// 頂点サイズ
#define	SUN_HEIGHT			(500.0f)			// 頂点サイズ

#define	MAX_SUN			(1)			// 木最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT3	pos;			// 位置
	XMFLOAT3	scl;			// スケール
	MATERIAL	material;		// マテリアル
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	BOOL		use;			// 使用しているかどうか

} SUNOBJ;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexSun(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static SUNOBJ					g_Sun;	// 木ワーク
static int					g_TexNo;			// テクスチャ番号
//static int					g_nAlpha;			// アルファテストの閾値

static char *g_TextureName[] =
{
	"data/TEXTURE/sun.png",
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSun(void)
{
	MakeVertexSun();

	// テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_TexNo = 0;

		ZeroMemory(&g_Sun.material, sizeof(g_Sun.material));
		g_Sun.material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_Sun.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Sun.scl = XMFLOAT3(100000.0f, 100000.0f, 100000.0f);
		g_Sun.fWidth = SUN_WIDTH;
		g_Sun.fHeight = SUN_HEIGHT;
		g_Sun.use = TRUE;


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSun(void)
{
	for(int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if(g_Texture[nCntTex] != NULL)
		{// テクスチャの解放
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	if(g_VertexBuffer != NULL)
	{// 頂点バッファの解放
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSun(void)
{
	SUN sun = GetSun();
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, sun.Direction);

	g_Sun.pos = { sun.pos.x*10000.0f ,sun.pos.y * 10000.0f,sun.pos.z * 10000.0f };
	//g_Sun.pos = { pos.x*-1000000.0f ,pos.y * -1000000.0f,pos.z * -100000.0f };


#ifdef _DEBUG

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSun(void)
{

	SetCullingMode(CULL_MODE_BACK);

	SetAlphaTestEnable(FALSE);

	SetBlendState(BLEND_MODE_ADD);

	// ライティングを無効
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView, mtxRot;
	CAMERA* cam = GetCamera();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// ビューマトリックスを取得
	mtxView = XMLoadFloat4x4(&cam->mtxView);



	// 正方行列（直交行列）を転置行列させて逆行列を作ってる版(速い)
	mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
	mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
	mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

	mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
	mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
	mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

	mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
	mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
	mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];




	// スケールを反映
	mtxScl = XMMatrixScaling(g_Sun.scl.x, g_Sun.scl.y, g_Sun.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Sun.pos.x, g_Sun.pos.y, g_Sun.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);


	// マテリアル設定
	SetMaterial(g_Sun.material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

	// ポリゴンの描画
	GetDeviceContext()->Draw(4, 0);


	// ライティングを有効に
	SetLightEnable(TRUE);
	SetAlphaTestEnable(FALSE);
	SetBlendState(BLEND_MODE_ALPHABLEND);


	SetCullingMode(CULL_MODE_BACK);


#ifdef _DEBUG



#endif


		// αテストを無効に
		SetAlphaTestEnable(FALSE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexSun(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// 頂点バッファに値をセットする
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = 60.0f;
	float fHeight = 60.0f;

	// 頂点座標の設定
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight/2, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight/2, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, -fHeight / 2, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 2.0f, -fHeight / 2, 0.0f);

	// 法線の設定
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 拡散光の設定
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}

