#pragma once
#include "gameobject.h"
class Effect : public GameObject
{
public:
	Effect(Level* level);
	~Effect();
	// GameObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;

	enum class EffectType
	{
		Contrail,
		Bomb,

	};

	void SetEffectAdd(
		XMFLOAT3 pos,
		XMFLOAT3 dir,
		float size,
		float spd,
		int life, 
		float alpha);

	void SetEffectSub(
		XMFLOAT3 pos,
		XMFLOAT3 dir,
		float size,
		float spd,
		int life, 
		float alpha);
	

private:
	
	XMFLOAT3 pos;
	XMFLOAT3 dir;

	float size;
	float spd;



	BOOL isAdd;

	int life;

	int partIndex;

	SquareParticle* particle;


};

