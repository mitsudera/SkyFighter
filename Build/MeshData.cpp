//=============================================================================
//
// モデルの処理 [meshdata.cpp]
// Author : 三ツ寺貴紀
//
//=============================================================================
#include "meshdata.h"
#include "AssetsManager.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_PATH	"data/TEXTURE/"


//*****************************************************************************
// 構造体定義
//*****************************************************************************




//*****************************************************************************
// グローバル変数
//*****************************************************************************



//*****************************************************************************
// クラス関数
//*****************************************************************************
DX11_TEXTURE::DX11_TEXTURE()
{
	psubset = nullptr;
	this->Texture = nullptr;
	
}

DX11_TEXTURE::~DX11_TEXTURE()
{
	if(this->Texture)	Texture->Release();
}


ID3D11ShaderResourceView* DX11_TEXTURE::GetTexture(void)
{
	return this->Texture;
}
void DX11_TEXTURE::CreateTexture(char* path)
{

	D3DX11CreateShaderResourceViewFromFile(
		this->psubset->GetpMeshData()->GetpAssetsManager()->GetMain()->GetRenderer()->GetDevice(),
		path,
		NULL,
		NULL,
		&Texture,
		NULL);

}
void DX11_TEXTURE::SetShaderResource(void)
{
	this->psubset->GetpMeshData()->GetpAssetsManager()->GetMain()->GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, &Texture);
}
DX11_SUBSET* DX11_TEXTURE::GetpSubset(void)
{
	return psubset;
}
void DX11_TEXTURE::SetpSubset(DX11_SUBSET* subset)
{
	this->psubset = subset;
}



DX11_SUBSET::DX11_SUBSET()
{
	this->StartIndex = 0;
	this->IndexNum = 0;
	this->texnum = 0;
	this->Material.Ambient = { 0.0f,0.0f,0.0f,0.0f };
	this->Material.Diffuse = { 0.0f,0.0f,0.0f,0.0f };
	this->Material.Emission = { 0.0f,0.0f,0.0f,0.0f };
	this->Material.Specular = { 0.0f,0.0f,0.0f,0.0f };
	this->Material.Shininess = 0.0f;
	this->Material.noDiffuseTex = TRUE;
	this->Material.noNormalTex = FALSE;
	this->Texture = nullptr;
	this->pmeshdata = nullptr;
}
DX11_SUBSET::~DX11_SUBSET()
{
	for (int i = 0; i < this->texnum; i++)
	{
		delete[] Texture;
	}
}

void DX11_SUBSET::SetStartIndex(unsigned short n)
{
	this->StartIndex = n;
}
unsigned short DX11_SUBSET::GetStartIndex(void)
{
	return this->StartIndex;
}

void DX11_SUBSET::SetIndexNum(unsigned short n)
{
	this->IndexNum = n;
}
unsigned short DX11_SUBSET::GetIndexNum(void)
{
	return this->IndexNum;
}

void DX11_SUBSET::SetTexNum(int n)
{
	this->texnum = n;
}
int DX11_SUBSET::GetTexNum(void)
{
	return this->texnum;
}

void DX11_SUBSET::SetMaterial(MATERIAL m)
{
	this->Material = m;
}
MATERIAL DX11_SUBSET::GetMaterial(void)
{
	return Material;
}


void DX11_SUBSET::CreateTextureArray(int n)
{
	this->Texture = new DX11_TEXTURE[n];
	this->texnum = n;
	for (int i = 0; i < n; i++)
	{
		this->Texture[i].SetpSubset(this);
	}


}


DX11_TEXTURE* DX11_SUBSET::GetTexture(void)
{
	return this->Texture;
}

MeshData* DX11_SUBSET::GetpMeshData(void)
{
	return this->pmeshdata;
}

void DX11_SUBSET::SetpMeshData(MeshData* meshdata)
{
	this->pmeshdata = meshdata;
}



MeshData::MeshData()
{
	this->nomesh = TRUE;
	this->VertexBuffer = nullptr;
	this->indexnum = 0;
	this->IndexBuffer = nullptr;
	this->offset = XMMatrixIdentity();
	this->subsetnum = 0;
	this->SubsetArray = nullptr;
	this->childcnt = 0;
	this->Child = nullptr;
	this->pAssetsManager = nullptr;
}

