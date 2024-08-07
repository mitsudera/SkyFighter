#pragma once
#include "component.h"
#include "Coreminimal.h"

#define LIGHT_MAX		(8)

class LightComponent :public Component
{
public:

	LightComponent();
	LightComponent(GameObject* gameObject);
	~LightComponent();

	
	

	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	void SetIndex(int n);
	int GetIndex(void);

	void SetDiffuse(XMFLOAT4 diffuse);
	XMFLOAT4 GetDiffuse(void);

	void SetEnable(BOOL enable);
	BOOL GetEnable(void);

	XMFLOAT3 GetDirection(void);
	void SetDirecton(XMFLOAT3 forward);
	XMFLOAT3 GetPosition(void);
	void SetPosition(XMFLOAT3 pos);
	XMFLOAT4 GetAmbient(void);
	void SetAmbient(XMFLOAT4 amb);
	float GetAttenuation(void);
	void SetAttenuation(float att);

	BOOL GetType(void);

	void Set(void);

	enum LIGHT_TYPE
	{
		NOT,
		DIRECTIONAL,
		POINT,
	};

protected:

	int index;				//何番目のライトバッファに登録するか？

	XMFLOAT3	direction;	// ライトの方向
	XMFLOAT3	position;	// ライトの位置
	XMFLOAT4	diffuse;	// 拡散光の色
	XMFLOAT4	ambient;	// 環境光の色
	float		attenuation;// 減衰率
	int			type;		// ライト種別フラグ
	BOOL		enable;		// 有効フラグ


};

