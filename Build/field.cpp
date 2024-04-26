//=============================================================================
//
// メッシュ地面の処理 [meshfield.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "field.h"
#include "renderer.h"
#include"debugproc.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX		(1)				// テクスチャの数
#define MAX_WIDTH		(256)
#define MAX_HIGHT		(256)
#define FIELD_HIGHT		(1000000)
#define SEED			(10)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11Buffer* g_IndexBuffer = NULL;	// インデックスバッファ

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;				// テクスチャ番号

static XMFLOAT3		g_posField;								// ポリゴン表示位置の中心座標
static XMFLOAT3		g_rotField;								// ポリゴンの回転角

//static int			MAX_WIDTH, MAX_HIGHT;	// ブロック数
//static int			g_nNumBlockXField, g_nNumBlockZField;	// ブロック数

static int			g_nNumVertexField;						// 総頂点数	
static int			g_nNumVertexIndexField;					// 総インデックス数
static int			g_nNumPolygonField;						// 総ポリゴン数
static float		g_fBlockSizeXField, g_fBlockSizeZField;	// ブロックサイズ

static char* g_TextureName[] = {
	"data/TEXTURE/rough_sand_texture.jpg",
	"data/TEXTURE/rough_sand_texture.jpg",
};



static VERTEX_3D  g_VertexArray[MAX_WIDTH][MAX_HIGHT];

