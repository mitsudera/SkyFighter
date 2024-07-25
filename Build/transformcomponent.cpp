#include "transformcomponent.h"
#include "math.h"
Transformcomponent::Transformcomponent()
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

Transformcomponent::~Transformcomponent()
{
}
void Transformcomponent::Init(void)
{
}

void Transformcomponent::Uninit(void)
{
}

void Transformcomponent::Update(void)
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

XMFLOAT3 Transformcomponent::GetPosition(void)
{
	return this->pos;
}

XMFLOAT3 Transformcomponent::GetOldPosition(void)
{
	return this->oldPos;
}

XMFLOAT3 Transformcomponent::GetRotation(void)
{
	return this->rot;
}

XMFLOAT3 Transformcomponent::GetOldRotation(void)
{
	return this->oldRot;
}

XMFLOAT3 Transformcomponent::GetScale(void)
{
	return this->scl;
}

XMFLOAT3 Transformcomponent::GetDirection(void)
{
	return this->dir;
}

XMVECTOR Transformcomponent::GetAxisX(void)
{
	return this->axisX;
}

XMVECTOR Transformcomponent::GetAxizY(void)
{
	return this->axisY;
}

XMVECTOR Transformcomponent::GetAxizZ(void)
{
	return this->axisZ;
}

XMVECTOR Transformcomponent::GetQtonX(void)
{
	return this->qtonX;
}

XMVECTOR Transformcomponent::GetQtonY(void)
{
	return this->qtonY;
}

XMVECTOR Transformcomponent::GetQtonZ(void)
{
	return this->qtonZ;
}

XMMATRIX Transformcomponent::GetMtxRot(void)
{
	return this->mtxrot;
}

XMMATRIX Transformcomponent::GetMtxRotX(void)
{
	return this->mtxrotx;
}

XMMATRIX Transformcomponent::GetMtxRotY(void)
{
	return this->mtxroty;
}

XMMATRIX Transformcomponent::GetMtxRotZ(void)
{
	return this->mtxrotz;
}

XMMATRIX Transformcomponent::GetWorldMtx(void)
{
	return this->mtxWorld;
}

void Transformcomponent::SetPosition(XMFLOAT3 pos)
{
	this->pos = pos;
	this->mtxpos = XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void Transformcomponent::SetRotation(XMFLOAT3 rot)
{
	this->rot = rot;
	this->mtxrot = XMMatrixRotationRollPitchYaw(rot.x,rot.y,rot.z);
}

void Transformcomponent::SetScale(XMFLOAT3 scl)
{
	this->scl = scl;
	this->mtxscl = XMMatrixScaling(scl.x, scl.y, scl.z);
}

void Transformcomponent::SetDirection(XMFLOAT3 dir)
{
	this->dir = dir;
}

void Transformcomponent::SetAxisX(XMVECTOR axis)
{
	this->axisX = axis;
}

void Transformcomponent::SetAxizY(XMVECTOR axis)
{
	this->axisY = axis;
}

void Transformcomponent::SetAxizZ(XMVECTOR axis)
{
	this->axisZ = axis;
}

void Transformcomponent::SetQtonX(XMVECTOR qton)
{
	this->qtonX = qton;
}

void Transformcomponent::SetQtonY(XMVECTOR qton)
{
	this->qtonY = qton;
}

void Transformcomponent::SetQtonZ(XMVECTOR qton)
{
	this->qtonZ = qton;
}

void Transformcomponent::SetMtxRot(XMMATRIX mtx)
{
	this->mtxrot = mtx;
}

void Transformcomponent::SetMtxRotX(XMMATRIX mtx)
{
	this->mtxrotx = mtx;
}

void Transformcomponent::SetMtxRotY(XMMATRIX mtx)
{
	this->mtxroty = mtx;
}

void Transformcomponent::SetMtxRotZ(XMMATRIX mtx)
{
	this->mtxrotz = mtx;
}

void Transformcomponent::SetWorldMtx(XMMATRIX mtx)
{
	this->mtxWorld = mtx;
}



