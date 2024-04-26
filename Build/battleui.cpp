//=============================================================================
//
// タイトル画面処理 [battleui.cpp]
// Author : 
//
//=============================================================================
#include "battleui.h"
#include "input.h"
#include "fade.h"
#include"player.h"
#include"enemy.h"
#include"camera.h"
#include"misairu.h"
#include"sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(100.0f)	
#define TEXTURE_HEIGHT				(100.0f)	// 
#define TEXTURE_MAX					(9)				// テクスチャの数
#define	SPEED_DIGIT					(4)
#define TIME_DIGIT					(5)
#define HP_DIGIT					(3)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/siromaru.png",
	"data/TEXTURE/lockon.png",
	"data/TEXTURE/hit.png",
	"data/TEXTURE/destroy.png",
	"data/TEXTURE/alert.png",
	"data/TEXTURE/namberD.png",
	"data/TEXTURE/stall.png",
	"data/TEXTURE/missile1.png",
	"data/TEXTURE/missile2.png",

};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号


static BOOL						g_Load = FALSE;

static float	effect_dx;
static float	effect_dy;

static HIT		hit;
static DESTROY	destroy;
static ALERT alert;
static SPEED speed;
int stalltex;
int stallcnt;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattleui(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// 変数の初期化
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = XMFLOAT3(g_w/2, g_h/2, 0.0f);
	g_TexNo = 0;

	effect_dx = 100.0f;
	effect_dy = 100.0f;


	hit.use = FALSE;
	hit.texno = 2;
	destroy.use = FALSE;
	destroy.texno = 3;
	alert.use = FALSE;
	alert.texno = 4;
	speed.speed = 0;
	speed.texno = 5;
	stalltex = 6;




	
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattleui(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattleui(void)
{

	//AddTime(GetStage());
	MISAIRU* misairu = GetMisairu();
	PLAYER* player = GetPlayer();
	alert.use = FALSE;
	for (int i = 0; i < MAX_MISAIRU; i++)
	{
		if (misairu[i].use==TRUE&&misairu[i].pore==0)
		{
			alert.use = TRUE;
		}
	}

	if (alert.use == TRUE)
	{
		if (alert.cnt % 10 == 0)
		{
			PlaySound(SOUND_LABEL_SE_alert001);
		}

		alert.cnt += 1;
	}



	if (hit.use == TRUE)
	{
		hit.life -= 1;
		if (hit.life<=0)
		{
			hit.use = FALSE;
		}

	}

	if (destroy.use == TRUE)
	{
		destroy.life -= 1;
		if (destroy.life <= 0)
		{
			destroy.use = FALSE;
		}

	}




	speed.speed = (int)(player->spd * 10.0f);

#ifdef _DEBUG	// デバッグ情報を表示する
	//PrintDebugProc("Player:↑ → ↓ ←　Space\n");

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattleui(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	
	//{
	//	// テクスチャ設定
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

	//	// １枚のポリゴンの頂点とテクスチャ座標を設定
	//	SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	//	// ポリゴン描画
	//	GetDeviceContext()->Draw(4, 0);
	//}




	
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();
	CAMERA* camera = GetCamera();

	XMMATRIX world, view, prj,wvp;
	view = XMLoadFloat4x4(&camera->mtxView);
	prj = XMLoadFloat4x4(&camera->mtxProjection);
	

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use==TRUE)
		{
			enemy[i].pos;
			world = XMMatrixIdentity();
			world = XMMatrixMultiply(world, XMMatrixTranslation(enemy[i].pos.x, enemy[i].pos.y, enemy[i].pos.z));
			wvp = XMMatrixMultiply(world, view);
			wvp = XMMatrixMultiply(wvp, prj);
			XMFLOAT4X4 wvp44;
			XMStoreFloat4x4(&wvp44, wvp);


		}
	}


	if (player->misairustanby[0]==TRUE)
	{



		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y + 50.0f, 100.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f,XMFLOAT4(0.0f,1.0f,0.0f,1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}


	}

	if (player->misairustanby[1]==TRUE)
	{



		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y + 70.0f, 100.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}


	}



	if (hit.use == TRUE)
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[hit.texno]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y + 90.0f, 40.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}
	if (destroy.use == TRUE)
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[destroy.texno]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y + 110.0f, 70.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}
	if (alert.use == TRUE)
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[alert.texno]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y +130.0f, 70.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}

	if (speed.speed <100)
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[stalltex]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X+80.0f, SCREEN_CENTER_Y +20.0f, 50.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}





	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[speed.texno]);
	for (int i = 0; i < SPEED_DIGIT; i++)
	{
		// 今回表示する桁の数字
		float x = (float)(speed.speed % 10);

		// スコアの位置やテクスチャー座標を反映
		float px = SCREEN_CENTER_X+100.0f - 10.0f * i - i * 5.0f;	// スコアの表示位置X
		float py = SCREEN_CENTER_Y;			// スコアの表示位置Y
		float pw = 10.0f;				// スコアの表示幅
		float ph = 15.0f;				// スコアの表示高さ

		float tw = 1.0f / 10;		// テクスチャの幅
		float th = 1.0f / 1;		// テクスチャの高さ
		float tx = x * tw;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// 次の桁へ
		speed.speed /= 10;
	}

	// テクスチャ設定
	int n = player->hp;
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[speed.texno]);
	for (int i = 0; i < HP_DIGIT; i++)
	{
		// 今回表示する桁の数字
		float x = (float)(n % 10);

		// スコアの位置やテクスチャー座標を反映
		float px = SCREEN_CENTER_X-100.0f - 10.0f * i - i * 5.0f;	// スコアの表示位置X
		float py = SCREEN_CENTER_Y;			// スコアの表示位置Y
		float pw = 10.0f;				// スコアの表示幅
		float ph = 15.0f;				// スコアの表示高さ

		float tw = 1.0f / 10;		// テクスチャの幅
		float th = 1.0f / 1;		// テクスチャの高さ
		float tx = x * tw;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// 次の桁へ
		n /= 10;
	}

	LONG num = GetStageTime(GetStage());
	for (int i = 0; i < TIME_DIGIT; i++)
	{
		// 今回表示する桁の数字
		float x = (float)(num % 10);

		// スコアの位置やテクスチャー座標を反映
		float px = SCREEN_WIDTH-50.0f - 20.0f * i - i * 5.0f;	// スコアの表示位置X
		float py = 30.0f;			// スコアの表示位置Y
		float pw = 20.0f;				// スコアの表示幅
		float ph = 30.0f;				// スコアの表示高さ

		float tw = 1.0f / 10;		// テクスチャの幅
		float th = 1.0f / 1;		// テクスチャの高さ
		float tx = x * tw;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// 次の桁へ
		num /= 10;
	}



}


void SetHit(void)
{
	hit.use = TRUE;
	hit.life = 30;
}

void SetDestroy(void)
{
	destroy.use = TRUE;
	destroy.life = 30;
}



