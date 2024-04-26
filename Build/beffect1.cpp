//=============================================================================
//
// パーティクル処理 [beffect1.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "light.h"
#include "beffect1.h"
#include "player.h"
#include"renderer.h"
#include"debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)			// テクスチャの数
//
//#define	g_aBeffect1[nCntBeffect1].fSizeX		(1.0f)		// 頂点サイズ
//#define	g_aBeffect1[nCntBeffect1].fSizeY		(1.0f)		// 頂点サイズ
#define	VALUE_MOVE_BEFFECT1	(5.0f)		// 移動速度

#define	MAX_BEFFECT1		(2048)		// パーティクル最大数

#define	DISP_SHADOW						// 影の表示

#define MAX_POLYGON			(MAX_BEFFECT1*2)
#define MAX_VERTEX			(MAX_POLYGON*6)
#define BEFFECT1_SIZE		(1.0f)
#define POLYGON_VERTEX		(6)
#define CLOUD_ALPHA			(0.8f)
#define BOMB_MOVE			(10.0f)
#define BOMB_POP			(30)
#define BOMB_LIFE			(60)
#define BOMB_SIZE			(100.0f)
//#undef DISP_SHADOW

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBeffect1(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点バッファ
// 頂点配列
static VERTEX_3D g_VertexArray[MAX_BEFFECT1][POLYGON_VERTEX];




static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;					// テクスチャ番号

static BEFFECT1					g_aBeffect1[MAX_BEFFECT1];		// パーティクルワーク
static XMFLOAT3					g_posBase;						// ビルボード発生位置
static float					g_fWidthBase = 5.0f;			// 基準の幅
static float					g_fHeightBase = 10.0f;			// 基準の高さ
static float					g_roty = 0.0f;					// 移動方向
static float					g_spd = 0.0f;					// 移動スピード
static int						g_partcnt = 0;

static XMFLOAT3 g_vpos[4];


static char* g_TextureName[] =
{
	//"data/TEXTURE/effect000.jpg",
	//"data/TEXTURE/siro.png",
	"data/TEXTURE/particle_smoke.png",
	//"data/TEXTURE/beffect1_sakura.png",
};

static BOOL							g_Load = FALSE;

// コンピュートシェーダー
static ID3D11ComputeShader* mComputeShader = NULL;

// パーティクル
//static BEFFECT1* mbeffect1;
// バッファ
//static ID3D11Buffer* mVertexBuffer;
static ID3D11Buffer* mpBeffect1Buffer;
static ID3D11Buffer* mpResultBuffer;
static ID3D11Buffer* mpPositionBuffer;
// SRV
static ID3D11ShaderResourceView* mpBeffect1SRV;
static ID3D11ShaderResourceView* mpPositionSRV;
// UAV
static ID3D11UnorderedAccessView* mpResultUAV;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBeffect1(void)
{
	// テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_TexNo = 0;

	// パーティクルワークの初期化
	for (int nCntBeffect1 = 0; nCntBeffect1 < MAX_BEFFECT1; nCntBeffect1++)
	{
		g_aBeffect1[nCntBeffect1].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aBeffect1[nCntBeffect1].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aBeffect1[nCntBeffect1].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aBeffect1[nCntBeffect1].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_aBeffect1[nCntBeffect1].material, sizeof(g_aBeffect1[nCntBeffect1].material));
		g_aBeffect1[nCntBeffect1].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		//g_aBeffect1[nCntBeffect1].fSizeX = g_aBeffect1[nCntBeffect1].fSizeX;
		//g_aBeffect1[nCntBeffect1].fSizeY = g_aBeffect1[nCntBeffect1].fSizeY;
		g_aBeffect1[nCntBeffect1].nLife = 0;
		g_aBeffect1[nCntBeffect1].use = FALSE;
		g_aBeffect1[nCntBeffect1].type = 1;



		for (int i = 0; i < 4; i++)
		{
			g_vpos[i] = g_aBeffect1[nCntBeffect1].pos;
		}

		g_vpos[0].x -= g_aBeffect1[nCntBeffect1].fSizeX / 2;
		g_vpos[0].y += g_aBeffect1[nCntBeffect1].fSizeY / 2;


		g_vpos[1].x += g_aBeffect1[nCntBeffect1].fSizeX / 2;
		g_vpos[1].y += g_aBeffect1[nCntBeffect1].fSizeY / 2;


		g_vpos[2].x -= g_aBeffect1[nCntBeffect1].fSizeX / 2;
		g_vpos[2].y -= g_aBeffect1[nCntBeffect1].fSizeY / 2;


		g_vpos[3].x += g_aBeffect1[nCntBeffect1].fSizeX / 2;
		g_vpos[3].y -= g_aBeffect1[nCntBeffect1].fSizeY / 2;



		g_VertexArray[nCntBeffect1][0] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
		g_VertexArray[nCntBeffect1][1] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
		g_VertexArray[nCntBeffect1][2] = { g_vpos[1], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) };
		g_VertexArray[nCntBeffect1][3] = { g_vpos[2], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) };
		g_VertexArray[nCntBeffect1][4] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };
		g_VertexArray[nCntBeffect1][5] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };




	}

	MakeVertexBeffect1();








	//バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	hBufferDesc.ByteWidth = sizeof(BEFFECT1);
	hBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	hBufferDesc.StructureByteStride = (UINT)MAX_BEFFECT1;
	hBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	hBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	//// 定数バッファ生成
	//D3D11_BUFFER_DESC hBufferDesc;
	//hBufferDesc.ByteWidth = sizeof(BEFFECT1);
	//hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//hBufferDesc.CPUAccessFlags = 0;
	//hBufferDesc.MiscFlags = 0;
	//hBufferDesc.StructureByteStride = (UINT)MAX_BEFFECT1;


	GetDevice()->CreateBuffer(&hBufferDesc, NULL, &mpBeffect1Buffer);
	GetDevice()->CreateBuffer(&hBufferDesc, NULL, &mpResultBuffer);


	hBufferDesc.ByteWidth = sizeof(XMFLOAT3);

	GetDevice()->CreateBuffer(&hBufferDesc, NULL, &mpPositionBuffer);


	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル



	// SRV生成
	GetDevice()->CreateShaderResourceView(
		mpBeffect1Buffer,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&mpBeffect1SRV);  // 受け取る変数




	// SRV生成
	GetDevice()->CreateShaderResourceView(
		mpPositionBuffer,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&mpPositionSRV);  // 受け取る変数



	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;

	ZeroMemory(&uavDesc, sizeof(uavDesc));

	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = 0;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;


	// UAV生成
	GetDevice()->CreateUnorderedAccessView(
		mpResultBuffer,          // アクセスするテクスチャ リソース
		&uavDesc,               // シェーダ リソース ビューの設定
		&mpResultUAV);  // 受け取る変数



	//D3DX11CompileFromFile("shader.hlsl",
	//	NULL,
	//	NULL,
	//	"CSFunc",
	//	"cs_4_0",
	//	shFlag,
	//	0,
	//	NULL,
	//	&pPSBlob,
	//	&pErrorBlob,
	//	NULL);





	g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_roty = 0.0f;
	g_spd = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBeffect1(void)
{
	if (g_Load == FALSE) return;

	//テクスチャの解放
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// 頂点バッファの解放
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	if (mComputeShader)		mComputeShader->Release();

	if (mpBeffect1Buffer)	mpBeffect1Buffer->Release();
	if (mpResultBuffer)		mpResultBuffer->Release();
	if (mpPositionBuffer)	mpPositionBuffer->Release();
	if (mpBeffect1SRV)		mpResultBuffer->Release();
	if (mpPositionSRV)		mpBeffect1Buffer->Release();
	if (mpResultUAV)		mpResultBuffer->Release();




	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBeffect1(void)
{

	CAMERA* cam = GetCamera();

	for (int nCntBeffect1 = 0; nCntBeffect1 < MAX_BEFFECT1; nCntBeffect1++)
	{
		if (g_aBeffect1[nCntBeffect1].use)
		{// 使用中
			switch (g_aBeffect1[nCntBeffect1].type)
			{
			case 0:
				g_aBeffect1[nCntBeffect1].pos.x += g_aBeffect1[nCntBeffect1].move.x;
				g_aBeffect1[nCntBeffect1].pos.z += g_aBeffect1[nCntBeffect1].move.z;

				g_aBeffect1[nCntBeffect1].pos.y += g_aBeffect1[nCntBeffect1].move.y;
				if (g_aBeffect1[nCntBeffect1].pos.y <= g_aBeffect1[nCntBeffect1].fSizeY / 2)
				{// 着地した
					g_aBeffect1[nCntBeffect1].pos.y = g_aBeffect1[nCntBeffect1].fSizeY / 2;
					g_aBeffect1[nCntBeffect1].move.y = -g_aBeffect1[nCntBeffect1].move.y * 0.75f;
				}

				g_aBeffect1[nCntBeffect1].move.x += (0.0f - g_aBeffect1[nCntBeffect1].move.x) * 0.015f;
				g_aBeffect1[nCntBeffect1].move.y -= 0.25f;
				g_aBeffect1[nCntBeffect1].move.z += (0.0f - g_aBeffect1[nCntBeffect1].move.z) * 0.015f;


				g_aBeffect1[nCntBeffect1].nLife--;
				if (g_aBeffect1[nCntBeffect1].nLife <= 0)
				{
					g_aBeffect1[nCntBeffect1].use = FALSE;
					g_partcnt -= 1;
				}
				else
				{
					if (g_aBeffect1[nCntBeffect1].nLife <= 80)
					{
						g_aBeffect1[nCntBeffect1].material.Diffuse.x = 0.8f - (float)(80 - g_aBeffect1[nCntBeffect1].nLife) / 80 * 0.8f;
						g_aBeffect1[nCntBeffect1].material.Diffuse.y = 0.7f - (float)(80 - g_aBeffect1[nCntBeffect1].nLife) / 80 * 0.7f;
						g_aBeffect1[nCntBeffect1].material.Diffuse.z = 0.2f - (float)(80 - g_aBeffect1[nCntBeffect1].nLife) / 80 * 0.2f;
					}

					if (g_aBeffect1[nCntBeffect1].nLife <= 20)
					{
						// α値設定
						g_aBeffect1[nCntBeffect1].material.Diffuse.w -= 0.05f;
						if (g_aBeffect1[nCntBeffect1].material.Diffuse.w < 0.0f)
						{
							g_aBeffect1[nCntBeffect1].material.Diffuse.w = 0.0f;
						}
					}
				}
			case 1:
				g_aBeffect1[nCntBeffect1].pos.x += g_aBeffect1[nCntBeffect1].move.x;
				g_aBeffect1[nCntBeffect1].pos.z += g_aBeffect1[nCntBeffect1].move.z;
				g_aBeffect1[nCntBeffect1].pos.y += g_aBeffect1[nCntBeffect1].move.y;

				g_aBeffect1[nCntBeffect1].nLife--;
				if (g_aBeffect1[nCntBeffect1].nLife <= 0)
				{
					g_aBeffect1[nCntBeffect1].use = FALSE;
					g_partcnt -= 1;
				}
				else
				{
					if (g_aBeffect1[nCntBeffect1].nLife <= 80)
					{
						//g_aBeffect1[nCntBeffect1].material.Diffuse.x = 0.9f - (float)(80 - g_aBeffect1[nCntBeffect1].nLife) / 80 * 0.9f;
						//g_aBeffect1[nCntBeffect1].material.Diffuse.y = 0.9f - (float)(80 - g_aBeffect1[nCntBeffect1].nLife) / 80 * 0.9f;
						//g_aBeffect1[nCntBeffect1].material.Diffuse.z = 0.9f - (float)(80 - g_aBeffect1[nCntBeffect1].nLife) / 80 * 0.9f;
					}

					if (g_aBeffect1[nCntBeffect1].nLife <= 20)
					{
						// α値設定
						g_aBeffect1[nCntBeffect1].material.Diffuse.w -= 0.05f;
						if (g_aBeffect1[nCntBeffect1].material.Diffuse.w < 0.0f)
						{
							g_aBeffect1[nCntBeffect1].material.Diffuse.w = 0.0f;
						}
					}
				}
			case 2:
				g_aBeffect1[nCntBeffect1].pos.x += g_aBeffect1[nCntBeffect1].move.x;
				g_aBeffect1[nCntBeffect1].pos.z += g_aBeffect1[nCntBeffect1].move.z;
				g_aBeffect1[nCntBeffect1].pos.y += g_aBeffect1[nCntBeffect1].move.y;

				g_aBeffect1[nCntBeffect1].nLife--;
				if (g_aBeffect1[nCntBeffect1].nLife <= 0)
				{
					g_aBeffect1[nCntBeffect1].use = FALSE;
					g_partcnt -= 1;
				}
				else
				{
					if (g_aBeffect1[nCntBeffect1].nLife <= 80)
					{
						//g_aBeffect1[nCntBeffect1].material.Diffuse.x = 0.9f - (float)(80 - g_aBeffect1[nCntBeffect1].nLife) / 80 * 0.9f;
						//g_aBeffect1[nCntBeffect1].material.Diffuse.y = 0.9f - (float)(80 - g_aBeffect1[nCntBeffect1].nLife) / 80 * 0.9f;
						//g_aBeffect1[nCntBeffect1].material.Diffuse.z = 0.9f - (float)(80 - g_aBeffect1[nCntBeffect1].nLife) / 80 * 0.9f;
					}

					if (g_aBeffect1[nCntBeffect1].nLife <= 20)
					{
						// α値設定
						g_aBeffect1[nCntBeffect1].material.Diffuse.w -= 0.005f;
						if (g_aBeffect1[nCntBeffect1].material.Diffuse.w < 0.0f)
						{
							g_aBeffect1[nCntBeffect1].material.Diffuse.w = 0.0f;
						}
					}
				}

			default:
				break;
			}


			XMFLOAT3 vpos[4];

			vpos[0].x = -g_aBeffect1[nCntBeffect1].fSizeX / 2;
			vpos[0].y = +g_aBeffect1[nCntBeffect1].fSizeY / 2;
			vpos[0].z = 0.0f;

			vpos[1].x = +g_aBeffect1[nCntBeffect1].fSizeX / 2;
			vpos[1].y = +g_aBeffect1[nCntBeffect1].fSizeY / 2;
			vpos[1].z = 0.0f;

			vpos[2].x = -g_aBeffect1[nCntBeffect1].fSizeX / 2;
			vpos[2].y = -g_aBeffect1[nCntBeffect1].fSizeY / 2;
			vpos[2].z = 0.0f;

			vpos[3].x = +g_aBeffect1[nCntBeffect1].fSizeX / 2;
			vpos[3].y = -g_aBeffect1[nCntBeffect1].fSizeY / 2;
			vpos[3].z = 0.0f;

			XMVECTOR vvec[4];


			//XMVECTOR axx= XMVector3Normalize(cam->axisX);

			//XMVECTOR axy= XMVector3Normalize(cam->axisY);


			XMVECTOR axx;

			XMVECTOR axy;

			XMFLOAT4X4 view = cam->mtxView;

			XMFLOAT3 vx = XMFLOAT3(view._11, view._21, view._31);
			axx = XMLoadFloat3(&vx);
			XMFLOAT3 vy = XMFLOAT3(view._12, view._22, view._32);
			axy = XMLoadFloat3(&vy);
			for (int i = 0; i < 4; i++)
			{

				vvec[i] += XMLoadFloat3(&vpos[i]);
				vvec[i] = XMVector3Normalize(vvec[i]);
				switch (i)
				{
				case 0:

					vvec[i] = axy * g_aBeffect1[nCntBeffect1].fSizeY;
					break;
				case 1:
					vvec[i] = axx * g_aBeffect1[nCntBeffect1].fSizeX;
					break;


				case 2:


					vvec[i] = axx * -g_aBeffect1[nCntBeffect1].fSizeX;
					break;

				case 3:

					vvec[i] = axy * -g_aBeffect1[nCntBeffect1].fSizeY;
					break;


				default:
					break;
				}

				//XMVector3Rotate(vvec[i],cam->qtonrot);
				XMStoreFloat3(&vpos[i], vvec[i]);

			}







			for (int i = 0; i < 4; i++)
			{
				g_vpos[i] = g_aBeffect1[nCntBeffect1].pos;
			}



			//g_vpos[0].x -= g_aBeffect1[nCntBeffect1].fSizeX / 2;
			//g_vpos[0].y += g_aBeffect1[nCntBeffect1].fSizeY / 2;

			//g_vpos[1].x += g_aBeffect1[nCntBeffect1].fSizeX / 2;
			//g_vpos[1].y += g_aBeffect1[nCntBeffect1].fSizeY / 2;

			//g_vpos[2].x -= g_aBeffect1[nCntBeffect1].fSizeX / 2;
			//g_vpos[2].y -= g_aBeffect1[nCntBeffect1].fSizeY / 2;

			//g_vpos[3].x += g_aBeffect1[nCntBeffect1].fSizeX / 2;
			//g_vpos[3].y -= g_aBeffect1[nCntBeffect1].fSizeY / 2;

			g_vpos[0].x += vpos[0].x;
			g_vpos[0].y += vpos[0].y;
			g_vpos[0].z += vpos[0].z;

			g_vpos[1].x += vpos[1].x;
			g_vpos[1].y += vpos[1].y;
			g_vpos[1].z += vpos[1].z;

			g_vpos[2].x += vpos[2].x;
			g_vpos[2].y += vpos[2].y;
			g_vpos[2].z += vpos[2].z;

			g_vpos[3].x += vpos[3].x;
			g_vpos[3].y += vpos[3].y;
			g_vpos[3].z += vpos[3].z;





			g_VertexArray[nCntBeffect1][0] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect1[nCntBeffect1].material.Diffuse, XMFLOAT2(0.0f, 0.0f) };
			g_VertexArray[nCntBeffect1][1] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect1[nCntBeffect1].material.Diffuse, XMFLOAT2(0.0f, 0.0f) };
			g_VertexArray[nCntBeffect1][2] = { g_vpos[1], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect1[nCntBeffect1].material.Diffuse, XMFLOAT2(1.0f, 0.0f) };
			g_VertexArray[nCntBeffect1][3] = { g_vpos[2], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect1[nCntBeffect1].material.Diffuse, XMFLOAT2(0.0f, 1.0f) };
			g_VertexArray[nCntBeffect1][4] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect1[nCntBeffect1].material.Diffuse, XMFLOAT2(1.0f, 1.0f) };
			g_VertexArray[nCntBeffect1][5] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect1[nCntBeffect1].material.Diffuse, XMFLOAT2(1.0f, 1.0f) };

		}
		else
		{
			g_VertexArray[nCntBeffect1][0] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
			g_VertexArray[nCntBeffect1][1] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
			g_VertexArray[nCntBeffect1][2] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) };
			g_VertexArray[nCntBeffect1][3] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) };
			g_VertexArray[nCntBeffect1][4] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };
			g_VertexArray[nCntBeffect1][5] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };



		}


	}



	//// 頂点バッファに値をセットする
	//D3D11_MAPPED_SUBRESOURCE msr;
	//GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	//VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	//// 全頂点情報を毎回上書きしているのはDX11ではこの方が早いからです
	//memcpy(pVtx, g_VertexArray, sizeof(VERTEX_3D)* MAX_VERTEX);

	//GetDeviceContext()->Unmap(g_VertexBuffer, 0);



	if (g_VertexBuffer)			g_VertexBuffer->Release();
	MakeVertexBeffect1();


	//GetDeviceContext()->UpdateSubresource(g_VertexBuffer, 0, NULL, &g_VertexArray, 0, 0);

	// パーティクル発生
	//{
	//	XMFLOAT3 pos;
	//	XMFLOAT3 move;
	//	float fAngle, fLength;
	//	int nLife;
	//	float fSize;

	//	pos = g_posBase;

	//	fAngle = (float)(rand() % 628 - 314) / 100.0f;
	//	fLength = rand() % (int)(g_fWidthBase * 200 ) / 100.0f - g_fWidthBase;
	//	move.x = sinf(fAngle) * fLength;
	//	move.y = rand() % 300 / 100.0f + g_fHeightBase;
	//	move.z = cosf(fAngle) * fLength;

	//	nLife = rand() % 100 + 150;  

	//	fSize = (float)(rand() % 30 + 20);

	//	pos.y = fSize / 2;

	//	// ビルボードの設定
	//	SetBeffect1(pos, move, XMFLOAT4(0.8f, 0.7f, 0.2f, 0.85f), fSize, fSize, nLife,0);
	//}




}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBeffect1(void)
{
	//XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	//CAMERA* cam = GetCamera();

	SetCullingMode(CULL_MODE_NONE);

	// ライティングを無効に
	SetLightEnable(FALSE);

	// 加算合成に設定
	SetBlendState(BLEND_MODE_ADD);

	// Z比較無し
	SetDepthEnable(FALSE);

	// フォグ無効
	SetFogEnable(FALSE);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	CAMERA* cam = GetCamera();


	XMMATRIX  mtxWorld, mtxView;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// ビューマトリックスを取得
	mtxView = XMLoadFloat4x4(&cam->mtxView);


	// 関数使った版
	//mtxWorld = XMMatrixInverse(nullptr, mtxView);
	//mtxWorld.r[3].m128_f32[0] = 0.0f;
	//mtxWorld.r[3].m128_f32[1] = 0.0f;
	//mtxWorld.r[3].m128_f32[2] = 0.0f;


	//// 正方行列（直交行列）を転置行列させて逆行列を作ってる版(速い)
	//mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
	//mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
	//mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

	//mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
	//mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
	//mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

	//mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
	//mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
	//mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];




	//// スケールを反映
	//mtxScl = XMMatrixScaling(g_aBeffect1[0].scale.x, g_aBeffect1[0].scale.y, g_aBeffect1[0].scale.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);


	//// 回転を反映
	//mtxRot = XMMatrixRotationRollPitchYaw(0.0f, XM_PI, 0.0f);



	//// 移動を反映
	//mtxTranslate = XMMatrixTranslation(g_aBeffect1[0].pos.x, g_aBeffect1[0].pos.y, g_aBeffect1[0].pos.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);



	// ポリゴン描画
	int cnt = sizeof(g_VertexArray) / sizeof(VERTEX_3D);
	GetDeviceContext()->Draw(cnt, 0);		// cnt頂点分を0番目の頂点番号から描画

#ifdef _DEBUG	// デバッグ情報を表示する


#endif


	// ライティングを有効に
	SetLightEnable(TRUE);

	// 通常ブレンドに戻す
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z比較有効
	SetDepthEnable(TRUE);

	// フォグ有効
	SetFogEnable(GetFogEnable());

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBeffect1(void)
{



	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4 * MAX_POLYGON;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	// グローバルの頂点配列から頂点バッファをつくる
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = g_VertexArray;

	GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);


	return S_OK;
}

