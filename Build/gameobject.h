#pragma once
#include"transformcomponent.h"
#include"main.h"



typedef enum
{
	TagNone,
	TagPlayer,
	TagEnemy,
	TagField,
	TagPlayerBullet,
	TagPlayerMissile,
	TagEnemyMissile,

	ObjectTagMax,
}ObjectTag;


class MeshComponent;
class ColliderComponent;

class GameObject
{
public:
	GameObject();
	GameObject(Level* level);
	~GameObject();

	virtual void Init(void)=0;
	virtual void Uninit(void)=0;
	virtual void Update(void)=0;
	virtual void Draw(void)=0;

	Level* GetLevel(void);
	TransformComponent* GetTransFormComponent(void);
	ColliderComponent* GetCollider(void);

	ObjectTag GetTag(void);

	BOOL GetUse(void);
	void SetUse(BOOL use);


protected:
	Level* pLevel;
	TransformComponent* transformComponent;
	ColliderComponent* collider;

	ObjectTag tag;

	BOOL use;


};

