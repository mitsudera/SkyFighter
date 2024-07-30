#pragma once
#include "gameobject.h"
#include "transformcomponent.h"
#include "Movementcomponent.h"
#include "level.h"
class Player:public GameObject
{


public:
	Player(Level* level);
	~Player();

	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;


private:
	




};

