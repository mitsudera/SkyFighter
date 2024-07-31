#include "transformcomponent.h"
#include "math.h"
#include"gameobject.h"

TransformComponent::TransformComponent()
{
	this->pos = { 0.0f,0.0f,0.0f };
	this->oldPos = { 0.0f, 0.0f, 0.0f };
	this->rot = { 0.0f,0.0f,0.0f };
	this->oldRot = { 0.0f,0.0f,0.0f };
	this->scl = { 1.0f,1.0f,1.0f };
	this->dir = { 0.0f,0.0f,-1.0f };
	this->axisX = xonevec();
	this->axisY = yonevec();
	this->axisZ = zonevec();
	this->qtonX = XMQuaternionRotationAxis(axisX, 0.0f);
	this->qtonY = XMQuaternionRotationAxis(axisY, XM_PI);
	this->qtonZ = XMQuaternionRotationAxis(axisZ, XM_PI);

	this->mtxpos = XMMatrixIdentity();
	this->mtxscl = XMMatrixIdentity();
	this->mtxrot = XMMatrixIdentity();
	this->mtxrotx=XMMatrixRotationQuaternion(qtonX);
	this->mtxroty=XMMatrixRotationQuaternion(qtonY);
	this->mtxrotz=XMMatrixRotationQuaternion(qtonZ);
	this->mtxWorld = XMMatrixIdentity();


}

TransformComponent::TransformComponent(GameObject* gameObject)
{
	this->pos = { 0.0f,0.0f,0.0f };
	this->oldPos = { 0.0f, 0.0f, 0.0f };
	this->rot = { 0.0f,0.0f,0.0f };
	this->oldRot = { 0.0f,0.0f,0.0f };
	this->scl = { 1.0f,1.0f,1.0f };
	this->dir = { 0.0f,0.0f,-1.0f };
	this->axisX = xonevec();
	this->axisY = yonevec();
	this->axisZ = zonevec();
	this->qtonX = XMQuaternionRotationAxis(axisX, 0.0f);
	this->qtonY = XMQuaternionRotationAxis(axisY, XM_PI);
	this->qtonZ = XMQuaternionRotationAxis(axisZ, XM_PI);

	this->mtxpos = XMMatrixIdentity();
	this->mtxscl = XMMatrixIdentity();
	this->mtxrot = XMMatrixIdentity();
	this->mtxrotx = XMMatrixRotationQuaternion(qtonX);
	this->mtxroty = XMMatrixRotationQuaternion(qtonY);
	this->mtxrotz = XMMatrixRotationQuaternion(qtonZ);
	this->mtxWorld = XMMatrixIdentity();


	this->pGameObject = gameObject;
}

TransformComponent::~TransformComponent()
{
}
void TransformComponent::Init(void)
{
}

void TransformComponent::Uninit(void)
{
}

void TransformComponent::Update(void)
{


	XMMATRIX world;
	world = XMMatrixIdentity();
	world = XMMatrixMultiply(world, mtxscl);
	world = XMMatrixMultiply(world, mtxrot);
	world = XMMatrixMultiply(world, mtxpos);

	this->mtxWorld = world;


	this->oldPos = pos;
	this->oldRot = rot;


}

XMFLOAT3 TransformComponent::GetPosition(void)
{
	return this->pos;
}

XMFLOAT3 TransformComponent::GetOldPosition(void)
{
	return this->oldPos;
}

XMFLOAT3 TransformComponent::GetRotation(void)
{
	return this->rot;
}

XMFLOAT3 TransformComponent::GetOldRotation(void)
{
	return this->oldRot;
}

XMFLOAT3 TransformComponent::GetScale(void)
{
	return this->scl;
}

XMFLOAT3 TransformComponent::GetDirection(void)
{
	return this->dir;
}

XMVECTOR TransformComponent::GetAxisX(void)
{
	return this->axisX;
}

XMVECTOR TransformComponent::GetAxizY(void)
{
	return this->axisY;
}

XMVECTOR TransformComponent::GetAxizZ(void)
{
	return this->axisZ;
}

XMVECTOR TransformComponent::GetQtonX(void)
{
	return this->qtonX;
}

XMVECTOR TransformComponent::GetQtonY(void)
{
	return this->qtonY;
}

XMVECTOR TransformComponent::GetQtonZ(void)
{
	return this->qtonZ;
}

XMMATRIX TransformComponent::GetMtxRot(void)
{
	return this->mtxrot;
}

XMMATRIX TransformComponent::GetMtxRotX(void)
{
	return this->mtxrotx;
}

XMMATRIX TransformComponent::GetMtxRotY(void)
{
	return this->mtxroty;
}

XMMATRIX TransformComponent::GetMtxRotZ(void)
{
	return this->mtxrotz;
}

XMMATRIX TransformComponent::GetWorldMtx(void)
{
	return this->mtxWorld;
}

XMMATRIX TransformComponent::GetWorldMtxWithParent(void)
{
	XMMATRIX pMtx = this->GetGameObject()->GetTransFormComponent()->GetWorldMtx();
	XMMATRIX lMtx = this->GetWorldMtx();

	lMtx = XMMatrixMultiply(lMtx, pMtx);
	
	return lMtx;
}

void TransformComponent::SetPosition(XMFLOAT3 pos)
{
	this->pos = pos;
	this->mtxpos = XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void TransformComponent::SetRotation(XMFLOAT3 rot)
{
	this->rot = rot;
	this->mtxrot = XMMatrixRotationRollPitchYaw(rot.x,rot.y,rot.z);
}

void TransformComponent::SetScale(XMFLOAT3 scl)
{
	this->scl = scl;
	this->mtxscl = XMMatrixScaling(scl.x, scl.y, scl.z);
}

void TransformComponent::SetDirection(XMFLOAT3 dir)
{
	this->dir = dir;
}

void TransformComponent::SetAxisX(XMVECTOR axis)
{
	this->axisX = axis;
}

void TransformComponent::SetAxizY(XMVECTOR axis)
{
	this->axisY = axis;
}

void TransformComponent::SetAxizZ(XMVECTOR axis)
{
	this->axisZ = axis;
}

void TransformComponent::SetQtonX(XMVECTOR qton)
{
	this->qtonX = qton;
}

void TransformComponent::SetQtonY(XMVECTOR qton)
{
	this->qtonY = qton;
}

void TransformComponent::SetQtonZ(XMVECTOR qton)
{
	this->qtonZ = qton;
}

void TransformComponent::SetMtxRot(XMMATRIX mtx)
{
	this->mtxrot = mtx;
}

void TransformComponent::SetMtxRotX(XMMATRIX mtx)
{
	this->mtxrotx = mtx;
}

void TransformComponent::SetMtxRotY(XMMATRIX mtx)
{
	this->mtxroty = mtx;
}

void TransformComponent::SetMtxRotZ(XMMATRIX mtx)
{
	this->mtxrotz = mtx;
}

void TransformComponent::SetWorldMtx(XMMATRIX mtx)
{
	this->mtxWorld = mtx;
}

void TransformComponent::SetTransForm(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl)
{
	this->SetPosition(pos);
	this->SetRotation(rot);
	this->SetScale(scl);
}

XMFLOAT3 TransformComponent::GetWorldPos(void)
{
	XMMATRIX pMtx = this->GetGameObject()->GetTransFormComponent()->GetWorldMtx();
	XMFLOAT3 lPos = this->pos;

	XMVECTOR wPos = XMLoadFloat3(&lPos);
	wPos = XMVector3Transform(wPos,pMtx);

	XMStoreFloat3(&lPos, wPos);
	
	return lPos;


}




