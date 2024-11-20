#pragma once
#include "gameobject.h"
#include "Renderer.h"


struct SquareInfo
{
	VERTEX_3D vertex[4];

	XMFLOAT3 pos;
	float size;

	BOOL use;
};

class SquareParticle :public GameObject
{
public:
	SquareParticle(Level* level);
	~SquareParticle();


	// GameObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;

	int AddParticle(XMFLOAT3 pos, float size, float alpha);

	void DeleteParticle(int n);

	void UpdatePositon(int n, XMFLOAT3 pos);
	void UpdateSize(int n, float size);

	void SetBlend(BLEND_MODE blend);

private:

	unsigned int maxPart;
	unsigned int vertNum;
	unsigned int indexNum;
	unsigned int *indexArray;
	VERTEX_3D* vertexArray;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	SquareInfo* squareInfoArray;

	ID3D11ShaderResourceView* texture;

	BLEND_MODE blend;


};

