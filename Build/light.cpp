//=============================================================================
//
// ライト処理 [light.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include"player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


#define	SUN_H	(1000.0f)
#define SUN_ROT	(XM_PI/18000000)
//#define SUN_ROT	(0)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];

static FOG		g_Fog;

static BOOL		g_FogEnable = false;


static BOOL		g_ShadowEnable = false;


static SUN		g_Sun;

//=============================================================================
// 初期化処理
//=============================================================================
void InitLight(void)
{
	PLAYER *player = GetPlayer();

	g_Sun.pos = XMFLOAT3(0.0f, SUN_H, 0.0f);
	g_Sun.Opos = player->pos;
	g_Sun.up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	g_Sun.Direction = XMLoadFloat3(&g_Sun.pos);
	g_Sun.Direction = XMLoadFloat3(&g_Sun.Opos) - g_Sun.Direction;
	g_Sun.Direction = XMVector3Normalize(g_Sun.Direction);
	g_Sun.upvec = XMLoadFloat3(&g_Sun.up);
	XMFLOAT3 vec = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_Sun.rvec = XMVector3Normalize(XMLoadFloat3(&vec));
	//g_Sun.rvec = zonevec();
	g_Sun.rot = 0.0f;
		//ライト初期化
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		g_Light[i].Direction = XMFLOAT3( 0.0f, 0.0f, -1.0f );
		g_Light[i].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		g_Light[i].Ambient   = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		g_Light[i].Attenuation = 100.0f;	// 減衰距離
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ライトのタイプ
		g_Light[i].Enable = false;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	// 並行光源の設定（世界を照らす光）
	g_Light[0].Direction = XMFLOAT3(g_Sun.Direction.m128_f32[0], g_Sun.Direction.m128_f32[1], g_Sun.Direction.m128_f32[2]);		// 光の向き
	//g_Light[0].Direction = XMFLOAT3( player[0].pos.x-300.0f, player[0].pos.y -300.0f, player[0].pos.z+300.0f );		// 光の向き
	g_Light[0].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );	// 光の色
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;					// 並行光源
	g_Light[0].Enable = true;									// このライトをON
	SetLight(0, &g_Light[0]);									// これで設定している



	// ポイントライトの設定（豆電球）
	g_Light[1].Position = XMFLOAT3(0.0f, 0.0f, 100.0f);

	g_Light[1].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );	// 光の色
	g_Light[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_Light[1].Attenuation = 40.0f;	// 減衰距離
	g_Light[1].Type = LIGHT_TYPE_POINT;					// 並行光源
	g_Light[1].Enable = true;									// このライトをON
	SetLight(1, &g_Light[1]);									// これで設定している



	// フォグの初期化（霧の効果）
	g_Fog.FogStart = 100.0f;									// 視点からこの距離離れるとフォグがかかり始める
	g_Fog.FogEnd   = 2500000.0f;									// ここまで離れるとフォグの色で見えなくなる
	g_Fog.FogColor = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );		// フォグの色
	SetFog(&g_Fog);
	SetFogEnable(g_FogEnable);				// 他の場所もチェックする shadow


	//// 影の初期化
	//g_Shadow.ShadowColor = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );		// 影の色
	//SetShadow(&g_Shadow);
	//SetShadowEnable(g_ShadowEnable);				// 他の場所もチェックする shadow



}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateLight(void)
{

	SetSunPos(XMFLOAT3(0.0f, 1.0f, 0.0f));



	g_Light[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// 光の色
	g_Light[0].Enable = TRUE;									// このライトをON

	PLAYER *player = GetPlayer();
	
	XMVECTOR qton = XMQuaternionIdentity();
	XMMATRIX mtxrot;

	//回転軸のベクトルでrot分回転するクォータニオンを求める
	qton = XMQuaternionRotationAxis(g_Sun.rvec, SUN_ROT);
	//クォータニオンを回転行列に変換する
	mtxrot = XMMatrixRotationQuaternion(qton);

	g_Sun.Direction = XMVector3Rotate(g_Sun.Direction, qton);

	g_Sun.upvec = XMVector3Rotate(g_Sun.upvec, qton);
	XMStoreFloat3(&g_Sun.up,g_Sun.upvec);



	g_Sun.pos.x = -g_Sun.Direction.m128_f32[0] * SUN_H;
	g_Sun.pos.y = -g_Sun.Direction.m128_f32[1] * SUN_H;
	g_Sun.pos.z = -g_Sun.Direction.m128_f32[2] * SUN_H;

	g_Sun.SunView= XMMatrixLookToLH(XMLoadFloat3(&g_Sun.pos), g_Sun.Direction, XMLoadFloat3(&g_Sun.up));




	 //並行光源の設定（世界を照らす光）
	g_Light[0].Direction = XMFLOAT3(g_Sun.Direction.m128_f32[0], g_Sun.Direction.m128_f32[1], g_Sun.Direction.m128_f32[2]);		// 光の向き



	//if (g_Sun.pos.y < 0.0f)
	//{
	//	SetShadowEnable(FALSE);
	//	g_Light[0].Enable = FALSE;

	//}
	//else
	//{
	//	SetShadowEnable(TRUE);
	//	g_Light[0].Enable = TRUE;

	//}
	SetLight(0, &g_Light[0]);									// これで設定している


}


//=============================================================================
// ライトの設定
// Typeによってセットするメンバー変数が変わってくる
//=============================================================================
void SetLightData(int index, LIGHT *light)
{
	SetLight(index, light);
}


LIGHT *GetLightData(int index)
{
	return(&g_Light[index]);
}


//=============================================================================
// フォグの設定
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}


BOOL	GetFogEnable(void)
{
	return(g_FogEnable);
}


//=============================================================================
// 影の設定
//=============================================================================




SUN GetSun(void)
{
	return g_Sun;
}
XMMATRIX GetSunView(void)
{
	return g_Sun.SunView;
}






void SetSunPos(XMFLOAT3 pos)
{

	XMVECTOR sunv = XMLoadFloat3(&pos);
	XMVector3Normalize(sunv);
	
	XMFLOAT3 vec;
	XMStoreFloat3(&vec, sunv);

	g_Sun.pos.x = vec.x * SUN_H;
	g_Sun.pos.y = vec.y * SUN_H;
	g_Sun.pos.z = vec.z * SUN_H;

	g_Sun.Direction = XMLoadFloat3(&g_Sun.pos);
	g_Sun.Direction = XMLoadFloat3(&g_Sun.Opos) - g_Sun.Direction;
	g_Sun.Direction = XMVector3Normalize(g_Sun.Direction);


	return;
}