//=============================================================================
//
// メッシュの処理 [meshdata.h]
// Author :三ツ寺貴紀
//
//=============================================================================
#pragma once
#include "CoreMinimal.h"
#include "renderer.h"
#include "meshdata.h"


//*********************************************************
//マクロ定義
//*********************************************************



//*********************************************************
//クラス定義
//*********************************************************
 class FrameMtxList
 {

	 
	 XMMATRIX* meshframemtx;//メッシュ数で配列確保
 public:
	 FrameMtxList();
	 ~FrameMtxList();

	 void CreateMeshMtxArray(int n);
	 void SetMeshFrameMtx(int meshN,XMMATRIX mtx);
	 XMMATRIX GetMeshFrameMtx(int meshN);

 private:

 };


 class KeyFrameAnimData
 {
	 int framenum;
	 int meshnum;
	 FrameMtxList* framemtxlist;//フレーム毎の各メッシュの行列のリストフレーム数で配列確保
	 string filepath;

 public:
	 KeyFrameAnimData();
	 ~KeyFrameAnimData();

	 int GetFrameNum(void);

	 int GetMeshNum(void);

	 void CreateFrameMeshMtxListArray(int frameN,int meshN);

	 void SetFrameMeshMtx(int frameN, int meshN, XMMATRIX mtx);
	 XMMATRIX GetFrameMeshMtx(int frameN, int meshN);


	 string GetFilePath(void);
	 

	 void LoadKeyFrameAnim(string fpath);


 private:

 };

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

