//=============================================================================
//
// モデルの処理 [meshdata.cpp]
// Author : 三ツ寺貴紀
//
//=============================================================================
#include "meshanimdata.h"
#include "Coreminimal.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************




//*****************************************************************************
// グローバル変数
//*****************************************************************************

KeyFrameAnimData* g_keyframeanimdata;

int keyframeanimfilenum;

std::vector<std::string> file_names;


//*****************************************************************************
// クラス関数
//*****************************************************************************

FrameMtxList::FrameMtxList()
{
	meshframemtx = nullptr;
}

FrameMtxList::~FrameMtxList()
{
	if (meshframemtx) delete[]meshframemtx;

}
void  FrameMtxList::CreateMeshMtxArray(int n)
{
	meshframemtx = new XMMATRIX[n];
}
void FrameMtxList::SetMeshFrameMtx(int meshN, XMMATRIX mtx)
{
	meshframemtx[meshN] = mtx;
}
XMMATRIX FrameMtxList::GetMeshFrameMtx(int meshN)
{
	XMMATRIX mtx= this->meshframemtx[meshN];
	return mtx;
}

KeyFrameAnimData::KeyFrameAnimData()
{
	framenum = 0;
	meshnum = 0;
	framemtxlist = nullptr;
}

KeyFrameAnimData::~KeyFrameAnimData()
{
	if (framemtxlist)delete[]framemtxlist;
}

int KeyFrameAnimData::GetFrameNum(void)
{
	return framenum;
}

int KeyFrameAnimData::GetMeshNum(void)
{
	return meshnum;
}

void KeyFrameAnimData::CreateFrameMeshMtxListArray(int frameN, int meshN)
{
	framenum = frameN;
	meshnum = meshN;
	framemtxlist = new FrameMtxList[frameN];
	for (int i = 0; i < frameN; i++)
	{
		framemtxlist[i].CreateMeshMtxArray(meshN);
	}

}

void KeyFrameAnimData::SetFrameMeshMtx(int frameN, int meshN, XMMATRIX mtx)
{
	framemtxlist[frameN].SetMeshFrameMtx(meshN, mtx);
}
XMMATRIX KeyFrameAnimData::GetFrameMeshMtx(int frameN, int meshN)
{
	XMMATRIX mtx = framemtxlist[frameN].GetMeshFrameMtx(meshN);
	return mtx;
}

string KeyFrameAnimData::GetFilePath(void)
{
	return filepath;
}



//*****************************************************************************
// fbxファイルからメッシュデータの読み込み
//*****************************************************************************
void KeyFrameAnimData::LoadKeyFrameAnim(string fpath)
{

	this->filepath = fpath;

	FbxManager* manager;
	FbxIOSettings* ioSettings;

	manager = FbxManager::Create();
	ioSettings = FbxIOSettings::Create(manager, IOSROOT);

	

	// Importerを生成
	FbxImporter* importer = FbxImporter::Create(manager, "");

	if (importer->Initialize(filepath.c_str(), -1, manager->GetIOSettings()) == false)
	{
		// インポートエラー
	}


	//SceneオブジェクトにFBXファイル内の情報を流し込む
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);



	int meshcount = scene->GetSrcObjectCount<FbxMesh>();

	//アニメーション名一覧を格納する配列の宣言
	FbxArray<FbxString*> animation_names;

	//シーンからアニメーション一覧を取得＆格納
	scene->FillAnimStackNameArray(animation_names);

	int animcount = animation_names.GetCount();
	int framenum = 0;
	FbxTime start_time = 0;
	FbxTime end_time = 0;
	FbxLongLong oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	FbxTime totalframe = 0;

	if (animcount > 0)
	{
		//アニメ番号0番を元にFbxTakeInfoを取得
		auto take_info = scene->GetTakeInfo(animation_names[0]->Buffer());
		//開始時間
		start_time = take_info->mLocalTimeSpan.GetStart();
		//終了時間
		end_time = take_info->mLocalTimeSpan.GetStop();

		oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

		totalframe = (end_time - start_time) / oneFrameValue;

		framenum = (int)totalframe.Get();//アニメーションの合計フレーム数



	}

	this->CreateFrameMeshMtxListArray(framenum, meshcount);


	for (int i = 0; i < meshcount; i++)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(i);
		FbxNode* node = mesh->GetNode();

		for (int j = 0; j < framenum; j++)
		{
			FbxMatrix framemtx = node->EvaluateGlobalTransform(start_time + oneFrameValue * j);

			XMMATRIX mtx = FbxMatrixConvertToXMMATRIX(framemtx);

			this->SetFrameMeshMtx(j, i, mtx);
		}


	}


	scene->Destroy();
	importer->Destroy(); // シーンを流し込んだらImporterは解放

	manager->Destroy();//fbxファイルの読み込みが終わったらマネージャーは解放する
}