//=============================================================================
// マテリアルカラーの設定
//=============================================================================
void SetColorBeffect1(int nIdxBeffect1, XMFLOAT4 col)
{
	g_aBeffect1[nIdxBeffect1].material.Diffuse = col;
}

//=============================================================================
// パーティクルの発生処理
//=============================================================================
int SetParticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife, int type)
{
	int nIdxBeffect1 = -1;

	for (int nCntBeffect1 = 0; nCntBeffect1 < MAX_BEFFECT1; nCntBeffect1++)
	{
		if (!g_aBeffect1[nCntBeffect1].use)
		{



			g_aBeffect1[nCntBeffect1].pos = pos;
			g_aBeffect1[nCntBeffect1].rot = { 0.0f, 0.0f, 0.0f };
			g_aBeffect1[nCntBeffect1].scale = { fSizeX, fSizeY, fSizeY };
			g_aBeffect1[nCntBeffect1].move = move;
			g_aBeffect1[nCntBeffect1].material.Diffuse = col;
			g_aBeffect1[nCntBeffect1].fSizeX = fSizeX;
			g_aBeffect1[nCntBeffect1].fSizeY = fSizeY;
			g_aBeffect1[nCntBeffect1].nLife = nLife;
			g_aBeffect1[nCntBeffect1].use = TRUE;

			g_partcnt += 1;
			for (int i = 0; i < 4; i++)
			{
				g_vpos[i] = g_aBeffect1[nCntBeffect1].pos;
			}
			g_vpos[0].x -= g_aBeffect1[nCntBeffect1].fSizeX / 2;
			g_vpos[0].y += g_aBeffect1[nCntBeffect1].fSizeY / 2;

			g_vpos[1].x += g_aBeffect1[nCntBeffect1].fSizeX / 2;
			g_vpos[1].y += g_aBeffect1[nCntBeffect1].fSizeY / 2;

			g_vpos[2].x -= g_aBeffect1[nCntBeffect1].fSizeX / 2;
			g_vpos[2].y -= g_aBeffect1[nCntBeffect1].fSizeY / 2;

			g_vpos[3].x += g_aBeffect1[nCntBeffect1].fSizeX / 2;
			g_vpos[3].y -= g_aBeffect1[nCntBeffect1].fSizeY / 2;





			g_VertexArray[nCntBeffect1][0].Diffuse = g_aBeffect1[nCntBeffect1].material.Diffuse;
			g_VertexArray[nCntBeffect1][1].Diffuse = g_aBeffect1[nCntBeffect1].material.Diffuse;
			g_VertexArray[nCntBeffect1][2].Diffuse = g_aBeffect1[nCntBeffect1].material.Diffuse;
			g_VertexArray[nCntBeffect1][3].Diffuse = g_aBeffect1[nCntBeffect1].material.Diffuse;
			g_VertexArray[nCntBeffect1][4].Diffuse = g_aBeffect1[nCntBeffect1].material.Diffuse;
			g_VertexArray[nCntBeffect1][5].Diffuse = g_aBeffect1[nCntBeffect1].material.Diffuse;



			//if (g_VertexBuffer)			g_VertexBuffer->Release();
			//MakeVertexBeffect1();

			nIdxBeffect1 = nCntBeffect1;


			break;
		}
	}

	return nIdxBeffect1;
}



