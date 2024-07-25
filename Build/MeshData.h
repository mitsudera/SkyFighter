//=============================================================================
//
// メッシュの処理 [meshdata.h]
// Author :三ツ寺貴紀
//
//=============================================================================
#pragma once
#include "CoreMinimal.h"
#include "Renderer.h"
//*********************************************************
//クラス定義
//*********************************************************

class DX11_TEXTURE;
class DX11_SUBSET;
class MeshData;
class MeshDataList;
class AssetsManager;

class DX11_TEXTURE
{

	DX11_SUBSET* psubset;
	ID3D11ShaderResourceView* Texture;

public:
	DX11_TEXTURE();
	~DX11_TEXTURE();
	
	ID3D11ShaderResourceView* GetTexture(void);
	void CreateTexture(char* path);

	void SetShaderResource(void);

	DX11_SUBSET* GetpSubset(void);
	void SetpSubset(DX11_SUBSET* subset);

private:

};



// 描画サブセットクラス
class DX11_SUBSET
{
	MeshData* pmeshdata;
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	int				texnum;
	MATERIAL		Material;
	DX11_TEXTURE*	Texture;


public:
	DX11_SUBSET();
	~DX11_SUBSET();



	void SetStartIndex(unsigned short n);
	unsigned short GetStartIndex(void);

	void SetIndexNum(unsigned short n);
	unsigned short GetIndexNum(void);

	void SetTexNum(int n);
	int GetTexNum(void);

	void SetMaterial(MATERIAL m);
	MATERIAL GetMaterial(void);


	void CreateTextureArray(int n);
	DX11_TEXTURE* GetTexture(void);


	MeshData* GetpMeshData(void);



	void SetpMeshData(MeshData* meshdata);

};


 class MeshData
 {
	 AssetsManager* pAssetsManager;

	 BOOL nomesh;

	 ID3D11Buffer* VertexBuffer;

	 unsigned int indexnum;
	 ID3D11Buffer* IndexBuffer;
	 XMMATRIX	offset;
	 unsigned short	subsetnum;
	 DX11_SUBSET* SubsetArray;
	 int childcnt;
	 MeshData* Child;


 public:
	 MeshData();
	 ~MeshData();

	 void SetNoMesh(BOOL no);
	 BOOL GetNoMesh(void);

	 void CreateVertexBuffer(int n);
	 ID3D11Buffer* GetVertexBuffer(void);

	 void SetIndexNum(unsigned int n);
	 unsigned int GetIndexNum(void);



	 void CreateIndexBuffer(int n);
	 ID3D11Buffer* GetIndexBuffer(void);

	 void SetChildCnt(int n);
	 int GetChildCount(void);

	 void CreateChildArray(int childnum);
	 MeshData* GetChild(int n);

	 void SetOffset(XMMATRIX offsetmtx);
	 XMMATRIX GetOffset(void);
		


	 void SetSubsetNum(int n);
	 unsigned short GetSubsetNum(void);



	 void CreateSubsetArray(int n);
	 DX11_SUBSET* GetSubset(void);
	 void SetSubset(int n, DX11_SUBSET sub);

	 void BufferSetVertex(void);
	 void BufferSetIndex(void);

	 void LoadFbxMesh(FbxMesh* mesh, AssetsManager* p);

	 void SetpAssetsManager(AssetsManager* pa);

	 AssetsManager* GetpAssetsManager(void);

 private:

 };

 class MeshDataList
 {
	 AssetsManager* pManager;
	 int meshdatanum;
	 MeshData* meshdata;
	 string filepath;

 public:
	 MeshDataList();
	 ~MeshDataList();

	 void SetMeshDataNum(int n);
	 int GetMeshDataNum(void);

	 void CreateMeshDataArray(int n);
	 MeshData* GetMeshData(void);


	 void SetFilePath(std::string fn);
	 std::string GetFilePath(void);

	 void LoadFbxFile(string filepath, AssetsManager* p);

	 AssetsManager*GetpAssetsManager(void);
	 void SetpAssetsManager(AssetsManager* pa);


 private:

 };

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

