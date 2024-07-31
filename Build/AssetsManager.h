#pragma once
#include "main.h"


class MeshDataList;

class KeyFrameAnimData; 

class SkinMeshDataList;

class SkeletonAnimData;


class AssetsManager
{


public:
	AssetsManager(Main* main);
	~AssetsManager();

	void Init(void);
	void Uninit(void);




	int LoadMesh(string filepath);

	MeshDataList* GetMeshDataList(int n);

	KeyFrameAnimData* GetKeyFrameAnimData(int n);


	Main* GetMain(void);

	int LoadMeshAnim(string filepath);

	SkinMeshDataList* GetSkinMeshDataList(int n);
	int LoadSkinMesh(string filepath);

	SkeletonAnimData* GetSkeletonAnimData(int n);
	int LoadSkeletonAnimData(string filepath);

	


private:

	Main* pMain;
	vector<MeshDataList*> MeshDataListArray;
	vector<KeyFrameAnimData*>  KeyFrameAnimDataArray;
	vector<SkinMeshDataList*> SkinMeshDataListArray;
	vector<SkeletonAnimData*> SkeletonAnimDataArray;
};

