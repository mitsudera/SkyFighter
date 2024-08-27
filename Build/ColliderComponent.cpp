#include "ColliderComponent.h"
#include "gameobject.h"
#include "CollisionManger.h"

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
	result.hitObject.clear();
	for (int i = 0; i < ObjectTag::ObjectTagMax; i++)
	{
		result.isHit[i] = FALSE;

	}

}

void ColliderComponent::Uninit(void)
{

	this->GetGameObject()->GetLevel()->GetCollisionManager()->DeleteCllider(this);

}

void ColliderComponent::Update(void)
{
	TransformComponent::Update();



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
	return pGameObject->GetTag();
}


void ColliderComponent::SetHitObject(GameObject* gameObject)
{
	this->result.hitObject.push_back(gameObject);

}

BOOL ColliderComponent::GetHitObject(GameObject* gameObject)
{
	BOOL ans = FALSE;

	if (gameObject==nullptr)
	{
		return FALSE;
	}

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
	sPos = p;
}

XMFLOAT3 ColliderComponent::GetStart(void)
{
	XMFLOAT3 ans;


	if (useOldPos)
	{
		ans = GetGameObject()->GetTransFormComponent()->GetPosition();
	}
	else
	{
		XMVECTOR v = XMLoadFloat3(&sPos);

		XMMATRIX trans = GetGameObject()->GetTransFormComponent()->GetWorldMtx();

		v = XMVector3Transform(v, trans);


		XMStoreFloat3(&ans, v);


	}
	return ans;
}

void ColliderComponent::SetEnd(XMFLOAT3 p)
{
	ePos = p;
}

XMFLOAT3 ColliderComponent::GetEnd(void)
{
	XMFLOAT3 ans;

	
	if (useOldPos)
	{
		ans = GetGameObject()->GetTransFormComponent()->GetOldPosition();
	}
	else
	{
		XMVECTOR v = XMLoadFloat3(&ePos);

		XMMATRIX trans = GetGameObject()->GetTransFormComponent()->GetWorldMtx();

		v = XMVector3Transform(v, trans);


		XMStoreFloat3(&ans, v);


	}

	return ans;
}

void ColliderComponent::Clear(void)
{
	this->result.hitObject.clear();

	for (int i = 0; i < ObjectTag::ObjectTagMax; i++)
	{
		this->result.isHit[i] = FALSE;
	}


}

void ColliderComponent::SetPointCollider(XMFLOAT3 pos)
{
	this->pos = pos;
	this->check = 0.0f;
	this->shape = Shape::Point;
}

void ColliderComponent::SetLineCollider(XMFLOAT3 pos, XMFLOAT3 spos, XMFLOAT3 epos)
{

	XMVECTOR sposv = XMLoadFloat3(&spos);
	XMVECTOR eposv = XMLoadFloat3(&epos);
	XMVECTOR posv = (eposv + sposv) * 0.5f;

	XMVECTOR len = XMVector3Length((eposv - sposv) * 0.5f);

	XMStoreFloat(&this->check, len);

	XMStoreFloat3(&this->pos, posv);



	this->sPos = spos;
	this->ePos = epos;
	this->shape = Shape::Line;

	useOldPos = FALSE;
}

void ColliderComponent::SetLineCollider(float maxSpeed)
{

	this->check = maxSpeed;

	this->shape = Shape::Line;

	useOldPos = TRUE;
}

void ColliderComponent::SetSphereCollider(XMFLOAT3 pos, float r)
{
	this->pos = pos;
	this->radius = r;
	this->check = r;
	this->shape = Shape::Sphere;
}

void ColliderComponent::SetCapsuleCollider(XMFLOAT3 pos, XMFLOAT3 spos, XMFLOAT3 epos, float r)
{
	XMVECTOR sposv = XMLoadFloat3(&spos);
	XMVECTOR eposv = XMLoadFloat3(&epos);
	XMVECTOR posv = (eposv + sposv) * 0.5f;

	XMVECTOR len = XMVector3Length((eposv - sposv) * 0.5f);

	XMStoreFloat(&this->check, len);

	XMStoreFloat3(&this->pos, posv);

	this->sPos;
	this->ePos;
	this->radius = r;
	this->shape = Shape::Capsule;
}

float ColliderComponent::GetCheckRadius(void)
{

	return this->check;
}

XMFLOAT3 ColliderComponent::GetPosition(void)
{
	XMVECTOR v = XMLoadFloat3(&pos);

	XMMATRIX trans = GetGameObject()->GetTransFormComponent()->GetWorldMtx();

	v = XMVector3Transform(v, trans);

	XMFLOAT3 ans;

	XMStoreFloat3(&ans, v);

	return ans;
}

void ColliderComponent::onCollider(void)
{
	this->Init();
	this->GetGameObject()->GetLevel()->GetCollisionManager()->AddCollider(this);


}

void ColliderComponent::offCollider(void)
{

	this->GetGameObject()->GetLevel()->GetCollisionManager()->DeleteCllider(this);

}

