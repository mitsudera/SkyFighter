#pragma once
#include "meshcomponent.h"
#include "AssetsManager.h"
#include "MeshData.h"
#include "MeshAnimData.h"
#include "gameobject.h"
MeshComponent::MeshComponent()
{
	
	BlendMeshMtxArray = nullptr;
	MeshMtxArray = nullptr;
	animation = FALSE;
	animindex = 0;
	animstate = ANIM_STATE::NO_ANIM;
	blendcnt = 0;
	blendcntmax = 0;
	framecnt = 0;
	framenum = 0;
	lastanimindex = 0;
	meshNum = 0;
	MeshDataListIndex = 0;
	cullMode = CULL_MODE::CULL_MODE_FRONT;

	motionblend = FALSE;


}

MeshComponent::MeshComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;

	BlendMeshMtxArray = nullptr;
	MeshMtxArray = nullptr;
	animation = FALSE;
	animindex = 0;
	animstate = ANIM_STATE::NO_ANIM;
	blendcnt = 0;
	blendcntmax = 0;
	framecnt = 0;
	framenum = 0;
	lastanimindex = 0;
	meshNum = 0;
	MeshDataListIndex = 0;
	cullMode = CULL_MODE::CULL_MODE_FRONT;

	motionblend = FALSE;


}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Init(void)
{
	PrimitiveComponent::Init();


}

void MeshComponent::Update(void)
{
	PrimitiveComponent::Update();


	if (animstate == ANIM_STATE::NO_ANIM)
	{

	}
	if (animstate == ANIM_STATE::DATA_ANIM)
	{
		for (int i = 0; i < meshNum; i++)
		{
			MeshMtxArray[i] = this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetKeyFrameAnimData(animindex)->GetFrameMeshMtx(framecnt, i);


		}

		framecnt += 1;
		if (framecnt >= framenum)
		{
			framecnt = 0;
		}


	}
	if (animstate == ANIM_STATE::BLEND_ANIM)
	{
		float blendweight1 = (1.0f / blendcntmax) * blendcnt;
		float blendweight2 = 1.0f - blendweight1;




		for (int i = 0; i < meshNum; i++)
		{

			MeshMtxArray[i] = (this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetKeyFrameAnimData(animindex)->GetFrameMeshMtx(framecnt, i) * blendweight1) + (this->BlendMeshMtxArray[i] * blendweight2);



		}
		blendcnt += 1;
		if (blendcnt >= blendcntmax)
		{
			animstate = ANIM_STATE::DATA_ANIM;
		}


	}

}

void MeshComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();

}


void MeshComponent::Draw(void)
{
	this->GetGameObject()->GetLevel()->GetMain()->GetRenderer()->SetCullingMode(this->cullMode);

	for (int i = 0; i < this->meshNum; i++)
	{
		this->DrawMesh(i);

	}
	this->GetGameObject()->GetLevel()->GetMain()->GetRenderer()->SetCullingMode(CULL_MODE::CULL_MODE_BACK);

}
void MeshComponent::DrawMesh(int n)
{


	MeshDataList* list = this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetMeshDataList(this->MeshDataListIndex);
	this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetMeshDataList(this->MeshDataListIndex)->GetMeshData()[n].BufferSetVertex();
	this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetMeshDataList(this->MeshDataListIndex)->GetMeshData()[n].BufferSetIndex();


	XMMATRIX world = XMMatrixIdentity();
	world = XMMatrixMultiply(world, this->MeshMtxArray[n]);
	world = XMMatrixMultiply(world, this->GetWorldMtxWithParent());
	this->GetGameObject()->GetLevel()->GetMain()->GetRenderer()->SetWorldMatrix(&world);


	//マテリアルテクスチャ設定
	int subsetnum = this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetMeshDataList(this->MeshDataListIndex)->GetMeshData()[n].GetSubsetNum();

	for (unsigned short i = 0; i < subsetnum; i++)
	{
		MATERIAL m = this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetMeshDataList(this->MeshDataListIndex)->GetMeshData()[n].GetSubset()[0].GetMaterial();



		this->GetGameObject()->GetLevel()->GetMain()->GetRenderer()->SetMaterial(m);


		if (m.noDiffuseTex == FALSE)
		{
			this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetMeshDataList(this->MeshDataListIndex)->GetMeshData()[n].GetSubset()[i].GetTexture()[0].SetShaderResource();
		}
		this->GetGameObject()->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->DrawIndexed(this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetMeshDataList(this->MeshDataListIndex)->GetMeshData()[n].GetIndexNum(), 0, 0);
	}


}

