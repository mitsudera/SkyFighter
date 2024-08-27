#include "EffectList.h"
#include "Effect.h"

#define MAX (16384)
#define CONTRAIL_NUM (20)
#define CONTRAIL_SIZE (1.0f)
#define CONTRAIL_SPD (0.1f)
#define CONTRAIL_LIFE (15)
#define CONTRAIL_ALPHA (0.1f)

#define BOMB_NUM (300)
#define BOMB_SIZE (10.0f)
#define BOMB_SPD (1.0f)
#define BOMB_LIFE (60)
#define BOMB_ALPHA (0.1f)


EffectList::EffectList(Level* level)
{
	pLevel = level;

	for (int i = 0; i < MAX; i++)
	{
		Effect* effect = new Effect(level);
		effectArray.push_back(effect);
	}


}

EffectList::~EffectList()
{
	for (int i = 0; i < MAX; i++)
	{
		
		delete effectArray[i];
	}

}

void EffectList::Init(void)
{
	for (int i = 0; i < MAX; i++)
	{
		effectArray[i]->Init();
	}

}

void EffectList::Uninit(void)
{
	for (int i = 0; i < MAX; i++)
	{
		effectArray[i]->Uninit();
	}

}

void EffectList::Update(void)
{
	for (int i = 0; i < MAX; i++)
	{
		effectArray[i]->Update();
	}

}

void EffectList::Draw(void)
{
	for (int i = 0; i < MAX; i++)
	{
		effectArray[i]->Draw();
	}

}

void EffectList::SetContrail(XMFLOAT3 pos, XMFLOAT3 opos)
{
	for (int i = 0; i < CONTRAIL_NUM; i++)
	{


		XMFLOAT3 pop;

		pop.x = opos.x + (((pos.x - opos.x) / CONTRAIL_NUM) * i);
		pop.y = opos.y + (((pos.y - opos.y) / CONTRAIL_NUM) * i);
		pop.z = opos.z + (((pos.z - opos.z) / CONTRAIL_NUM) * i);
		
		XMFLOAT2 moveangle;

		moveangle.x = (rand() % 618) * 0.01f;
		moveangle.y = (rand() % 618) * 0.01f;

		XMFLOAT3 dir;

		dir.x = sinf(moveangle.y) * cosf(moveangle.x);
		dir.z = cosf(moveangle.y) * cosf(moveangle.x);
		dir.y = sinf(moveangle.x);


		for (int j = 0; j < MAX; j++)
		{
			if (!effectArray[j]->GetUse())
			{
				effectArray[j]->SetEffectAdd(pop, dir, CONTRAIL_SIZE, CONTRAIL_SPD, CONTRAIL_LIFE,CONTRAIL_ALPHA);
				break;
			}

		}

	}


}
void EffectList::SetBomb(XMFLOAT3 pos)
{
	for (int i = 0; i < BOMB_NUM; i++)
	{


		XMFLOAT2 moveangle;

		moveangle.x = (rand() % 618) * 0.01f;
		moveangle.y = (rand() % 618) * 0.01f;

		XMFLOAT3 dir;

		dir.x = sinf(moveangle.y) * cosf(moveangle.x);
		dir.z = cosf(moveangle.y) * cosf(moveangle.x);
		dir.y = sinf(moveangle.x);


		for (int j = 0; j < MAX; j++)
		{
			if (!effectArray[j]->GetUse())
			{
				effectArray[j]->SetEffectSub(pos, dir, BOMB_SIZE, BOMB_SPD, BOMB_LIFE,BOMB_ALPHA);
				break;
			}

		}

	}


}