MeshData::~MeshData()
{
	if(VertexBuffer) VertexBuffer->Release();
	if(IndexBuffer) IndexBuffer->Release();


	delete[]SubsetArray;
}


void MeshData::SetNoMesh(BOOL no)
{
	this->nomesh = no;
}
BOOL MeshData::GetNoMesh(void)
{
	return this->nomesh;
}



void MeshData::CreateVertexBuffer(int VertNum)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * VertNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetpAssetsManager()->GetMain()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->VertexBuffer);



}

ID3D11Buffer* MeshData::GetVertexBuffer(void)
{
	return VertexBuffer;
}


void MeshData::SetIndexNum(unsigned int n)
{
	this->indexnum = n;
}
unsigned int MeshData::GetIndexNum(void)
{
	return this->indexnum;
}


void MeshData::CreateIndexBuffer(int IndexNum)
{
	// インデックスバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned int) * IndexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetpAssetsManager()->GetMain()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->IndexBuffer);


}
ID3D11Buffer* MeshData::GetIndexBuffer(void)
{
	return IndexBuffer;
}

void MeshData::SetChildCnt(int n)
{
	childcnt = n;
}
int MeshData::GetChildCount(void)
{
	return childcnt;
}

void MeshData::CreateChildArray(int n)
{
	this->Child = new MeshData[n];
}



MeshData* MeshData::GetChild(int n)
{
	return &this->Child[n];
}

void MeshData::SetOffset(XMMATRIX offsetmtx)
{
	offset = offsetmtx;
}
XMMATRIX MeshData::GetOffset(void)
{
	return offset;
}



void MeshData::SetSubsetNum(int n)
{
	this->subsetnum = n;
}

unsigned short MeshData::GetSubsetNum(void)
{
	return this->subsetnum;
}

void MeshData::CreateSubsetArray(int n)
{
	this->SubsetArray = new DX11_SUBSET[n];

	for (int i = 0; i < n; i++)
	{
		this->SubsetArray[i].SetpMeshData(this);
	}
}


DX11_SUBSET* MeshData::GetSubset(void)
{
	return this->SubsetArray;
}

void MeshData::SetSubset(int n, DX11_SUBSET sub)
{
	this->SubsetArray[n] = sub;
}

void MeshData::BufferSetVertex(void)
{

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	this->GetpAssetsManager()->GetMain()->GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->VertexBuffer, &stride, &offset);
}

void MeshData::BufferSetIndex(void)
{

	// プリミティブトポロジ設定
	this->GetpAssetsManager()->GetMain()->GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// インデックスバッファ設定
	this->GetpAssetsManager()->GetMain()->GetRenderer()->GetDeviceContext()->IASetIndexBuffer(this->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

}


AssetsManager* MeshData::GetpAssetsManager(void)
{
	return this->pAssetsManager;
}



MeshDataList::MeshDataList()
{
	this->meshdatanum = 0;
	this->meshdata = nullptr;
	this->pManager = nullptr;
}

MeshDataList::~MeshDataList()
{
	if (meshdata) delete[]meshdata;
}


void MeshDataList::SetMeshDataNum(int n)
{
	this->meshdatanum = n;
}
int MeshDataList::GetMeshDataNum(void)
{
	return this->meshdatanum;
}


void MeshDataList::CreateMeshDataArray(int n)
{
	meshdata = new MeshData[n];
}

MeshData* MeshDataList::GetMeshData(void)
{
	return meshdata;
}

void MeshDataList::SetFilePath(std::string fn)
{
	filepath = fn;
}

std::string MeshDataList::GetFilePath(void)
{
	return filepath;

}
void MeshDataList::LoadFbxFile(string filepath, AssetsManager* p)
{
	
	this->pManager = p;
	SetFilePath(filepath);

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
	this->SetMeshDataNum(meshcount);
	this->CreateMeshDataArray(meshcount);
	for (int i = 0; i < meshcount; i++)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(i);

		this->meshdata[i].LoadFbxMesh(mesh,p);

	}

	scene->Destroy();
	importer->Destroy(); // シーンを流し込んだらImporterは解放
	manager->Destroy();//fbxファイルの読み込みが終わったらマネージャーは解放する



}
AssetsManager* MeshDataList::GetpAssetsManager(void)
{

	return this->pManager;
}



