#include "SkySphire.h"
#include "meshcomponent.h"

SkySphire::SkySphire(Level* level)
{
	this->pLevel = level;
	this->meshComponent = new MeshComponent(this);
}

SkySphire::~SkySphire()
{

}

void SkySphire::Init(void)
{
	this->transformComponent->Init();
	this->meshComponent->Init();
	this->meshComponent->SetCullMode(CULL_MODE_BACK);

	transformComponent->SetTransForm(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(100.0f, 100.0f, 100.0f));

	meshComponent->SetMeshComponent(
		"data/MODEL/mesh/skysphire.fbx");

	meshComponent->SetMeshDataList();

}

void SkySphire::Uninit(void)
{
	meshComponent->Uninit();
	transformComponent->Uninit();
}

void SkySphire::Update(void)
{
	transformComponent->Update();
	meshComponent->SetWorldMtx(this->transformComponent->GetWorldMtx());

	meshComponent->Update();	


}

void SkySphire::Draw(void)
{
	meshComponent->Draw();

}