// 波の高さ = sin( -経過時間 * 周波数 + 距離 * 距離補正 ) * 振幅
static XMFLOAT3		g_Center;					// 波の発生場所
static float		g_Time = 0.0f;				// 波の経過時間
static float		g_wave_frequency = 2.0f;	// 波の周波数
static float		g_wave_correction = 0.02f;	// 波の距離補正
static float		g_wave_amplitude = 20.0f;	// 波の振幅


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField(XMFLOAT3 pos, XMFLOAT3 rot,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	// ポリゴン表示位置の中心座標を設定
	g_posField = pos;
	g_posField.x -= 256*0.5 * fBlockSizeX;
	g_posField.z -= 256*0.5 * fBlockSizeZ;

	g_rotField = rot;
	g_fBlockSizeXField = fBlockSizeX;
	g_fBlockSizeZField = fBlockSizeZ;

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

	//// ブロック数の設定
	//MAX_WIDTH = MAX_WIDTH;
	//MAX_HIGHT = MAX_HIGHT;

	// 頂点数の設定
	g_nNumVertexField = (MAX_WIDTH + 1) * (MAX_HIGHT + 1);

	// インデックス数の設定
	g_nNumVertexIndexField = (MAX_WIDTH + 1) * 2 * MAX_HIGHT + (MAX_HIGHT - 1) * 2;

	// ポリゴン数の設定
	g_nNumPolygonField = MAX_WIDTH * MAX_HIGHT * 2 + (MAX_HIGHT - 1) * 4;

	// ブロックサイズの設定
	g_fBlockSizeXField = fBlockSizeX;
	g_fBlockSizeZField = fBlockSizeZ;


	// 頂点情報をメモリに作っておく（波の為）
	// 波の処理
	// 波の高さ = sin( -経過時間 * 周波数 + 距離 * 距離補正 ) * 振幅
	//g_Vertex = new VERTEX_3D[MAX_WIDTH][MAX_HIGHT];
	g_Center = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 波の発生場所
	g_Time = 0.0f;								// 波の経過時間(＋とーとで内側外側になる)
	g_wave_frequency = 1.0f;					// 波の周波数（上下運動の速さ）
	g_wave_correction = 0.02f;					// 波の距離補正（変えなくても良いと思う）
	g_wave_amplitude = 30.0f;					// 波の振幅(波の高さ)

	//for (int z = 0; z < (MAX_HIGHT + 1); z++)
	//{
	//	for (int x = 0; x < (MAX_WIDTH + 1); x++)
	//	{
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].Position.x = -(MAX_WIDTH / 2.0f) * g_fBlockSizeXField + x * g_fBlockSizeXField;
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].Position.y = 0.0f;
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].Position.z = (MAX_HIGHT / 2.0f) * g_fBlockSizeZField - z * g_fBlockSizeZField;

	//		float dx = g_Vertex[z * (MAX_WIDTH + 1) + x].Position.x - g_Center.x;
	//		float dz = g_Vertex[z * (MAX_WIDTH + 1) + x].Position.z - g_Center.z;

	//		// 波紋の中心点からの距離を得る
	//		float len = (float)sqrtf(dx * dx + dz * dz);

	//		// 波の高さを、sin関数で得る
	//		// 波の高さ　= sin( -経過時間 * 周波数 + 距離 * 距離補正 ) * 振幅
	//		//g_Vertex[z * (MAX_WIDTH + 1) + x].Position.y = sinf(-g_Time * g_wave_frequency + len * g_wave_correction) * g_wave_amplitude;


	//		// 法線の設定
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].Normal = XMFLOAT3(0.0f, 1.0, 0.0f);

	//		// 反射光の設定
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//		// テクスチャ座標の設定
	//		float texSizeX = 1.0f;
	//		float texSizeZ = 1.0f;
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].TexCoord.x = texSizeX * x;
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].TexCoord.y = texSizeZ * z;
	//	}

	//}


	for (int x = 0; x < MAX_WIDTH; x++)
	{
		for (int z = 0; z < MAX_HIGHT; z++)
		{
			g_VertexArray[x][z].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
			g_VertexArray[x][z].Normal = { 0.0f,1.0f,0.0f };
			g_VertexArray[x][z].TexCoord = { 1.0f * x,1.0f * z };
			g_VertexArray[x][z].Position = { g_fBlockSizeXField *x,0.0f,g_fBlockSizeZField *z };
		}
	}
	int x1 = 0;
	int x2 = MAX_WIDTH-1;
	int z1 = 0;
	int z2 = MAX_HIGHT;



	//中点変位法による地形生成
	 
	InitRand(SEED, FALSE);
	g_VertexArray[0][0].Position.y = GetRandomFieldHightPM(0);
	g_VertexArray[MAX_WIDTH - 1][0].Position.y = GetRandomFieldHightPM(0);
	g_VertexArray[0][MAX_HIGHT - 1].Position.y = GetRandomFieldHightPM(0);
	g_VertexArray[MAX_WIDTH - 1][MAX_HIGHT-1].Position.y = GetRandomFieldHightPM(0);
	SetFieldHight(x1, x2, z1, z2, 0, g_VertexArray[0][0].Position.y, g_VertexArray[MAX_WIDTH - 1][0].Position.y, g_VertexArray[0][MAX_HIGHT - 1].Position.y, g_VertexArray[MAX_WIDTH - 1][MAX_HIGHT - 1].Position.y);


	InitRand(0, TRUE);


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * MAX_WIDTH * MAX_HIGHT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * g_nNumVertexIndexField;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_IndexBuffer);


	{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, g_VertexArray, sizeof(VERTEX_3D) * MAX_WIDTH*MAX_HIGHT);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short* pIdx = (unsigned short*)msr.pData;


		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < MAX_HIGHT-1; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < MAX_WIDTH; nCntVtxX++)
			{
				pIdx[nCntIdx] = nCntVtxX  * (MAX_HIGHT) + nCntVtxZ;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxX * (MAX_HIGHT) + nCntVtxZ+1;
				nCntIdx++;
			}

			if (nCntVtxZ < (MAX_HIGHT-2 ))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = pIdx[nCntIdx-1];
				nCntIdx++;
			}



		}

		GetDeviceContext()->Unmap(g_IndexBuffer, 0);


		g_nNumVertexIndexField = nCntIdx;


	}




	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
	// インデックスバッファの解放
	if (g_IndexBuffer) {
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
	}

	// 頂点バッファの解放
	if (g_VertexBuffer) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
{
	// 波の処理
	float dt = 0.03f;

	//for (int z = 0; z < MAX_HIGHT; z++)
	//{
	//	for (int x = 0; x < MAX_WIDTH; x++)
	//	{
	//		float dx = g_Vertex[z * (MAX_WIDTH + 1) + x].Position.x - g_Center.x;
	//		float dz = g_Vertex[z * (MAX_WIDTH + 1) + x].Position.z - g_Center.z;

	//		// 波紋の中心点からの距離を得る
	//		float len = (float)sqrtf(dx * dx + dz * dz);

	//		// 波の高さを、sin関数で得る
	//		//  　波の高さ　= sin( -経過時間 * 周波数 + 距離 * 距離補正 ) * 振幅
	//	//	g_Vertex[z * (MAX_WIDTH + 1) + x].Position.y = sinf(-g_Time * g_wave_frequency + len * g_wave_correction) * g_wave_amplitude;
	//	}

	//}
	//g_Time += dt;


	//// 頂点バッファに値をセットする
	//D3D11_MAPPED_SUBRESOURCE msr;
	//GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	//VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	//// 全頂点情報を毎回上書きしているのはDX11ではこの方が早いからです
	//memcpy(pVtx, g_Vertex, sizeof(VERTEX_3D) * g_nNumVertexField);

	//GetDeviceContext()->Unmap(g_VertexBuffer, 0);



}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	GetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);


	XMMATRIX mtxRot, mtxTranslate, mtxWorld;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_rotField.x, g_rotField.y, g_rotField.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_posField.x,g_posField.y, g_posField.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	//SetCullingMode(CULL_MODE_NONE);
	// ポリゴンの描画
	GetDeviceContext()->DrawIndexed(g_nNumVertexIndexField,0, 0);
	//// ポリゴンの描画
	//GetDeviceContext()->DrawIndexed(14,0, 0);
}





