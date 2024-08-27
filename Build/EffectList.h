#pragma once
#include "gameobject.h"

class Effect;

class EffectList : public GameObject
{
public:
	EffectList(Level* level);
	~EffectList();
	// GameObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;

	void SetContrail(XMFLOAT3 pos, XMFLOAT3 opos);

	void SetBomb(XMFLOAT3 pos);


private:
	
	vector<Effect*> effectArray;

};

