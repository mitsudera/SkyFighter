#pragma once

class GameObject;

class Component
{
public:
	Component();
	Component(GameObject* gameObject);
	~Component();

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;

	GameObject* GetGameObject(void);

protected:
	GameObject* pGameObject;

};

