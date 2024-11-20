#include "SquareParticle.h"
#include "CameraComponent.h"
#include "camera.h"

#define PART_MAX (8192)

#define TEXTUE_PATH "data/PARTICLE/partical.png"

SquareParticle::SquareParticle(Level* level)
{

	this->pLevel = level;
	maxPart = PART_MAX;
	vertNum = maxPart * 4;
	indexNum = maxPart * 6;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	squareInfoArray = new SquareInfo[maxPart];
	indexArray = new unsigned int[indexNum];
	vertexArray = new VERTEX_3D[vertNum];
	texture = nullptr;
}

SquareParticle::~SquareParticle()
{
	delete[] squareInfoArray;
	delete[] indexArray;
	delete[] vertexArray;
}

void SquareParticle::Init(void)
{
	blend = BLEND_MODE::BLEND_MODE_ADD;

	D3DX11CreateShaderResourceViewFromFile(GetLevel()->GetMain()->GetRenderer()->GetDevice(),
		TEXTUE_PATH,
		NULL,
		NULL,
		&texture,
		NULL);


	for (unsigned int i = 0; i < maxPart; i++)
	{
		squareInfoArray[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		squareInfoArray[i].size = 0.0f;
		squareInfoArray[i].use = FALSE;

		squareInfoArray[i].vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
		squareInfoArray[i].vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
		squareInfoArray[i].vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
		squareInfoArray[i].vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


		for (int j = 0; j < 4; j++)
		{
			squareInfoArray[i].vertex[j].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			squareInfoArray[i].vertex[j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			squareInfoArray[i].vertex[j].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

			

			vertexArray[i * 4 + j] = squareInfoArray[i].vertex[j];


		}

	}




	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * vertNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetLevel()->GetMain()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBuffer);


	D3D11_MAPPED_SUBRESOURCE msrV;
	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msrV);

	VERTEX_3D* pvtx = (VERTEX_3D*)msrV.pData;

	memcpy(pvtx, vertexArray, sizeof(VERTEX_3D) * vertNum);


	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Unmap(this->vertexBuffer, 0);




	// インデックスバッファ生成
	
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned int) * indexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetLevel()->GetMain()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->indexBuffer);


	for (unsigned int i = 0; i < maxPart; i++)
	{
		indexArray[i * 6] = i * 4;
		indexArray[i * 6 + 1] = i * 4 + 1;
		indexArray[i * 6 + 2] = i * 4 + 2;
		indexArray[i * 6 + 3] = i * 4 + 1;
		indexArray[i * 6 + 4] = i * 4 + 2;
		indexArray[i * 6 + 5] = i * 4 + 3;
	}


	D3D11_MAPPED_SUBRESOURCE msrI;
	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Map(this->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msrI);

	unsigned int* pIdx = (unsigned int*)msrI.pData;

	memcpy(pIdx, indexArray, sizeof(unsigned int) * indexNum);

	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Unmap(this->indexBuffer, 0);





}

void SquareParticle::Uninit(void)
{
	if (vertexBuffer) vertexBuffer->Release();
	vertexBuffer = nullptr;
	if (indexBuffer) indexBuffer->Release();
	indexBuffer = nullptr;
}

void SquareParticle::Update(void)
{
	

	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view,GetLevel()->GetCamera()->GetView());

	XMFLOAT3 vx = XMFLOAT3(view._11, view._21, view._31);
	XMVECTOR axx = XMLoadFloat3(&vx);
	XMFLOAT3 vy = XMFLOAT3(view._12, view._22, view._32);
	XMVECTOR axy = XMLoadFloat3(&vy);
	

	for (unsigned int i = 0; i < maxPart; i++)
	{
		float size = squareInfoArray[i].size;
		XMVECTOR vvec[4];
		XMVECTOR posv = XMLoadFloat3(&squareInfoArray[i].pos);





		vvec[0] = axy * size;
		vvec[1] = axx * size;
		vvec[2] = -axx * size;
		vvec[3] = -axy * size;
		


		XMFLOAT3 vpos[4];


		for (int j = 0; j < 4; j++)
		{
			vvec[j] += posv;
			XMStoreFloat3(&vpos[j], vvec[j]);

		}





		for (int j = 0; j < 4; j++)
		{
			squareInfoArray[i].vertex[j].Position = vpos[j];
		}


	}


	for (unsigned int i = 0; i < maxPart; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			int n = i * 4 + j;
			vertexArray[i * 4 + j] = squareInfoArray[i].vertex[j];
		}
	}

	D3D11_MAPPED_SUBRESOURCE msrV;
	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msrV);

	VERTEX_3D* pvtx = (VERTEX_3D*)msrV.pData;

	memcpy(pvtx, vertexArray, sizeof(VERTEX_3D) * vertNum);


	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Unmap(this->vertexBuffer, 0);


}

void SquareParticle::Draw(void)
{
	Renderer* renderer = GetLevel()->GetMain()->GetRenderer();

	renderer->SetCullingMode(CULL_MODE_NONE);

	// ライティングを無効に
	renderer->SetLightEnable(FALSE);

	// 加算合成に設定
	renderer->SetBlendState(this->blend);

	renderer->SetAlphaTestEnable(FALSE);

	// Z比較無し
	renderer->SetDepthEnable(FALSE);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	renderer->GetDeviceContext()->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f);
	renderer->SetMaterial(material);

	// テクスチャ設定
	renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &texture);




	// ワールドマトリックスの初期化
	XMMATRIX mtxWorld = XMMatrixIdentity();




	// ワールドマトリックスの設定
	renderer->SetWorldMatrix(&mtxWorld);



	// ポリゴン描画
	renderer->GetDeviceContext()->DrawIndexed(indexNum, 0, 0);	


		// ライティングを有効に
	renderer->SetLightEnable(TRUE);

	// 通常ブレンドに戻す
	renderer->SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z比較有効
	renderer->SetDepthEnable(TRUE);

}

int SquareParticle::AddParticle(XMFLOAT3 pos, float size,float alpha)
{
	for (unsigned int i = 0; i < maxPart; i++)
	{
		if (squareInfoArray[i].use == FALSE)
		{
			squareInfoArray[i].pos = pos;
			squareInfoArray[i].size = size;
			squareInfoArray[i].vertex[0].Diffuse.w = alpha;
			squareInfoArray[i].vertex[1].Diffuse.w = alpha;
			squareInfoArray[i].vertex[2].Diffuse.w = alpha;
			squareInfoArray[i].vertex[3].Diffuse.w = alpha;
			squareInfoArray[i].use = TRUE;
			return i;
		}
	}
	return -1;
}

void SquareParticle::DeleteParticle(int n)
{
	squareInfoArray[n].pos =XMFLOAT3(0.0f,0.0f,0.0f);
	squareInfoArray[n].size = 0.0f;
	squareInfoArray[n].use = FALSE;
}

void SquareParticle::UpdatePositon(int n, XMFLOAT3 pos)
{
	squareInfoArray[n].pos = pos;
}

void SquareParticle::UpdateSize(int n, float size)
{
	squareInfoArray[n].size = size;
}

void SquareParticle::SetBlend(BLEND_MODE blend)
{
	this->blend = blend;
}
