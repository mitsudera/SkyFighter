#include "MissileLauncherComponent.h"
#include "gameobject.h"
#include "transformcomponent.h"
#include "MissleList.h"

MissileLauncherComponent::MissileLauncherComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}

MissileLauncherComponent::~MissileLauncherComponent()
{
}

void MissileLauncherComponent::Init(void)
{
	this->coolTimeMax = 10;
	this->coolTimeCount = 0;
	this->isEnable = TRUE;


}

void MissileLauncherComponent::Uninit(void)
{
}

void MissileLauncherComponent::Update(void)
{
	if (!isEnable)
	{
		coolTimeCount -= 1;

		if (coolTimeCount <= 0)
		{
			isEnable = TRUE;
		}

	}

	lockOnObject = nullptr;

	float lastangle = XM_PI;

	for (int i = 0; i < TargetObjectList.size(); i++)
	{
		XMVECTOR posv = XMLoadFloat3(&pGameObject->GetTransFormComponent()->GetPosition());
		XMVECTOR dirv = XMLoadFloat3(&pGameObject->GetTransFormComponent()->GetDirection());
		XMVECTOR tarv = XMLoadFloat3(&TargetObjectList[i]->GetTransFormComponent()->GetPosition());

		XMVECTOR pt =  tarv -posv ;
		XMVECTOR lenv = XMVector3Length(pt);

		float len;
		XMStoreFloat(&len, lenv);

		

		pt = XMVector3Normalize(pt);

		XMVECTOR anglev = XMVector3AngleBetweenNormals(dirv, pt);
		float angle;
		XMStoreFloat(&angle, anglev);

		if (angle < XM_PIDIV4 * 0.33 && angle < lastangle &&  len < 30000.0f)
		{
			lockOnObject = TargetObjectList[i];
			lastangle = angle;
		}


	}


}

void MissileLauncherComponent::Launch(XMFLOAT3 pos, XMMATRIX rot, float spd)
{
	if (isEnable)
	{
		TransformComponent* transform = pGameObject->GetTransFormComponent();

		isEnable = FALSE;

		coolTimeCount = coolTimeMax;

		//‚±‚±‚É”­ŽËˆ—
		switch (parent)
		{
		case Parent::Player:
			pGameObject->GetLevel()->GetMissileList()->Launch(pos, rot, spd, lockOnObject, ObjectTag::TagPlayerMissile);
			break;

		case Parent::Enemy:
			pGameObject->GetLevel()->GetMissileList()->Launch(pos, rot, spd, lockOnObject, ObjectTag::TagEnemyMissile);
			break;

		default:
			break;
		}

	}

}

void MissileLauncherComponent::SetCoolTimeMax(int ct)
{
	this->coolTimeMax = ct;
}

void MissileLauncherComponent::SetParent(Parent p)
{
	this->parent = p;
}

void MissileLauncherComponent::SetTargetObjectList(vector<GameObject*> list)
{
	TargetObjectList = list;
}

GameObject* MissileLauncherComponent::GetTargetObject(void)
{
	return this->lockOnObject;
}
