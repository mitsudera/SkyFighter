#include "BlueField.h"
#include "meshcomponent.h"

BlueField::BlueField(Level* level)
{
	this->pLevel = level;
	this->mesh = new MeshComponent(this);

}

BlueField::~BlueField()
{
	delete mesh;
}

void BlueField::Init(void)
{
	this->transformComponent->Init();
	this->mesh->Init();

	transformComponent->SetTransForm(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));

	mesh->SetMeshComponent(
		"data/MODEL/mesh/blueplane.fbx");

	mesh->SetMeshDataList();

}

void BlueField::Uninit(void)
{

	transformComponent->Uninit();

	mesh->Uninit();

}

void BlueField::Update(void)
{
	transformComponent->Update();
	mesh->SetWorldMtx(this->transformComponent->GetWorldMtx());

	mesh->Update();

}

void BlueField::Draw(void)
{
	mesh->Draw();

}
