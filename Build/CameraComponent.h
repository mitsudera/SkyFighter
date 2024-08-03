//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#pragma once
#include "PrimitiveComponent.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


enum VIEWPORT_TYPE
{
	TYPE_FULL_SCREEN,
	TYPE_LEFT_HALF_SCREEN,
	TYPE_RIGHT_HALF_SCREEN,
	TYPE_UP_HALF_SCREEN,
	TYPE_DOWN_HALF_SCREEN,
	TYPE_VIEWPORT,
	TYPE_NONE,

};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
class CameraComponent : public PrimitiveComponent
{
public:
	CameraComponent();
	CameraComponent(GameObject* gameObject);
	~CameraComponent();


	void SetCameraAT(XMFLOAT3 pos);
	void SetCameraUp(XMFLOAT3 up);
	void SetLookObject(GameObject* gameObject);

	XMFLOAT4X4 CameraInverseViewMatrix(void) const;
	XMFLOAT4X4 CameraViewMatrix(void) const;


	void SetViewPort(int m_type);
	int GetViewPortType(void);

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void Uninit(void) override;

	enum class MODE
	{
		TRACKING,
		WORLD,
	};

	void SetMode(MODE mode);

private:
	void SetCamera(void);

	XMFLOAT4X4			mtxView;		// ビューマトリックス
	XMFLOAT4X4			mtxInvView;		// ビューマトリックス
	XMFLOAT4X4			mtxProjection;	// プロジェクションマトリックス


	XMFLOAT3			at;				// カメラの注視点
	XMFLOAT3			up;				// カメラの上方向ベクトル

	float				angle;			// 視野角
	float				aspect;			// アスペクト比
	float				len;			// カメラの視点と注視点の距離
	float				nearZ;			// カメラのクリッピング最小値Z
	float				farZ;			// カメラのクリッピング最大値Z

	GameObject*			lookObject;

	MODE				mode;


};