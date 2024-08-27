#include "Effect.h"
#include "level.h"
#include "SquareParticle.h"

Effect::Effect(Level* level)
{
	pLevel = level;
}

Effect::~Effect()
{
}

void Effect::Init(void)
{
	transformComponent->Init();
}

void Effect::Uninit(void)
{
	transformComponent->Uninit();
}

void Effect::Update(void)
{

	life -= 1;
	if (life<=0)
	{
		use = FALSE;
		if (isAdd)
		{
			pLevel->GetSquareParticleAdd()->DeleteParticle(partIndex);

		}
		else
		{
			pLevel->GetSquareParticleSub()->DeleteParticle(partIndex);
		}

	}

	if (!use)
	{
		return;
	}

	transformComponent->Update();

	pos.x += dir.x * spd;
	pos.y += dir.y * spd;
	pos.z += dir.z * spd;


	if (isAdd)
	{
		pLevel->GetSquareParticleAdd()->UpdatePositon(partIndex, pos);
	}
	else
	{
		pLevel->GetSquareParticleSub()->UpdatePositon(partIndex, pos);
	}

}

void Effect::Draw(void)
{

}

void Effect::SetEffectAdd(XMFLOAT3 pos, XMFLOAT3 dir, float size, float spd, int life, float alpha)
{
	use = TRUE;
	this->pos = pos;
	this->dir = dir;
	this->size = size;
	this->spd = spd;
	this->life = life;
	isAdd = TRUE;
	this->partIndex = pLevel->GetSquareParticleAdd()->AddParticle(pos,size,alpha);


}
void Effect::SetEffectSub(XMFLOAT3 pos, XMFLOAT3 dir, float size, float spd, int life,float alpha)
{
 	use = TRUE;
	this->pos = pos;
	this->dir = dir;
	this->size = size;
	this->spd = spd;
	this->life = life;
	isAdd = FALSE;
	this->partIndex = pLevel->GetSquareParticleSub()->AddParticle(pos, size, alpha);


}
