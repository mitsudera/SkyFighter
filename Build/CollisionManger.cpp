#include "CollisionManger.h"
#include "collision.h"

CollisionManger::CollisionManger(Level* level)
{
	this->level = level;

}

CollisionManger::~CollisionManger()
{
}

void CollisionManger::Init(void)
{
}

void CollisionManger::Uninit(void)
{
}

void CollisionManger::Update(void)
{

	for (int i = 0; i < colliderList.size(); i++)
	{
		colliderList[i]->Clear();
	}


	for (int i = 0; i < pairList.size(); i++)
	{
		
		BOOL ans = CheckCillision(pairList[i]);



		pairList[i].collider1->SetHitTag(pairList[i].collider2->GetTag(), ans);
		pairList[i].collider2->SetHitTag(pairList[i].collider1->GetTag(), ans);

		if (ans==TRUE)
		{
			pairList[i].collider1->SetHitObject(pairList[i].collider2->GetGameObject());
			pairList[i].collider2->SetHitObject(pairList[i].collider1->GetGameObject());

		}



	}

	
}

void CollisionManger::AddCollider(ColliderComponent* collider)
{

	for (int i = 0; i < colliderList.size(); i++)
	{
		ColliderPair pair;

		pair.collider1 = colliderList[i];
		pair.collider2 = collider;

		pairList.push_back(pair);
	}

	colliderList.push_back(collider);

}

void CollisionManger::DeleteCllider(ColliderComponent* collider)
{
	for (int i = 0; i < pairList.size(); i++)
	{
		if (pairList[i].collider1 == collider || pairList[i].collider2 == collider)
		{
			pairList.erase(pairList.begin()+i);
		}

	}

	for (int i = 0; i < colliderList.size(); i++)
	{
		if (colliderList[i] == collider)
		{
			colliderList.erase(colliderList.begin()+i);
		}

	}


}

BOOL CollisionManger::CheckCillision(ColliderPair pair)
{
	XMFLOAT3 pos1 = pair.collider1->GetWorldPos();
	XMFLOAT3 pos2 = pair.collider2->GetWorldPos();

	float check1 = pair.collider1->GetCheckRadius();
	float check2 = pair.collider2->GetCheckRadius();

	//‚±‚±‚Å‘å‚Ü‚©‚É“–‚½‚Á‚Ä‚¢‚é‚©”»’è
	if (CollisionSphereSphere(pos1, check1, pos2, check2) == FALSE)
	{
		return FALSE;
	}


	XMFLOAT3 rot1 = pair.collider1->GetRotation();
	XMFLOAT3 rot2 = pair.collider2->GetRotation();

	XMFLOAT3 scl1 = pair.collider1->GetScale();
	XMFLOAT3 scl2 = pair.collider2->GetScale();

	float r1 = pair.collider1->GetRadius();
	float r2 = pair.collider2->GetRadius();



	XMFLOAT3 sPoint1 = pair.collider1->GetStart();
	XMFLOAT3 sPoint2 = pair.collider2->GetStart();

	XMFLOAT3 ePoint1 = pair.collider1->GetEnd();
	XMFLOAT3 ePoint2 = pair.collider2->GetEnd();





	if (pair.collider1->GetShape() == Shape::Point)
	{
		if (pair.collider2->GetShape() == Shape::Point)
		{
			return FALSE;
		}
		else if (pair.collider2->GetShape() == Shape::Line)
		{
			return FALSE;
		}
		else if (pair.collider2->GetShape() == Shape::Sphere)
		{
			return CollisionPointSphere(pos1,pos2,r2);
		}
		else if (pair.collider2->GetShape() == Shape::Capsule)
		{
			return CollisionPointCapsule(pos1,sPoint2,ePoint2,r2);
		}
		
	}
	else if (pair.collider1->GetShape() == Shape::Line)
	{
		if (pair.collider2->GetShape() == Shape::Point)
		{
			return FALSE;
		}
		else if (pair.collider2->GetShape() == Shape::Line)
		{
			return FALSE;
		}
		else if (pair.collider2->GetShape() == Shape::Sphere)
		{
			return CollisionLineSphere(sPoint1,ePoint1,pos2,r2);
		}
		else if (pair.collider2->GetShape() == Shape::Capsule)
		{
			return CollisionLineCapsule(sPoint1, ePoint1,sPoint2,ePoint2,r2);
		}
		
	}
	else if (pair.collider1->GetShape() == Shape::Sphere)
	{
		if (pair.collider2->GetShape() == Shape::Point)
		{
			return CollisionPointSphere(pos2,pos1,r1);
		}
		else if (pair.collider2->GetShape() == Shape::Line)
		{
			return CollisionLineSphere(sPoint2, ePoint2, pos1, r1);
		}
		else if (pair.collider2->GetShape() == Shape::Sphere)
		{
			return CollisionSphereSphere(pos1, r1, pos2, r2);
		}
		else if (pair.collider2->GetShape() == Shape::Capsule)
		{
			return CollisionSphereCapsule(pos1, r1, sPoint2, ePoint2, r2);
		}
		
	}
	else if (pair.collider1->GetShape() == Shape::Capsule)
	{
		if (pair.collider2->GetShape() == Shape::Point)
		{
			return CollisionPointCapsule(pos2, sPoint1, ePoint1, r1);
		}
		else if (pair.collider2->GetShape() == Shape::Line)
		{
			return CollisionLineCapsule(sPoint2, ePoint2, sPoint1, ePoint1, r1);
		}
		else if (pair.collider2->GetShape() == Shape::Sphere)
		{
			return CollisionSphereCapsule(pos2, r2, sPoint1, ePoint1, r1);
		}
		else if (pair.collider2->GetShape() == Shape::Capsule)
		{
			return CollisionCapsuleCapsule(sPoint1, ePoint1, r1, sPoint2, ePoint2, r2);
		}
		
	}

	return FALSE;
}

