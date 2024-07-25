#include "MeshComponent.h"
#include "GameEngine.h"
#include "Renderer.h"
#include "AssetsManager.h"
#include "SkinMeshDataList.h"
#include "World.h"
#include "SkinMeshComponent.h"
#include "SkeletonAnimData.h"

SkinMeshComponent::SkinMeshComponent()
{
	this->blendBoneMtx = nullptr;
	this->boneCnt = 0;
	this->meshVertex = nullptr;
	this->boneMtx = nullptr;

}

SkinMeshComponent::~SkinMeshComponent()
{
}

void SkinMeshComponent::Init(void)
{
	MeshComponent::Init();
	SetName("SkinMesh");

}

void SkinMeshComponent::Update(void)
{
	PrimitiveComponent::Update();


	if (animstate == ANIM_STATE::NO_ANIM)
	{



	}
	if (animstate == ANIM_STATE::DATA_ANIM)
	{
		
		for (int i = 0; i < this->boneCnt; i++)
		{
			this->boneMtx[i] = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[animindex])->GetFrameSkeleton(framecnt)->GetBone(i)->GetMtx();

		}


		for (int i = 0; i < this->meshNum; i++)
		{
			SkinMeshData* skinmeshdata = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData();

			int controlN = skinmeshdata[i].GetControlNum();
			CONTROLPOINT* cparray = new CONTROLPOINT[controlN];
			SkinMeshDataList* smdl = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex);
			SkeletonAnimData* sad = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[animindex]);





			for (int j = 0; j < controlN; j++)
			{

				CONTROLPOINT* cp = skinmeshdata[i].GetControlPoint(j);
				XMVECTOR cpoint = XMLoadFloat3(&cp->pos);

				XMMATRIX lasttransmtx = XMMatrixIdentity() * 0.0f;
				float totalweight = 0.0f;
				for (int k = 0; k < this->boneCnt; k++)
				{
					float weight = cp->weight[k];
					if (weight > 0.0f)
					{
						XMMATRIX transmtx;

						transmtx = XMMatrixMultiply(smdl->GetSkeleton()->GetBone(k)->GetInitMtxInverse(), this->boneMtx[k]);
						transmtx = transmtx * weight;
						lasttransmtx = lasttransmtx + transmtx;


					}
				}
				cpoint = XMVector3Transform(cpoint, lasttransmtx);
				XMFLOAT3 pos;
				XMStoreFloat3(&pos, cpoint);

				cparray[j].pos = pos;
				cparray[j].pos.z = -pos.z;

			}

			for (int j = 0; j < this->meshVertex[i].vertNum; j++)
			{
				this->meshVertex[i].vertexArray[j].Position = cparray[skinmeshdata[i].GetCPIndexArray()[j]].pos;
				
			}
			delete[] cparray;
			// 頂点バッファへのポインタを取得
			D3D11_MAPPED_SUBRESOURCE msr;
			this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->meshVertex[i].m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

			memcpy(pVtx, this->meshVertex[i].vertexArray, sizeof(VERTEX_3D) * this->meshVertex[i].vertNum);

			this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->meshVertex[i].m_vertexBuffer, 0);


			
			this->meshVertex[i].vertexArray;

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


		for (int i = 0; i < this->boneCnt; i++)
		{
			XMMATRIX mtx1 = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[animindex])->GetFrameSkeleton(framecnt)->GetBone(i)->GetMtx() * blendweight1;
			XMMATRIX mtx2 = this->blendBoneMtx[i] * blendweight2;
			
			this->boneMtx[i] = mtx1 + mtx2;
		}



		for (int i = 0; i < this->meshNum; i++)
		{
			SkinMeshData* skinmeshdata = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData();

			int controlN = skinmeshdata[i].GetControlNum();
			CONTROLPOINT* cparray = new CONTROLPOINT[controlN];
			SkinMeshDataList* smdl = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex);
			SkeletonAnimData* sad = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[animindex]);





			for (int j = 0; j < controlN; j++)
			{

				CONTROLPOINT* cp = skinmeshdata[i].GetControlPoint(j);
				XMVECTOR cpoint = XMLoadFloat3(&cp->pos);

				XMMATRIX lasttransmtx = XMMatrixIdentity() * 0.0f;
				float totalweight = 0.0f;
				for (int k = 0; k < this->boneCnt; k++)
				{
					float weight = cp->weight[k];
					if (weight > 0.0f)
					{
						XMMATRIX transmtx;

						transmtx = XMMatrixMultiply(smdl->GetSkeleton()->GetBone(k)->GetInitMtxInverse(), this->boneMtx[k]);
						transmtx = transmtx * weight;
						lasttransmtx = lasttransmtx + transmtx;


					}
				}
				cpoint = XMVector3Transform(cpoint, lasttransmtx);
				XMFLOAT3 pos;
				XMStoreFloat3(&pos, cpoint);

				cparray[j].pos = pos;
				cparray[j].pos.z = -pos.z;


			}

			for (int j = 0; j < this->meshVertex[i].vertNum; j++)
			{
				this->meshVertex[i].vertexArray[j].Position = cparray[skinmeshdata[i].GetCPIndexArray()[j]].pos;

			}
			delete[] cparray;
			// 頂点バッファへのポインタを取得
			D3D11_MAPPED_SUBRESOURCE msr;
			this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->meshVertex[i].m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

			memcpy(pVtx, this->meshVertex[i].vertexArray, sizeof(VERTEX_3D) * this->meshVertex[i].vertNum);

			this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->meshVertex[i].m_vertexBuffer, 0);



			this->meshVertex[i].vertexArray;

		}




		blendcnt += 1;
		if (blendcnt >= blendcntmax)
		{
			animstate = ANIM_STATE::DATA_ANIM;
		}

	}


}

