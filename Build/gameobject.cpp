#include "gameobject.h"


GameObject::GameObject()
{
	this->transformComponent = new Transformcomponent;
}

GameObject::GameObject(Level* level)
{
	this->pLevel = level;
	
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
