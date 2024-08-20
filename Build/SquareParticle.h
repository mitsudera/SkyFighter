#pragma once
#include "gameobject.h"
#include "Renderer.h"


struct SquareInfo
{
	VERTEX_3D vertex[4];

	XMFLOAT3 pos;
	float size;

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

private:

	unsigned int maxPart;
	unsigned int vertNum;
	unsigned int indexNum;
	unsigned int *indexArray;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	SquareInfo* squareInfoArray;

};