void SetFieldHight(int xl, int xr, int zd, int zu, int n, float tl, float tr, float bl, float br)
{
	if (n == 8)
	{
		return;
	}
	//平均
	float top = (tl + tr) / 2;
	float bottom = (bl + br) / 2;
	float left = (tl + bl) / 2;
	float right = (tr + br) / 2;
	float midpoint = (tl + tr + bl + br) / 4 + GetRandomFieldHightPM(n);

	//float top = (tl + tr) / 2 + GetRandomHightPM(n+1);
	//float bottom = (bl + br) / 2 + GetRandomHightPM(n+1);
	//float left = (tl+ bl) / 2 + GetRandomHightPM(n+1);
	//float right = (tr + br) / 2 + GetRandomHightPM(n+1);
	//float midpoint = (tl + tr + bl + br) / 4+GetRandomHightPM(n);
	////
	g_VertexArray[(xl + xr) / 2][zu].Position.y = top;
	g_VertexArray[(xl + xr) / 2][zd].Position.y = bottom;
	g_VertexArray[xl][(zu + zd) / 2].Position.y = left;
	g_VertexArray[xr][(zu + zd) / 2].Position.y = right;
	g_VertexArray[(xl + xr) / 2][(zu + zd) / 2].Position.y = midpoint;


	SetFieldHight(xl, (xl + xr) / 2, (zu + zd) / 2, zu, n +1, tl, top, left, midpoint);
	SetFieldHight((xl + xr) / 2, xr, (zu + zd) / 2, zu, n +1, top, tr, midpoint, right);
	SetFieldHight(xl, (xl + xr) / 2, zd, (zu + zd) / 2, n +1, left, midpoint, bl, bottom);
	SetFieldHight((xl + xr) / 2, xr, zd, (zu + zd) / 2, n +1, midpoint, right, bottom, br);
}
float GetRandomFieldHightPM(int n)
{
	InitRand(SEED,FALSE);
	float h = 0.0f;
	//h = (float)((rand() % (int)(FIELD_HIGHT / powf(2, n))) * 0.1f);
	int rl = ((LONGLONG)(FIELD_HIGHT / powf(2, n)));

	//h = ((rand() % rl)) * 1.0f;
	h = ((rand() % rl)-(rl /2)) * 1.0f;

	return h;
}

//XMFLOAT3 GetVertexField(int x,int y)
//{
//	return g_VertexArray[x][y].Position;
//}

//BOOL RayHitField(XMFLOAT3 pos, XMFLOAT3* HitPosition, XMFLOAT3* Normal)
//{
//	XMFLOAT3 start = pos;
//	XMFLOAT3 end = pos;
//
//	if (start.x == 0.0f)
//	{
//		start.x += 0.5f;
//		end.x += 0.5f;
//	}
//	if (start.z == 0.0f)
//	{
//		start.z -= 0.5f;
//		end.z -= 0.5f;
//	}
//
//	// 少し上から、ズドーンと下へレイを飛ばす
//	start.y += 1000.0f;
//	end.y -= 1000.0f;
//
//	// 処理を高速化する為に全検索ではなくて、座標からポリゴンを割り出している
//	float fz = (MAX_HIGHT / 2.0f) * g_fBlockSizeXField;
//	float fx = (MAX_WIDTH/ 2.0f) * g_fBlockSizeZField;
//	int sz = (int)((-start.z + fz) / g_fBlockSizeZField);
//	int sx = (int)((start.x + fx) / g_fBlockSizeXField);
//	int ez = sz + 2;
//	int ex = sx + 2;
//
//
//	if ((sz < 0) || (sz > MAX_HIGHT - 1) ||
//		(sx < 0) || (sx > MAX_WIDTH - 1))
//	{
//		*Normal = { 0.0f, 1.0f, 0.0f };
//		return FALSE;
//	}
//
//
//	// 必要数分検索を繰り返す
//	for (int z = sz; z < ez; z++)
//	{
//		for (int x = sx; x < ex; x++)
//		{
//
//			if (x < MAX_WIDTH && z < MAX_HIGHT)
//			{
//
//
//
//				XMFLOAT3 p0 = g_VertexArray[x][z].Position;
//				XMFLOAT3 p1 = g_VertexArray[x + 1][z].Position;
//				XMFLOAT3 p2 = g_VertexArray[x][z + 1].Position;
//				XMFLOAT3 p3 = g_VertexArray[x + 1][z + 1].Position;
//
//				p0.y += g_posField.y;
//				p1.y += g_posField.y;
//				p2.y += g_posField.y;
//				p3.y += g_posField.y;
//
//
//				
//
//
//
//
//				// 三角ポリゴンだから２枚分の当たり判定
//				BOOL ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
//				if (ans)
//				{
//					return TRUE;
//				}
//
//				ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
//				if (ans)
//				{
//					return TRUE;
//				}
//
//			}
//			else
//			{
//
//				return FALSE;
//
//			}
//
//
//		}
//	}
//
//
//	return FALSE;
//}
