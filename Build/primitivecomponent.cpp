#include "primitivecomponent.h"

PrimitiveComponent::PrimitiveComponent()
{
}

PrimitiveComponent::PrimitiveComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

PrimitiveComponent::~PrimitiveComponent()
{
}

void PrimitiveComponent::Init(void)
{
	TransformComponent::Init();
}

void PrimitiveComponent::Uninit(void)
{
}

void PrimitiveComponent::Update(void)
{
}

void PrimitiveComponent::Draw(void)
{
}
