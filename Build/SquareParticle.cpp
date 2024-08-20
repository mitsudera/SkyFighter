#include "SquareParticle.h"

#define PART_MAX (1024)


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

}

SquareParticle::~SquareParticle()
{
	delete[] squareInfoArray;
}

void SquareParticle::Init(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * vertNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetLevel()->GetMain()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBuffer);

	// インデックスバッファ生成
	
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned int) * indexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetLevel()->GetMain()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->indexBuffer);


	for (int i = 0; i < maxPart; i++)
	{
		indexArray[i * 6] = i * 4;
		indexArray[i * 6 + 1] = i * 4 + 1;
		indexArray[i * 6 + 2] = i * 4 + 2;
		indexArray[i * 6 + 3] = i * 4 + 1;
		indexArray[i * 6 + 4] = i * 4 + 2;
		indexArray[i * 6 + 5] = i * 4 + 3;
	}


	D3D11_MAPPED_SUBRESOURCE msr;
	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Map(this->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	unsigned int* pIdx = (unsigned int*)msr.pData;

	memcpy(pIdx, indexArray, sizeof(unsigned int) * indexNum);

	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Unmap(this->indexBuffer, 0);





	for (int i = 0; i < maxPart; i++)
	{
		squareInfoArray[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		squareInfoArray[i].size = 0.0f;
		for (int j = 0; j < 4; j++)
		{
			squareInfoArray[i].vertex[j].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			squareInfoArray[i].vertex[j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			squareInfoArray[i].vertex[j].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
			squareInfoArray[i].vertex[j].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

	}
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
}

void SquareParticle::Draw(void)
{
}

