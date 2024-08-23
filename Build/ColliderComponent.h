#pragma once
#include "transformcomponent.h"
#include "Coreminimal.h"



typedef enum
{
	TagPlayer,
	TagEnemy,
	TagField,
	TagPlayerBullet,

	ObjectTagMax,
}ObjectTag;

struct HitResult
{
	BOOL isHit[ObjectTag::ObjectTagMax];
	vector<GameObject*> hitObject;

};



enum class Shape
{

	Point,
	Line,
	Sphere,
	Capsule,


	//–¢ŽÀ‘•
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

	void Clear(void);

	void SetPointCollider(XMFLOAT3 pos);
	void SetLineCollider(XMFLOAT3 pos, XMFLOAT3 spos, XMFLOAT3 epos);
	void SetLineCollider(float maxSpeed);
	void SetSphereCollider(XMFLOAT3 pos, float r);
	void SetCapsuleCollider(XMFLOAT3 pos, XMFLOAT3 spos, XMFLOAT3 epos, float r);
	float GetCheckRadius(void);

	virtual XMFLOAT3 GetPosition(void) override;

	void onCollider(void);
	void offCollider(void);


private:
	HitResult result;
	ObjectTag tag;
	Shape shape;
	float radius;
	XMFLOAT3 sPos;
	XMFLOAT3 ePos;
	float check;

	BOOL useOldPos;

};

