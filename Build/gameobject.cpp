#include "gameobject.h"


GameObject::GameObject()
{
	this->transformComponent = new TransformComponent;
}

GameObject::GameObject(Level* level)
{
	this->pLevel = level;
	this->transformComponent = new TransformComponent;
	
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
