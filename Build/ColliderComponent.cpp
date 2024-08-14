#include "ColliderComponent.h"

ColliderComponent::ColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;

	
}

ColliderComponent::~ColliderComponent()
{

}

void ColliderComponent::Init(void)
{
	TransformComponent::Init();
}

void ColliderComponent::Uninit(void)
{

}

void ColliderComponent::Update(void)
{
	TransformComponent::Update();

	this->result.hitObject.clear();

}

BOOL ColliderComponent::GetHitTag(ObjectTag tag)
{
	return result.isHit[tag];
}

void ColliderComponent::SetHitTag(ObjectTag tag, BOOL isHit)
{
	this->result.isHit[tag] = isHit;
}

Shape ColliderComponent::GetShape(void)
{
	return this->shape;

}

void ColliderComponent::SetShape(Shape shape)
{
	this->shape = shape;
}

ObjectTag ColliderComponent::GetTag(void)
{
	return this->tag;
}

void ColliderComponent::SetTag(ObjectTag tag)
{
	this->tag = tag;
}

void ColliderComponent::SetHitObject(GameObject* gameObject)
{
	this->result.hitObject.push_back(gameObject);

}

BOOL ColliderComponent::GetHitObject(GameObject* gameObject)
{
	BOOL ans = FALSE;

	for (int i = 0; i < result.hitObject.size(); i++)
	{
		if (result.hitObject[i]==gameObject)
		{
			ans = TRUE;
		}
	}


	return ans;
}

void ColliderComponent::SetRadius(float r)
{
	this->radius = r;
}

float ColliderComponent::GetRadius(void)
{
	return this->radius;
}

void ColliderComponent::SetStart(XMFLOAT3 p)
{
	sPoint = p;
}

XMFLOAT3 ColliderComponent::GetStart(void)
{
	return sPoint;
}

void ColliderComponent::SetEnd(XMFLOAT3 p)
{
	ePoint = p;
}

XMFLOAT3 ColliderComponent::GetEnd(void)
{
	return ePoint;
}