void MeshData::LoadFbxMesh(FbxMesh* mesh,AssetsManager* ap)
{

	this->pAssetsManager = ap;
	FbxNode* node = mesh->GetNode();
	

	int PolygonNum = mesh->GetPolygonCount();               //総ポリゴン数
	int PolygonVertexNum = mesh->GetPolygonVertexCount();   //ポリゴン頂点インデックス数
	int* IndexAry = mesh->GetPolygonVertices();             //ポリゴン頂点インデックス配列
	int controlNum = mesh->GetControlPointsCount();         // 頂点座標数
	FbxVector4* src = mesh->GetControlPoints();             // 頂点座標配列
	VERTEX_3D* VertexArray = nullptr;
	VertexArray = new VERTEX_3D[PolygonVertexNum];
	unsigned int indexnum = PolygonNum * 3;
	this->SetIndexNum(indexnum);

	int n = mesh->GetElementTangentCount();
	FbxGeometryElementTangent* t= mesh->GetElementTangent();
	//int n=mesh->GetElementPolygonGroupCount();
	
	//頂点配列を埋める
	FbxStringList uvSetNameList;
	mesh->GetUVSetNames(uvSetNameList);
	FbxVector2* uv = new FbxVector2[PolygonVertexNum];


	bool bIsUnmapped = false;

	unsigned int* IndexArry = nullptr;
	IndexArry = new unsigned int[indexnum];
	int ic = 0;


	int vcnt = 0;

	for (int p = 0; p < PolygonNum; p++)
	{

		int IndexNumInPolygon = mesh->GetPolygonSize(p);  // p番目のポリゴンの頂点数

		if (IndexNumInPolygon==3)
		{
			for (int n = 0; n < IndexNumInPolygon; n++)
			{
				FbxVector4 nor;

				mesh->GetPolygonVertexNormal(
					p,				// int pPolyIndex
					n,				// int pVertexIndex
					nor);        // FbxVector4& pNormal
				VertexArray[vcnt].Normal.x = (float)nor[0];
				VertexArray[vcnt].Normal.y = (float)nor[1];
				VertexArray[vcnt].Normal.z = (float)nor[2];



				// UV値取得。
				FbxString name = uvSetNameList.GetStringAt(0);
				mesh->GetPolygonVertexUV(p, n, name, uv[vcnt], bIsUnmapped);
				VertexArray[vcnt].TexCoord.x = (float)uv[vcnt][0];
				VertexArray[vcnt].TexCoord.y = 1.0f - (float)uv[vcnt][1];


				// ポリゴンpを構成するn番目の頂点のインデックス番号
				int IndexNumber = mesh->GetPolygonVertex(p, n);
				VertexArray[vcnt].Position.x = (float)src[IndexNumber][0];
				VertexArray[vcnt].Position.y = (float)src[IndexNumber][1];
				VertexArray[vcnt].Position.z = -(float)src[IndexNumber][2];


				VertexArray[vcnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };

				IndexArry[ic] = vcnt;
				ic++;
				vcnt++;

			}

		}
		else if (IndexNumInPolygon==4)
		{
			for (int n = 0; n < 3; n++)
			{
				FbxVector4 nor;

				mesh->GetPolygonVertexNormal(
					p,				// int pPolyIndex
					n,				// int pVertexIndex
					nor);        // FbxVector4& pNormal
				VertexArray[vcnt].Normal.x = (float)nor[0];
				VertexArray[vcnt].Normal.y = (float)nor[1];
				VertexArray[vcnt].Normal.z = (float)nor[2];



				// UV値取得。
				FbxString name = uvSetNameList.GetStringAt(0);
				mesh->GetPolygonVertexUV(p, n, name, uv[vcnt], bIsUnmapped);
				VertexArray[vcnt].TexCoord.x = (float)uv[vcnt][0];
				VertexArray[vcnt].TexCoord.y = 1.0f - (float)uv[vcnt][1];


				// ポリゴンpを構成するn番目の頂点のインデックス番号
				int IndexNumber = mesh->GetPolygonVertex(p, n);
				VertexArray[vcnt].Position.x = (float)src[IndexNumber][0];
				VertexArray[vcnt].Position.y = (float)src[IndexNumber][1];
				VertexArray[vcnt].Position.z = -(float)src[IndexNumber][2];


				VertexArray[vcnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };


				IndexArry[ic] = vcnt;
				ic++;

				vcnt++;

			}
			for (int n = 1; n < 4; n++)
			{
				FbxVector4 nor;

				mesh->GetPolygonVertexNormal(
					p,				// int pPolyIndex
					n,				// int pVertexIndex
					nor);        // FbxVector4& pNormal
				VertexArray[vcnt].Normal.x = (float)nor[0];
				VertexArray[vcnt].Normal.y = (float)nor[1];
				VertexArray[vcnt].Normal.z = (float)nor[2];



				// UV値取得。
				FbxString name = uvSetNameList.GetStringAt(0);
				mesh->GetPolygonVertexUV(p, n, name, uv[vcnt], bIsUnmapped);
				VertexArray[vcnt].TexCoord.x = (float)uv[vcnt][0];
				VertexArray[vcnt].TexCoord.y = 1.0f - (float)uv[vcnt][1];


				// ポリゴンpを構成するn番目の頂点のインデックス番号
				int IndexNumber = mesh->GetPolygonVertex(p, n);
				VertexArray[vcnt].Position.x = (float)src[IndexNumber][0];
				VertexArray[vcnt].Position.y = (float)src[IndexNumber][1];
				VertexArray[vcnt].Position.z = -(float)src[IndexNumber][2];


				VertexArray[vcnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };

				IndexArry[ic] = vcnt;
				ic++;


				vcnt++;

			}

		}

	}
	////インデックス配列を埋める
	//unsigned int* IndexArry = nullptr;
	//IndexArry = new unsigned int[indexnum];
	//int ic = 0;

	//for (int i = 0; i < PolygonNum; i++)
	//{

	//	IndexArry[i * 3] = ic;
	//	ic++;
	//	IndexArry[i * 3 + 1] = ic;
	//	ic++;
	//	IndexArry[i * 3 + 2] = ic;
	//	ic++;
	//}

	// 頂点バッファ生成
	this->CreateVertexBuffer(ic);

	{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		this->GetpAssetsManager()->GetMain()->GetRenderer()->GetDeviceContext()->Map(this->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, VertexArray, sizeof(VERTEX_3D) * PolygonVertexNum);

		this->GetpAssetsManager()->GetMain()->GetRenderer()->GetDeviceContext()->Unmap(this->GetVertexBuffer(), 0);
	}
	// インデックスバッファ生成
	this->CreateIndexBuffer(indexnum);

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		this->GetpAssetsManager()->GetMain()->GetRenderer()->GetDeviceContext()->Map(this->GetIndexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned int* pIdx = (unsigned int*)msr.pData;

		memcpy(pIdx, IndexArry, sizeof(unsigned int) * indexnum);

		//int n;
		//for (unsigned int i = 0; i < indexnum; i++)
		//{
		//	pIdx[i] = IndexArry[i];
		//	n = i;
		//}

		this->GetpAssetsManager()->GetMain()->GetRenderer()->GetDeviceContext()->Unmap(this->GetIndexBuffer(), 0);
	}



	FbxMatrix offset = node->EvaluateGlobalTransform(FBXSDK_TIME_INFINITE);//オフセット行列の取得
	this->SetOffset(FbxMatrixConvertToXMMATRIX(offset));


	// マテリアルの数
	//int materialNum = 1;



	int mtlcnt = node->GetMaterialCount();

	this->SetSubsetNum(mtlcnt);
	this->CreateSubsetArray(mtlcnt);

	// マテリアル情報を取得
	for (int i = 0; i < mtlcnt; i++)
	{
		FbxSurfaceMaterial* fbxmaterial =node->GetMaterial(i);
		
		FbxClassId id = fbxmaterial->GetClassId();

		if (fbxmaterial != 0)
		{
			MATERIAL material;

			// マテリアル解析
			// LambertかPhongか
			//lambart
			material.phong = 0;

			if (id==(FbxSurfaceLambert::ClassId))
			{

				// Lambertにダウンキャスト
				FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)fbxmaterial;

				material.phong = 0;

				//アンビエント
				FbxDouble3 amb = lambert->Ambient;

				material.Ambient.x = (float)amb[0];
				material.Ambient.y = (float)amb[1];
				material.Ambient.z = (float)amb[2];

				//ディフューズ
				FbxDouble3 diffuse = lambert->Diffuse;

				material.Diffuse.x = (float)diffuse[0];
				material.Diffuse.y = (float)diffuse[1];
				material.Diffuse.z = (float)diffuse[2];

				// 透過度
				FbxDouble transparency = lambert->TransparencyFactor;
				material.Diffuse.w = (float)transparency;

				//エミッション
				FbxDouble3 emi = lambert->Emissive;

				material.Emission.x = (float)emi[0];
				material.Emission.y = (float)emi[1];
				material.Emission.z = (float)emi[2];



			}
			//phong
			else if (id==(FbxSurfacePhong::ClassId))
			{

				// Phongにダウンキャスト
				FbxSurfacePhong* phong = (FbxSurfacePhong*)fbxmaterial;
				material.phong = 1;
				//アンビエント
				FbxDouble3 amb = phong->Ambient;

				material.Ambient.x = (float)amb[0];
				material.Ambient.y = (float)amb[1];
				material.Ambient.z = (float)amb[2];

				//ディフューズ
				FbxDouble3 diffuse = phong->Diffuse;

				material.Diffuse.x = (float)diffuse[0];
				material.Diffuse.y = (float)diffuse[1];
				material.Diffuse.z = (float)diffuse[2];

				// 透過度
				FbxDouble transparency = phong->TransparencyFactor;
				material.Diffuse.w = (float)transparency;

				//エミッション
				FbxDouble3 emi = phong->Emissive;

				material.Emission.x = (float)emi[0];
				material.Emission.y = (float)emi[1];
				material.Emission.z = (float)emi[2];

				//スペキュラー
				FbxDouble3 spe = phong->Specular;

				material.Specular.x = (float)spe[0];
				material.Specular.y = (float)spe[1];
				material.Specular.z = (float)spe[2];

				FbxDouble shine = phong->Shininess;
				material.Shininess = (float)shine;


			}
			else
			{
				material.phong = 0;
				material.Diffuse.x = (float)1.0f;
				material.Diffuse.y = (float)1.0f;
				material.Diffuse.z = (float)1.0f;
				material.Diffuse.w = 1.0f;

			}



			// プロパティ取得。
			const FbxProperty property = fbxmaterial->FindProperty(
				FbxSurfaceMaterial::sDiffuse    // const char* pName
			);                                  // bool pCaseSensitive = true


			int fileTextureCount = property.GetSrcObjectCount<FbxFileTexture>();
			// プロパティが持っているレイヤードテクスチャの枚数をチェック
			int layerNum = property.GetSrcObjectCount<FbxLayeredTexture>();
			this->GetSubset()[i].CreateTextureArray(fileTextureCount);


			material.noDiffuseTex = true;
			material.noNormalTex = true;
			for (int j = 0; j < fileTextureCount; j++)
			{

				FbxFileTexture* pFileTexture = mesh->GetScene()->GetSrcObject<FbxFileTexture>(j);

				FbxFileTexture::ETextureUse m_type = FbxFileTexture::ETextureUse(pFileTexture->GetTextureUse());


				//ディフューズテクスチャなら
				if (m_type == FbxFileTexture::ETextureUse::eStandard)
				{
					const char* fileName = pFileTexture->GetFileName();
					int slush = '/';
					char* path;
					path = new char[256];

					//最後に/が出てくる場所
					const char* last = strrchr(fileName, slush);

					strcpy(path, "data/TEXTURE");
					strcat(path, last);

					this->GetSubset()[i].GetTexture()[0].CreateTexture(path);

					delete[]path;

					// マテリアル設定
					material.noDiffuseTex = false;
				}
				if (m_type == FbxFileTexture::ETextureUse::eBumpNormalMap)
				{

					const char* fileName = pFileTexture->GetFileName();
					int slush = '/';
					char* path;
					path = new char[256];

					//最後に/が出てくる場所
					const char* last = strrchr(fileName, slush);

					strcpy(path, "data/TEXTURE");
					strcat(path, last);

					this->GetSubset()[i].GetTexture()[1].CreateTexture(path);

					delete[]path;

				}


			}
			this->GetSubset()[i].SetMaterial(material);
			this->GetSubset()[i].SetpMeshData(this);

			MATERIAL m = this->GetSubset()[i].GetMaterial();
		}
	}


	delete[] uv;
	delete[] IndexArry;
	delete[] VertexArray;




}


void MeshData::SetpAssetsManager(AssetsManager* pa)
{
	this->pAssetsManager = pa;
}
void MeshDataList::SetpAssetsManager(AssetsManager* pa)
{
	this->pManager = pa;

}
