#pragma once
#include "primitivecomponent.h"
#include "Renderer.h"

class MeshDataList;

class KeyFrameAnimData;

class MeshComponent :public PrimitiveComponent
{
public:
	MeshComponent();
	MeshComponent(GameObject* gameObject);
	~MeshComponent();



	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	enum class  ANIM_STATE
	{
		NO_ANIM,
		DATA_ANIM,
		BLEND_ANIM,
	};


	//void SetpGameEngine(GameEngine* gameEngine);

	string GetMeshFilePath(void);
	void SetMeshFilePath(string path);
	void SetAnimFilePath(string path);
	void SetAnimFilePathArray(vector<string> path);


	void SetMeshComponent(
		string meshFilePath,		//メッシュデータのパス
		vector<string> animFilePath,//アニメーションファイルパス
		BOOL blend,					//モーションブレンドするか？
		int blendN);				//モーションブレンディングのフレーム数
	void SetMeshComponent(string meshFilePath);		//メッシュデータのパス

	virtual void SetMeshDataList(void);
	virtual void SetAnimationArray(void);

	void CreatepAnimDataArray(int n);
	void SetpKeyFrameAnimData(int n, KeyFrameAnimData* p);

	void CreateMeshMtxArray(int n);
	void SetMeshMtxArray(int n, XMMATRIX mtx);
	void SetBlendMtxArray(void);
	virtual void SwichAnimIndex(int n);



	virtual void DrawMesh(int n);

	void SetCullMode(CULL_MODE mode);

protected:
	string meshFilePath;
	vector<string> animFilePath;

	int MeshDataListIndex;
	vector<int> AnimDataIndexArray;



	XMMATRIX* MeshMtxArray;
	int meshNum;

	XMMATRIX* BlendMeshMtxArray;
	int animindex;
	int lastanimindex;
	int framecnt;
	int framenum;
	int blendcnt;
	int blendcntmax;
	ANIM_STATE animstate;
	BOOL motionblend;
	BOOL animation;

	CULL_MODE cullMode;

	

};

