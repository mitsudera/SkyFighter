#include "MissileComponent.h"
#include "gameobject.h"
#include "transformcomponent.h"
#include "ColliderComponent.h"

#define LIFE (300)

MissileComponent::MissileComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}

MissileComponent::~MissileComponent()
{
}

void MissileComponent::Init(void)
{
	target = nullptr;
	spd = 0.0f;
	spdmax = 300.0f;
	spdup = 1.0f;
	spdupmax = 3.0f;
	spdupvalue = 0.002f;
	rotValue = (XM_PI / 180) * 1.0f;
	pGameObject->SetUse(FALSE);
	life = 0;
}

void MissileComponent::Uninit(void)
{
}

void MissileComponent::Update(void)
{

	life -= 1;

	if (life<=0)
	{
		pGameObject->SetUse(FALSE);
		pGameObject->GetCollider()->offCollider();

	}



	TransformComponent* transform = pGameObject->GetTransFormComponent();



	spdup += spdupvalue;
	if (spdup > spdupmax)
	{
		spdup = spdupmax;
	}
	spd += spdup;
	transform->MoveForward(spd);


	if (target != nullptr)
	{
		XMFLOAT3 pos = transform->GetPosition();
		XMFLOAT3 tpos = target->GetTransFormComponent()->GetPosition();

		XMVECTOR ptv = -XMLoadFloat3(&tpos) +XMLoadFloat3(&pos);

		if (!XMVector3Equal(ptv,XMVectorZero()))
		{
			XMVECTOR dv = XMLoadFloat3(&transform->GetDirection());

			ptv = XMVector3Normalize(ptv);
			dv = XMVector3Normalize(dv);
			float dot;
			XMStoreFloat(&dot,XMVector3Dot(ptv, dv));

			if (!XMVector3Equal(ptv, dv)&&dot!=-1.0f)
			{
				XMVECTOR axis = XMVector3Cross(ptv, dv);
				float angle;
				XMStoreFloat(&angle, XMVector3AngleBetweenNormals(ptv, dv));

				if (angle > rotValue)
				{
					angle = rotValue;
				}

				transform->RotAxisAngle(axis, angle);

			}

		}



	}



}

void MissileComponent::SetSpeed(float f)
{
	spd = f;
}

void MissileComponent::SetSpeedMax(float f)
{
	spdmax = f;
}

void MissileComponent::SetSpeedUp(float f)
{
	spdup = f;
}

void MissileComponent::SetSpeedUpMax(float f)
{
	spdupmax = f;
}

void MissileComponent::SetSpeedUpValue(float f)
{
	spdupvalue = f;
}

void MissileComponent::SetRotValue(float f)
{
	rotValue = f;
}

void MissileComponent::Launch(XMFLOAT3 pos, XMMATRIX rot, float spd, GameObject* target)
{

	this->Init();
	this->pGameObject->GetTransFormComponent()->SetPosition(pos);

	this->pGameObject->GetTransFormComponent()->SetMtxRot(rot);

	this->spd = spd;
	pGameObject->SetUse(TRUE);
	life = LIFE;

	pGameObject->GetCollider()->onCollider();

	this->target = target;

}

BOOL MissileComponent::GetUse(void)
{
	return pGameObject->GetUse();
}

