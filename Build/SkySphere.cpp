#include "SkySphere.h"
#include "meshcomponent.h"

SkySphere::SkySphere(Level* level)
{
	this->pLevel = level;
	this->meshComponent = new MeshComponent(this);
}

SkySphere::~SkySphere()
{

}

void SkySphere::Init(void)
{
	this->transformComponent->Init();
	this->meshComponent->Init();
	this->meshComponent->SetCullMode(CULL_MODE_BACK);

	transformComponent->SetTransForm(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(10000.0f, 10000.0f, 10000.0f));

	meshComponent->SetMeshComponent(
		"data/MODEL/mesh/skysphire.fbx");

	meshComponent->SetMeshDataList();

}

void SkySphere::Uninit(void)
{
	meshComponent->Uninit();
	transformComponent->Uninit();
}

void SkySphere::Update(void)
{
	transformComponent->Update();

	meshComponent->Update();	


}

void SkySphere::Draw(void)
{
	meshComponent->Draw();

}
