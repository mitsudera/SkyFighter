#pragma once
#include "transformcomponent.h"
#include "Coreminimal.h"




typedef enum
{
	Player,
	Enemy,
	Field,

	MAX,
}ObjectTag;



struct HitResult
{
	BOOL isHit[ObjectTag::MAX];
	vector<GameObject*> hitObject;

};

enum class Shape
{

	Point,
	Line,
	Sphere,
	Capsule,

	AABB,
	CBB,

};

class ColliderComponent :public TransformComponent
{
public:


	ColliderComponent(GameObject* gameObject);
	~ColliderComponent();


	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	BOOL GetHitTag(ObjectTag tag);

	void SetHitTag(ObjectTag tag, BOOL isHit);

	Shape GetShape(void);
	void SetShape(Shape shape);

	ObjectTag GetTag(void);
	void SetTag(ObjectTag tag);

	
	void SetHitObject(GameObject* gameObject);
	BOOL GetHitObject(GameObject* gameObject);

	void SetRadius(float r);
	float GetRadius(void);

	void SetStart(XMFLOAT3 p);
	XMFLOAT3 GetStart(void);

	void SetEnd(XMFLOAT3 p);
	XMFLOAT3 GetEnd(void);

private:
	HitResult result;
	ObjectTag tag;
	Shape shape;
	float radius;
	XMFLOAT3 sPoint;
	XMFLOAT3 ePoint;

};

