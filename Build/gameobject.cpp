#include "gameobject.h"


GameObject::GameObject()
{
	pLevel=nullptr;
	this->transformComponent = new TransformComponent(this);
}

GameObject::GameObject(Level* level)
{
	this->pLevel = level;
	this->transformComponent = new TransformComponent(this);
	
}

GameObject::~GameObject()
{
	delete this->transformComponent;
}

void GameObject::Init(void)
{
}

void GameObject::Uninit(void)
{
}

void GameObject::Update(void)
{
}

void GameObject::Draw(void)
{
}

Level* GameObject::GetLevel(void)
{
	return this->pLevel;
}

TransformComponent* GameObject::GetTransFormComponent(void)
{
	return this->transformComponent;
}
