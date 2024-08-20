#pragma once
#include "Coreminimal.h"
#include "ColliderComponent.h"

class Level;
class ColliderComponent;

struct ColliderPair
{
	ColliderComponent* collider1;
	ColliderComponent* collider2;

};

class CollisionManger
{
public:
	CollisionManger(Level* level);
	~CollisionManger();

	void Init(void);
	void Uninit(void);
	void Update(void);

	void AddCollider(ColliderComponent* collider);

	void DeleteCllider(ColliderComponent* collider);


	BOOL CheckCillision(ColliderPair pair);


private:
	Level* level;

	vector<ColliderComponent*> colliderList;
	vector<ColliderPair> pairList;

};