void MeshComponent::SetCullMode(CULL_MODE mode)
{
	this->cullMode = mode;
}



void MeshComponent::CreatepAnimDataArray(int n)
{

}
void MeshComponent::SetpKeyFrameAnimData(int n, KeyFrameAnimData* p)
{

}

string MeshComponent::GetMeshFilePath(void)
{
	return meshFilePath;
}

void MeshComponent::SetMeshFilePath(string path)
{
	meshFilePath = path;
}

void MeshComponent::SetAnimFilePath(string path)
{
	animFilePath.push_back(path);
}

void MeshComponent::SetAnimFilePathArray(vector<string> path)
{
	for (int i = 0; i < path.size(); i++)
	{
		animFilePath.push_back(path[i]);

	}
}



void MeshComponent::SetMeshComponent(
	string meshFilePath,		//メッシュデータのパス
	vector<string> animFilePath,//アニメーションさせるか？
	BOOL blend,					//モーションブレンドするか？
	int blendN)					//モーションブレンディングのフレーム数
{


	SetMeshFilePath(meshFilePath);
	SetAnimFilePathArray(animFilePath);


	motionblend = blend;
	blendcnt = 0;
	blendcntmax = blendN;
	animation = TRUE;
	animstate = ANIM_STATE::DATA_ANIM;

}


void MeshComponent::SetMeshComponent(string meshFilePath)		//メッシュデータのパス
{
	SetMeshFilePath(meshFilePath);
	animation = FALSE;

}




void MeshComponent::SetMeshDataList(void)
{
	this->MeshDataListIndex = this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->LoadMesh(this->meshFilePath);
	this->CreateMeshMtxArray(this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetMeshDataList(this->MeshDataListIndex)->GetMeshDataNum());
	for (int i = 0; i < this->meshNum; i++)
	{
		MeshMtxArray[i] = this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetMeshDataList(this->MeshDataListIndex)->GetMeshData()[i].GetOffset();
	}
	this->SetBlendMtxArray();
}

void MeshComponent::SetAnimationArray(void)
{
	for (int i = 0; i < this->animFilePath.size(); i++)
	{
		int n = this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->LoadMeshAnim(this->animFilePath[i]);
		this->AnimDataIndexArray.push_back(n);
	}
	framecnt = 0;
	framenum = this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetKeyFrameAnimData(AnimDataIndexArray[0])->GetFrameNum();


}



void MeshComponent::CreateMeshMtxArray(int n)
{
	meshNum = n;
	MeshMtxArray = new XMMATRIX[n];
	BlendMeshMtxArray = new XMMATRIX[n];
}

void MeshComponent::SetMeshMtxArray(int n, XMMATRIX mtx)
{
	MeshMtxArray[n] = mtx;

}
void MeshComponent::SetBlendMtxArray(void)
{
	for (int i = 0; i < meshNum; i++)
	{
		BlendMeshMtxArray[i] = MeshMtxArray[i];
	}

}

void MeshComponent::SwichAnimIndex(int n)
{
	animindex = n;
	framecnt = 0;
	framenum = this->GetGameObject()->GetLevel()->GetMain()->GetAssetsManager()->GetKeyFrameAnimData(AnimDataIndexArray[n])->GetFrameNum();
	if (motionblend)
	{
		SetBlendMtxArray();
		blendcnt = 0;
		animstate = ANIM_STATE::BLEND_ANIM;

	}
}
