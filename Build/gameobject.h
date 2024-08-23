#pragma once
#include"transformcomponent.h"
#include"main.h"


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

protected:
	Level* pLevel;
	TransformComponent* transformComponent;
	ColliderComponent* collider;

	

};

