#include "BulletList.h"
#include "PlayerBullet.h"
#include "level.h"
#include "BulletComponent.h"

#define BULLET_MAX (1024)


BulletList::BulletList(Level* level)
{
	this->pLevel = level;
	playerBulletArray = new PlayerBullet[BULLET_MAX];

	for (int i = 0; i < BULLET_MAX; i++)
	{
		playerBulletArray[i].SetLevel(level);
	}


}

BulletList::~BulletList()
{
	delete[] playerBulletArray;
}

void BulletList::Init(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		playerBulletArray[i].Init();
	}


}

void BulletList::Uninit(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		playerBulletArray[i].Uninit();
	}

}

void BulletList::Update(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		playerBulletArray[i].Update();
	}

}

void BulletList::Draw(void)
{
}

void BulletList::SetPlayerBullet(XMFLOAT3 pos, XMFLOAT3 dir, float spd)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (this->playerBulletArray[i].GetBullet()->GetUse() == FALSE)
		{
			this->playerBulletArray[i].SetBullet(pos, dir, spd);
			return;
		}

	}


}
