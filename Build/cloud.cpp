#include "cloud.h"
#include "CloudComponent.h"

Cloud::Cloud(Level* level)
{
	pLevel = level;
	cloudComponent = new CloudComponent(this);
}

Cloud::~Cloud()
{
	delete cloudComponent;
}

void Cloud::Init(void)
{
	transformComponent->Init();
	cloudComponent->Init();

}

void Cloud::Uninit(void)
{
	transformComponent->Uninit();
	cloudComponent->Uninit();

}

void Cloud::Update(void)
{
	transformComponent->Update();
	cloudComponent->Update();

}

void Cloud::Draw(void)
{

}
