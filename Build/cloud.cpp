//=============================================================================
//
// パーティクル処理 [cloud.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "light.h"
#include "cloud.h"
#include "player.h"
#include"renderer.h"
#include"debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)			// テクスチャの数

#define	CLOUD_SIZE_X		(20000.0f)		// 頂点サイズ
#define	CLOUD_SIZE_Y		(20000.0f)		// 頂点サイズ
#define	VALUE_MOVE_CLOUD	(5.0f)		// 移動速度

#define	MAX_CLOUD_PART		(16348)		// パーティクル最大数
#define MAX_CLOUD			(100)
#define MAX_POLYGON			(MAX_CLOUD_PART*2)
#define MAX_VERTEX			(MAX_POLYGON*6)
#define CLOUD_SIZE			(10.0f)
#define POLYGON_VERTEX		(6)
#define CLOUD_ALPHA			(0.2f)
#define MAX_MPOINT			(32)//2の倍数
#define MPOINT_SPLIT		(5)
#define CLOUD_HIGHT			(3500)
#define CLOUD_PART_DIST		(20000.0f)


//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexCloud(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;		// 頂点バッファ
// 頂点配列
static VERTEX_3D g_VertexArray[MAX_CLOUD_PART][POLYGON_VERTEX];



static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;					// テクスチャ番号

static CLOUD					g_aCloud[MAX_CLOUD_PART];		// パーティクルワーク
static XMFLOAT3					g_posBase;						// ビルボード発生位置
static float					g_fWidthBase = 5.0f;			// 基準の幅
static float					g_fHeightBase = 10.0f;			// 基準の高さ
static float					g_roty = 0.0f;					// 移動方向
static float					g_spd = 0.0f;					// 移動スピード
static int						g_partcnt = 0;

static XMFLOAT3 g_vpos[4];

//x		//z
static float hight[MAX_MPOINT + 1][MAX_MPOINT + 1][2];

static char *g_TextureName[] =
{
	//"data/TEXTURE/effect000.jpg",
	//"data/TEXTURE/siro.png",
	"data/TEXTURE/particle_smoke.png",
	//"data/TEXTURE/cloud_sakura.png",
};

