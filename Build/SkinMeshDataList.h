#pragma once
#include "CoreMinimal.h"
#include "MeshData.h"
class DX11_SUBSET;
class MeshData;
class MeshDataList;
class AssetsManager;



struct CONTROLPOINT
{
	XMFLOAT3 pos;
	float* weight;


};



class BONE
{
public:
	BONE();
	~BONE();
	void LoadBone(FbxCluster* cluster);
	void LoadBone(FbxNode* node, BONE* parent);
	void LoadBoneFrame(FbxCluster* cluster,FbxTime time);
	void SetInitMtx(XMMATRIX Mtx);
	XMMATRIX GetMtx(void);
	XMMATRIX GetInitMtxInverse(void);
	void CreateChildArray(int n);


private:
	XMMATRIX mtx;
	XMMATRIX initMtxInverse;
	int childCnt;
	BONE* child;
	BONE* parent;
	BOOL isRootBone;
};

class SKELETON
{
public:
	SKELETON();
	~SKELETON();
	void CreateBoneArray(int n);
	void LoadSkeleton(FbxScene* scene);
	void LoadSkeletonFrame(FbxSkin* skin, int boneCnt, FbxTime time);
	BONE* GetBone(int n);
	int GetBoneCount(void);

private:
	BONE* bone;
	int boneCnt;
};


class SkinMeshData:public MeshData
{
public:
	SkinMeshData();
	~SkinMeshData();
	void LoadSkinMeshData(FbxMesh* mesh,SKELETON* sp,AssetsManager* ap);
	int GetVertNum(void);
	VERTEX_3D GetVertex(int n);
	CONTROLPOINT GetControlPointVertex(int n);//頂点番号からコントロールポイント取得
	CONTROLPOINT* GetControlPoint(int n);
	int GetControlNum(void);
	int* GetCPIndexArray(void);


private:

	SKELETON *pSkeleton;
	int controlPointNum;
	CONTROLPOINT* controlPoint;
	VERTEX_3D* VertexArray;
	int* controlPointIndexArray;
	int indexNum;
	int vertNum;


};

class SkinMeshDataList:public MeshDataList
{

public:
	SkinMeshDataList();
	~SkinMeshDataList();
	

	void LoadSkinMeshDataList(string filepath,AssetsManager* ap);
	void CreateSkinMeshDataArray(int n);
	SkinMeshData* GetSkinMeshData(void);
	SKELETON* GetSkeleton(void);


private:
	SkinMeshData* skinMeshData;
	
	SKELETON* skeleton;

};

XMMATRIX GetParentTransFormFbx(FbxNode* boneNode, XMMATRIX mtx);