void SkinMeshComponent::Uninit(void)
{
	MeshComponent::Uninit();
}

void SkinMeshComponent::BeginPlay(void)
{
	MeshComponent::BeginPlay();
}

void SkinMeshComponent::UpdatePlay(void)
{
	MeshComponent::UpdatePlay();
}

void SkinMeshComponent::EndPlay(void)
{
	MeshComponent::EndPlay();
}

void SkinMeshComponent::Draw(void)
{
	for (int i = 0; i < this->meshNum; i++)
	{
		DrawSkinMesh(i);
	}
}

void SkinMeshComponent::DrawSkinMesh(int n)
{
	this->GetWorld()->GetGameEngine()->GetRenderer()->SetCullingMode(CULL_MODE_NONE);
	MeshDataList* list = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex);
	
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->meshVertex[n].m_vertexBuffer, &stride, &offset);


	this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[n].BufferSetIndex();

	XMMATRIX world = XMMatrixIdentity();
	world = XMMatrixMultiply(world, this->MeshMtxArray[n]);
	world = XMMatrixMultiply(world, GetWorldMatrix());
	this->GetWorld()->GetGameEngine()->GetRenderer()->SetWorldMatrix(&world);


	//マテリアルテクスチャ設定
	int subsetnum = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[n].GetSubsetNum();

	for (unsigned short i = 0; i < subsetnum; i++)
	{
		MATERIAL m = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[n].GetSubset()[0].GetMaterial();



		this->GetWorld()->GetGameEngine()->GetRenderer()->SetMaterial(m);


		if (m.noDiffuseTex == FALSE)
		{
			this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[n].GetSubset()[i].GetTexture()[0].SetShaderResource();
		}
		this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->DrawIndexed(this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[n].GetIndexNum(), 0, 0);
	}

}

void SkinMeshComponent::SetSkinMeshStatus(string meshfilepath, vector<string> animfilepatharray, BOOL blend, int blendframe)
{
	
	SetMeshFilePath(meshfilepath);
	SetAnimFilePathArray(animfilepatharray);


	motionblend = blend;
	blendcnt = 0;
	blendcntmax = blendframe;
	animation = TRUE;
	animstate = ANIM_STATE::DATA_ANIM;
	animindex = 0;

}
void SkinMeshComponent::SetSkinMeshStatus(string meshfilepath)
{
	
	SetMeshFilePath(meshfilepath);


	blendcnt = 0;
	animation = FALSE;
	animstate = ANIM_STATE::NO_ANIM;
	animindex = 0;

}




void SkinMeshComponent::SetSkinMesh(void)
{
	this->MeshDataListIndex = this->GetWorld()->GetGameEngine()->GetAssetsManager()->LoadSkinMesh(this->meshFilePath);
	this->CreateMeshMtxArray(this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetMeshDataNum());
	this->meshVertex = new MeshVertex[this->meshNum];
	this->boneCnt = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkeleton()->GetBoneCount();
	this->boneMtx = new XMMATRIX[boneCnt];
	this->blendBoneMtx = new XMMATRIX[boneCnt];
	for (int i = 0; i < boneCnt; i++)
	{
		this->boneMtx[i] = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkeleton()->GetBone(i)->GetMtx();

	}
	SetBlendBone();

	for (int i = 0; i < this->meshNum; i++)
	{

		MeshMtxArray[i] = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[i].GetOffset();
		int vertNum = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[i].GetVertNum();
		this->meshVertex[i].vertexArray = new VERTEX_3D[vertNum];
		this->meshVertex[i].vertNum = vertNum;
		for (int j = 0; j < vertNum; j++)
		{
			this->meshVertex[i].vertexArray[j] = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[i].GetVertex(j);
		}



		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * vertNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		this->GetWorld()->GetGameEngine()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->meshVertex[i].m_vertexBuffer);

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->meshVertex[i].m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, this->meshVertex[i].vertexArray, sizeof(VERTEX_3D) * vertNum);

		this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->meshVertex[i].m_vertexBuffer, 0);

			
	}
	this->SetBlendMtxArray();


	if (animation==TRUE)
	{
		for (int i = 0; i < this->animFilePath.size(); i++)
		{
			int n = this->GetWorld()->GetGameEngine()->GetAssetsManager()->LoadSkeletonAnimData(this->animFilePath[i]);
			this->AnimDataIndexArray.push_back(n);
		}
		framecnt = 0;
		framenum = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[this->animindex])->GetFrameNum();

	}




}

void SkinMeshComponent::SetBlendBone(void)
{

	for (int i = 0; i < boneCnt; i++)
	{
		this->blendBoneMtx[i] = this->boneMtx[i];
	}
}

void SkinMeshComponent::SwichAnimIndex(int n)
{
	animindex = n;
	framecnt = 0;
	framenum = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(AnimDataIndexArray[n])->GetFrameNum();
	if (motionblend)
	{
		SetBlendMtxArray();
		SetBlendBone();
		blendcnt = 0;
		animstate = ANIM_STATE::BLEND_ANIM;

	}

}


