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
	this->forward = { 0.0f,0.0f,-1.0f };
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
	this->forward = { 0.0f,0.0f,1.0f };
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
	this->pos = { 0.0f,0.0f,0.0f };
	this->oldPos = { 0.0f, 0.0f, 0.0f };
	this->rot = { 0.0f,0.0f,0.0f };
	this->oldRot = { 0.0f,0.0f,0.0f };
	this->scl = { 1.0f,1.0f,1.0f };
	this->forward = { 0.0f,0.0f,1.0f };
	this->fDirection= { 0.0f,0.0f,1.0f };
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

	XMVECTOR v = XMLoadFloat3(&fDirection);

	v = XMVector3Transform(v, mtxrot);

	v = XMVector3Normalize(v);

	XMStoreFloat3(&this->forward, v);

	
	

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
	return this->forward;
}

XMVECTOR TransformComponent::GetAxisX(void)
{
	return this->axisX;
}

XMVECTOR TransformComponent::GetAxisY(void)
{
	return this->axisY;
}

XMVECTOR TransformComponent::GetAxisZ(void)
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

void TransformComponent::SetForward(XMFLOAT3 forward)
{


}

void TransformComponent::RotForward(XMFLOAT3 forward)
{
	XMVECTOR f = XMVector3Normalize(XMLoadFloat3(&forward));
	XMVECTOR d = XMVector3Normalize(XMLoadFloat3(&this->forward));

	float dot;
	XMStoreFloat(&dot, XMVector3Dot(f, d));

	if (dot == -1.0f)
	{

		XMVECTOR qton = XMQuaternionRotationAxis(axisX, XM_PI);
		this->RotAxis(qton);
		mtxrot = XMMatrixRotationQuaternion(qton);
		return;
	}
	if (XMVector3Equal(f, d))
	{
		return;
	}

	XMVECTOR c = XMVector3Cross(f, d);

	XMVECTOR a = XMVector3AngleBetweenNormals(f, d);
	float angle;
	XMStoreFloat(&angle, a);



	XMVECTOR q = XMQuaternionRotationAxis(c, angle);
	XMMATRIX mtx = XMMatrixRotationQuaternion(q);

	this->RotAxis(q);

	this->mtxrot = mtx;

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

void TransformComponent::MoveX(float f)
{
	this->pos.x += f;
	this->PosUpdate();

}

void TransformComponent::MoveY(float f)
{
	this->pos.y += f;
	this->PosUpdate();

}

void TransformComponent::MoveZ(float f)
{
	this->pos.z += f;
	this->PosUpdate();

}


void TransformComponent::MoveForward(float f)
{
	float x, y, z;
	x = this->forward.x * f;
	y = this->forward.y * f;
	z = this->forward.z * f;

	MoveX(x);
	MoveY(y);
	MoveZ(z);


}

void TransformComponent::PosUpdate(void)
{
	this->mtxpos = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
}

void TransformComponent::SetForwardDiection(XMFLOAT3 dir)
{
	this->fDirection = XMFLOAT3Normalize(dir);
	this->forward = XMFLOAT3Normalize(dir);
}

void TransformComponent::RotRoll(float f)
{
	XMVECTOR qton = XMQuaternionRotationAxis(this->axisZ, f);
	this->mtxrot = XMMatrixMultiply(this->mtxrot, XMMatrixRotationQuaternion(qton));
	this->axisY = XMVector3Rotate(this->axisY, qton);
	this->axisX = XMVector3Rotate(this->axisX, qton);
	

}

void TransformComponent::RotPitch(float f)
{
	XMVECTOR qton = XMQuaternionRotationAxis(this->axisX, f);
	this->mtxrot = XMMatrixMultiply(this->mtxrot, XMMatrixRotationQuaternion(qton));
	this->axisY = XMVector3Rotate(this->axisY, qton);
	this->axisZ = XMVector3Rotate(this->axisZ, qton);

}

void TransformComponent::RotYaw(float f)
{
	XMVECTOR qton = XMQuaternionRotationAxis(this->axisY, f);
	this->mtxrot = XMMatrixMultiply(this->mtxrot, XMMatrixRotationQuaternion(qton));
	this->axisX = XMVector3Rotate(this->axisX, qton);
	this->axisZ = XMVector3Rotate(this->axisZ, qton);

}

void TransformComponent::RotAxis(XMVECTOR qton)
{
	this->axisX = XMVector3Rotate(this->axisX, qton);
	this->axisY = XMVector3Rotate(this->axisY, qton);
	this->axisZ = XMVector3Rotate(this->axisZ, qton);

}

void TransformComponent::RotAxisAngle(XMVECTOR axis, float angle)
{
	XMVECTOR qton = XMQuaternionRotationAxis(axis, angle);
	RotAxis(qton);

	mtxrot = XMMatrixMultiply(mtxrot, XMMatrixRotationQuaternion(qton));
}




