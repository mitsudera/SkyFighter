#include "AssetsManager.h"
#include "MeshData.h"
#include "MeshAnimData.h"
#include "SkinMeshDataList.h"
#include "SkeletonAnimData.h"
AssetsManager::AssetsManager(Main* main)
{
	this->pMain = main;
}


AssetsManager::~AssetsManager()
{

	for (int i = 0; i < this->MeshDataListArray.size(); i++)
	{
		delete this->MeshDataListArray[i];

	}
	this->MeshDataListArray.clear();

	for (int i = 0; i < this->KeyFrameAnimDataArray.size(); i++)
	{
		delete this->KeyFrameAnimDataArray[i];
	}
	this->KeyFrameAnimDataArray.clear();

	for (int i = 0; i < this->SkinMeshDataListArray.size(); i++)
	{
		delete this->SkinMeshDataListArray[i];
	}
	this->SkinMeshDataListArray.clear();


	for (int i = 0; i < this->SkeletonAnimDataArray.size(); i++)
	{
		delete this->SkeletonAnimDataArray[i];
	}
	this->SkeletonAnimDataArray.clear();






}

void AssetsManager::Init(void)
{

}

void AssetsManager::Uninit(void)
{


}

int AssetsManager::LoadMesh(string filepath)
{
	int p = -1;
	BOOL find = FALSE;
	for (int i = 0; i < MeshDataListArray.size(); i++)
	{
		if (filepath == MeshDataListArray[i]->GetFilePath())
		{
			p = i;
			find = TRUE;
			break;
		}
	}

	if (find == FALSE)
	{

		MeshDataList* meshdatalist = new MeshDataList;
		
		meshdatalist->LoadFbxFile(filepath, this);

		this->MeshDataListArray.push_back(meshdatalist);
		p = (int)MeshDataListArray.size() - 1;

	}

	return p;
}
MeshDataList* AssetsManager::GetMeshDataList(int n)
{
	return this->MeshDataListArray[n];
}

KeyFrameAnimData* AssetsManager::GetKeyFrameAnimData(int n)
{
	return this->KeyFrameAnimDataArray[n];
}




Main* AssetsManager::GetMain(void)
{
	return this->pMain;
}

int AssetsManager::LoadMeshAnim(string filepath)
{
	int p = -1;
	BOOL find = FALSE;
	for (int i = 0; i < KeyFrameAnimDataArray.size(); i++)
	{
		if (filepath == KeyFrameAnimDataArray[i]->GetFilePath())
		{
			p = i;
			find = TRUE;
			break;
		}
	}

	if (find == FALSE)
	{
		KeyFrameAnimData* animdata = new KeyFrameAnimData;
		animdata->LoadKeyFrameAnim(filepath);

		this->KeyFrameAnimDataArray.push_back(animdata);
		p = (int)MeshDataListArray.size() - 1;
	}

	return p;
}

int AssetsManager::LoadSkinMesh(string filepath)
{
	int p = -1;
	BOOL find = FALSE;
	for (int i = 0; i < SkinMeshDataListArray.size(); i++)
	{
		if (filepath == SkinMeshDataListArray[i]->GetFilePath())
		{
			p = i;
			find = TRUE;
			break;
		}
	}

	if (find == FALSE)
	{

		SkinMeshDataList* skinmeshdatalist = new SkinMeshDataList;

		skinmeshdatalist->LoadSkinMeshDataList(filepath, this);

		this->SkinMeshDataListArray.push_back(skinmeshdatalist);
		p = (int)SkinMeshDataListArray.size() - 1;

	}

	return p;
}
SkeletonAnimData* AssetsManager::GetSkeletonAnimData(int n)
{
	return this->SkeletonAnimDataArray[n];
}
int AssetsManager::LoadSkeletonAnimData(string filepath)
{
	int p = -1;
	BOOL find = FALSE;
	for (int i = 0; i < SkeletonAnimDataArray.size(); i++)
	{
		if (filepath == SkeletonAnimDataArray[i]->GetFilePath())
		{
			p = i;
			find = TRUE;
			break;
		}
	}

	if (find == FALSE)
	{

		SkeletonAnimData* skeletonAnimdata = new SkeletonAnimData;

		
		skeletonAnimdata->LoadSkeletonAnimData(filepath);

		this->SkeletonAnimDataArray.push_back(skeletonAnimdata);
		p = (int)SkeletonAnimDataArray.size() - 1;

	}

	return p;
}
SkinMeshDataList* AssetsManager::GetSkinMeshDataList(int n)
{
	return this->SkinMeshDataListArray[n];
}
