#pragma once
#include "meshcomponent.h"
#include "Renderer.h"

struct MeshVertex
{
	ID3D11Buffer* m_vertexBuffer;
	VERTEX_3D* vertexArray;
	int vertNum;
};

class SkinMeshComponent :public MeshComponent
{

public:
	SkinMeshComponent();
	SkinMeshComponent(GameObject* gameObject);
	~SkinMeshComponent();


	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;
	
	virtual void Draw(void) override;
	virtual void DrawSkinMesh(int n) ;

	void SetSkinMeshStatus(
		string meshfilepath,
		vector<string> animfilepatharray,
		BOOL blend,
		int blendframe);

	void SetSkinMeshStatus(string meshfilepath);


	virtual void SetSkinMesh(void);

	void SetBlendBone(void);

	virtual void SwichAnimIndex(int n) override;



private:



	XMMATRIX* boneMtx;
	int boneCnt;
	XMMATRIX* blendBoneMtx;
	MeshVertex* meshVertex;



};

