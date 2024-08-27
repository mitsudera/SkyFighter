#include "MIssleList.h"
#include "Missile.h"
#include "MissileComponent.h"

#define MAX (256)


MissleList::MissleList(Level* level)
{
	pLevel = level;

	for (int i = 0; i < MAX; i++)
	{
		Missile* missile = new Missile(level);
		missileArray.push_back(missile);
	}
}

MissleList::~MissleList()
{
	for (int i = 0; i < MAX; i++)
	{
		delete missileArray[i];
	}
}

void MissleList::Init(void)
{
	for (int i = 0; i < MAX; i++)
	{
		missileArray[i]->Init();
	}

}

void MissleList::Uninit(void)
{
	for (int i = 0; i < MAX; i++)
	{
		missileArray[i]->Uninit();
	}

}

void MissleList::Update(void)
{
	for (int i = 0; i < MAX; i++)
	{
		missileArray[i]->Update();
	}

}

void MissleList::Draw(void)
{

	for (int i = 0; i < MAX; i++)
	{
		missileArray[i]->Draw();
	}

}

void MissleList::Launch(XMFLOAT3 pos, XMMATRIX rot, float spd, GameObject* target, ObjectTag tag)
{
	for (int i = 0; i < MAX; i++)
	{
		if (!missileArray[i]->GetMissileComponent()->GetUse())
		{
			missileArray[i]->Launch(pos, rot, spd, target, tag);
			return;
		}
	}
}