//void MakeVertexBeffect1(void)
//{
//	// 頂点バッファ生成
//	D3D11_BUFFER_DESC bd;
//	ZeroMemory(&bd, sizeof(bd));
//	bd.Usage = D3D11_USAGE_DYNAMIC;;
//	bd.ByteWidth = sizeof(VERTEX_3D) * 4 * MAX_POLYGON;
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//
//	// グローバルの頂点配列から頂点バッファをつくる
//	D3D11_SUBRESOURCE_DATA sd;
//	ZeroMemory(&sd, sizeof(sd));
//	sd.pSysMem = g_VertexArray;
//
//	GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);
//
//	return;
//}


//void SetBombEffect1(XMFLOAT3 pos)
//{
//
//	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;
//
//	for (int i = 0; i < BOMB_POP; i++)
//	{
//
//
//
//
//		XMFLOAT2 moveangle;
//
//		moveangle.x = (rand() % 618) * 0.01f;
//		moveangle.y = (rand() % 618) * 0.01f;
//
//		XMFLOAT3 move;
//
//		move.x = sinf(moveangle.y) * cosf(moveangle.x) * BOMB_MOVE;
//		move.z = cosf(moveangle.y) * cosf(moveangle.x) * BOMB_MOVE;
//		move.y = sinf(moveangle.x) * BOMB_MOVE;
//
//
//
//		float fSize = BOMB_SIZE;
//		SetBeffect1(pos, move, XMFLOAT4(0.1f, 0.1f, 0.1f, 0.5f), fSize, fSize, BOMB_LIFE + (rand() % BOMB_LIFE), 2);
//
//
//
//
//	}
//
//
//}