static BOOL							g_Load = FALSE;



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCloud(void)
{

	//// 頂点情報の作成
	//MakeVertexCloud();

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
	for(int nCntCloud = 0; nCntCloud < MAX_CLOUD_PART; nCntCloud++)
	{
		g_aCloud[nCntCloud].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aCloud[nCntCloud].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aCloud[nCntCloud].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aCloud[nCntCloud].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_aCloud[nCntCloud].material, sizeof(g_aCloud[nCntCloud].material));
		g_aCloud[nCntCloud].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_aCloud[nCntCloud].fSizeX = CLOUD_SIZE_X;
		g_aCloud[nCntCloud].fSizeY = CLOUD_SIZE_Y;
		g_aCloud[nCntCloud].nLife = 0;
		g_aCloud[nCntCloud].use = FALSE;
		g_aCloud[nCntCloud].type = 1;



		for (int i = 0; i < 4; i++)
		{
			g_vpos[i] = g_aCloud[nCntCloud].pos;
		}

		g_vpos[0].x -= CLOUD_SIZE_X / 2;
		g_vpos[0].y += CLOUD_SIZE_Y / 2;


		g_vpos[1].x += CLOUD_SIZE_X / 2;
		g_vpos[1].y += CLOUD_SIZE_Y / 2;


		g_vpos[2].x -= CLOUD_SIZE_X / 2;
		g_vpos[2].y -= CLOUD_SIZE_Y / 2;


		g_vpos[3].x += CLOUD_SIZE_X / 2;
		g_vpos[3].y -= CLOUD_SIZE_Y / 2;



		g_VertexArray[nCntCloud][0] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
		g_VertexArray[nCntCloud][1] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
		g_VertexArray[nCntCloud][2] = { g_vpos[1], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) };
		g_VertexArray[nCntCloud][3] = { g_vpos[2], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) };
		g_VertexArray[nCntCloud][4] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };
		g_VertexArray[nCntCloud][5] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };


		

	}

	for (int x = 0; x < MAX_MPOINT+1; x++)
	{

		for (int z = 0; z < MAX_MPOINT+1; z++)
		{
			hight[x][z][0] = -10.0f;
			hight[x][z][1] = -10.0f;
		}
	}


	XMFLOAT3 clopos = XMFLOAT3(-200000.0f, 50000.0f, 0.0f);
	InitRand(1000, FALSE);
	SetCloud(clopos, 1.0f, 1.0f, 1.0f);



	g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_roty = 0.0f;
	g_spd = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCloud(void)
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

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCloud(void)
{

		XMMATRIX mtxWorld;

		//mtxWorld = XMMatrixIdentity();
		CAMERA* cam = GetCamera();

		XMFLOAT3 vpos[4];

		vpos[0].x = -CLOUD_SIZE_X / 2;
		vpos[0].y = +CLOUD_SIZE_Y / 2;
		vpos[0].z = 0.0f;

		vpos[1].x = +CLOUD_SIZE_X / 2;
		vpos[1].y = +CLOUD_SIZE_Y / 2;
		vpos[1].z = 0.0f;

		vpos[2].x = -CLOUD_SIZE_X / 2;
		vpos[2].y = -CLOUD_SIZE_Y / 2;
		vpos[2].z = 0.0f;

		vpos[3].x = +CLOUD_SIZE_X / 2;
		vpos[3].y = -CLOUD_SIZE_Y / 2;
		vpos[3].z = 0.0f;

		XMVECTOR vvec[4];


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

				vvec[i] = axy * CLOUD_SIZE_Y;
				break;
			case 1:
				vvec[i] = axx * CLOUD_SIZE_X;
				break;


			case 2:


				vvec[i] = axx * -CLOUD_SIZE_X;
				break;

			case 3:

				vvec[i] = axy * -CLOUD_SIZE_Y;
				break;


			default:
				break;
			}

			//XMVector3Rotate(vvec[i],cam->qtonrot);
			XMStoreFloat3(&vpos[i], vvec[i]);

		}


		for(int nCntCloud = 0; nCntCloud < MAX_CLOUD_PART; nCntCloud++)
		{
			if(g_aCloud[nCntCloud].use)
			{// 使用中








				for (int i = 0; i < 4; i++)
				{
					g_vpos[i] = g_aCloud[nCntCloud].pos;
				}



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





				g_VertexArray[nCntCloud][0] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(0.0f, 0.0f) };
				g_VertexArray[nCntCloud][1] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(0.0f, 0.0f) };
				g_VertexArray[nCntCloud][2] = { g_vpos[1], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(1.0f, 0.0f) };
				g_VertexArray[nCntCloud][3] = { g_vpos[2], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(0.0f, 1.0f) };
				g_VertexArray[nCntCloud][4] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(1.0f, 1.0f) };
				g_VertexArray[nCntCloud][5] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(1.0f, 1.0f) };

			}
			else
			{
				g_VertexArray[nCntCloud][0] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
				g_VertexArray[nCntCloud][1] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
				g_VertexArray[nCntCloud][2] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) };
				g_VertexArray[nCntCloud][3] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) };
				g_VertexArray[nCntCloud][4] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };
				g_VertexArray[nCntCloud][5] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };



			}


		}




		if (g_VertexBuffer)			g_VertexBuffer->Release();
		MakeVertexCloud();




}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCloud(void)
{
	//XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	//CAMERA* cam = GetCamera();

	

	SetCullingMode(CULL_MODE_FRONT);

	// ライティングを無効に
	SetLightEnable(FALSE);

	// 加算合成に設定
	SetBlendState(BLEND_MODE_ADD);

	// Z比較有効
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
	material.Diffuse = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	CAMERA* cam = GetCamera();


	XMMATRIX mtxWorld, mtxView;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// ビューマトリックスを取得
	mtxView = XMLoadFloat4x4(&cam->mtxView);







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
HRESULT MakeVertexCloud(void)
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
void SetColorCloud(int nIdxCloud, XMFLOAT4 col)
{
	g_aCloud[nIdxCloud].material.Diffuse = col;
}

//=============================================================================
// 雲の発生処理
//=============================================================================
void SetCloud(
	XMFLOAT3 pos,		//雲の中心座標


	float fSizeX,		//雲のサイズ
	float fSizeY,
	float fSizeZ		
)
{

	int x1 = 0;
	int x2 = MAX_MPOINT;
	int z1 = 0;
	int z2 = MAX_MPOINT;

	SetCloudHight1(x1,x2,z1,z2, 0, hight[0][MAX_MPOINT][1], hight[MAX_MPOINT][MAX_MPOINT][1],hight[0][0][1],hight[MAX_MPOINT][0][1]);

	

	int nIdxCloud = -1;

	for (int x = 0; x < MAX_MPOINT+1; x++)
	{
		for (int z = 0; z <MAX_MPOINT+1; z++)
		{

			hight[x][z][1] -= 5.0f;
			int yleng =(int)( hight[x][z][1]);

			for (int y = 0; y < yleng; y++)
			{
				for (int nCntCloud = 0; nCntCloud < MAX_CLOUD_PART; nCntCloud++)
				{
					if (!g_aCloud[nCntCloud].use)
					{


						{
							g_aCloud[nCntCloud].pos.x = x * CLOUD_PART_DIST + pos.x + (float)(rand() % 10-5) * 0.1f* CLOUD_PART_DIST;
							g_aCloud[nCntCloud].pos.z = z * CLOUD_PART_DIST + pos.z + (float)(rand() % 10-5) * 0.1f*CLOUD_PART_DIST;
							//g_aCloud[nCntCloud].pos.y = pos.y + hight[x][z][1]/ 2;
							g_aCloud[nCntCloud].pos.y = pos.y +( hight[x][z][1] - hight[x][z][1] / 2) + ((hight[x][z][1]* CLOUD_PART_DIST) / yleng) * y - hight[x][z][1] / 2 + (float)(rand() % 10 - 5) * 0.1f* CLOUD_PART_DIST-yleng*CLOUD_HIGHT;
							//g_aCloud[nCntCloud].pos.y -= (float)((rand()%yleng)/2)*(float)(CLOUD_HIGHT);
							g_aCloud[nCntCloud].rot = { 0.0f, 0.0f, 0.0f };
							g_aCloud[nCntCloud].scale = { 1.0f, 1.0f, 1.0f };
							g_aCloud[nCntCloud].fSizeX = 1.0f;
							g_aCloud[nCntCloud].fSizeY = 1.0f;
							g_aCloud[nCntCloud].use = TRUE;
							g_aCloud[nCntCloud].material.Diffuse = { 1.0f,1.0f,1.0f,CLOUD_ALPHA };

							g_partcnt += 1;
							for (int i = 0; i < 4; i++)
							{
								g_vpos[i] = g_aCloud[nCntCloud].pos;
							}




							nIdxCloud = nCntCloud;
						}

						break;
					}
				}



			}


		}

	}


	return ;
}












void SetCloudHight1(int xl,int xr,int zd,int zu, int n, float tl,float tr,float bl,float br)
{
	if (n == MPOINT_SPLIT)
	{
		return;
	}
	//平均
	float top = (tl + tr) / 2;
	float bottom = (bl + br) / 2;
	float left = (tl+ bl) / 2;
	float right = (tr + br) / 2 ;
	float midpoint = (tl + tr + bl + br) / 4+GetRandomHightPM(n);
	
	//float top = (tl + tr) / 2 + GetRandomHightPM(n+1);
	//float bottom = (bl + br) / 2 + GetRandomHightPM(n+1);
	//float left = (tl+ bl) / 2 + GetRandomHightPM(n+1);
	//float right = (tr + br) / 2 + GetRandomHightPM(n+1);
	//float midpoint = (tl + tr + bl + br) / 4+GetRandomHightPM(n);
	//
	hight[(xl + xr) / 2][zu][1] = top;
	hight[(xl + xr) / 2][zd][1] = bottom;
	hight[xl][(zu + zd) / 2][1] = left;
	hight[xr][(zu + zd) / 2][1] = right;
	hight[(xl + xr) / 2][(zu + zd) / 2][1] = midpoint;


	SetCloudHight1(xl, (xl + xr) / 2, (zu + zd) / 2, zu, n + 1, tl, top, left, midpoint);
	SetCloudHight1((xl + xr) / 2, xr, (zu + zd) / 2, zu, n + 1, top, tr, midpoint, right);
	SetCloudHight1(xl, (xl + xr) / 2, zd, (zu + zd) / 2, n + 1, left, midpoint, bl, bottom);
	SetCloudHight1((xl + xr) / 2, xr, zd, (zu + zd) / 2, n + 1, midpoint, right, bottom, br);
}
void SetCloudHight2(int xl,int xr,int zd,int zu, int n, float tl,float tr,float bl,float br)
{
	if (n == MPOINT_SPLIT)
	{
		return;
	}
	//平均
	float top = (tl + tr) / 2;
	float bottom = (bl + br) / 2;
	float left = (tl+ bl) / 2;
	float right = (tr + br) / 2 ;
	float midpoint = (tl + tr + bl + br) / 4+GetRandomHightPM(n);
	
	//float top = (tl + tr) / 2 + GetRandomHightPM(n+1);
	//float bottom = (bl + br) / 2 + GetRandomHightPM(n+1);
	//float left = (tl+ bl) / 2 + GetRandomHightPM(n+1);
	//float right = (tr + br) / 2 + GetRandomHightPM(n+1);
	//float midpoint = (tl + tr + bl + br) / 4+GetRandomHightPM(n);
	//
	hight[(xl + xr) / 2][zu][1] = top;
	hight[(xl + xr) / 2][zd][1] = bottom;
	hight[xl][(zu + zd) / 2][1] = left;
	hight[xr][(zu + zd) / 2][1] = right;
	hight[(xl + xr) / 2][(zu + zd) / 2][1] = midpoint;


	SetCloudHight2(xl, (xl + xr) / 2, (zu + zd) / 2, zu, n + 1, tl, top, left, midpoint);
	SetCloudHight2((xl + xr) / 2, xr, (zu + zd) / 2, zu, n + 1, top, tr, midpoint, right);
	SetCloudHight2(xl, (xl + xr) / 2, zd, (zu + zd) / 2, n + 1, left, midpoint, bl, bottom);
	SetCloudHight2((xl + xr) / 2, xr, zd, (zu + zd) / 2, n + 1, midpoint, right, bottom, br);
}
float GetRandomHightPM(int n)
{
	float h = 0.0f;
	h = (rand() % (int)(CLOUD_HIGHT / powf(2, n))) * 0.01f;
	//h = (((rand() % (int)(CLOUD_HIGHT / powf(2, n)))- ((CLOUD_HIGHT / powf(2, n)))/2)) * 0.01f;

	return h;
}
float GetRandomHightP(int n)
{
	float h = 0.0f;
	h = (rand() % (int)(CLOUD_HIGHT / powf(2, n))) * 0.01f;
	//h = (((rand() % (int)(CLOUD_HIGHT / powf(2, n)))- ((CLOUD_HIGHT / powf(2, n)))/2)) * 0.01f;

	return h;
}

