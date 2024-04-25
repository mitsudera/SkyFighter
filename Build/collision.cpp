//=============================================================================
//
// �����蔻�菈�� [collision.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


//=============================================================================
// BB�ɂ�铖���蔻�菈��
// ��]�͍l�����Ȃ�
// �߂�l�F�������Ă���true
//=============================================================================
BOOL CollisionBB(XMFLOAT3 mpos, float mw, float mh,
	XMFLOAT3 ypos, float yw, float yh)
{
	BOOL ans = FALSE;	// �O����Z�b�g���Ă���

	// ���W�����S�_�Ȃ̂Ōv�Z���₷�������ɂ��Ă���
	mw /= 2;
	mh /= 2;
	yw /= 2;
	yh /= 2;

	// �o�E���f�B���O�{�b�N�X(BB)�̏���
	if ((mpos.x + mw > ypos.x - yw) &&
		(mpos.x - mw < ypos.x + yw) &&
		(mpos.y + mh > ypos.y - yh) &&
		(mpos.y - mh < ypos.y + yh))
	{
		// �����������̏���
		ans = TRUE;
	}

	return ans;
}

//=============================================================================
// BC�ɂ�铖���蔻�菈��
// �T�C�Y�͔��a
// �߂�l�F�������Ă���TRUE
//=============================================================================
BOOL CollisionBC(XMFLOAT3 pos1, XMFLOAT3 pos2, float r1, float r2)
{
	BOOL ans = FALSE;						// �O����Z�b�g���Ă���

	float len = (r1 + r2) * (r1 + r2);		// ���a��2�悵����
	XMVECTOR temp = XMLoadFloat3(&pos1) - XMLoadFloat3(&pos2);
	temp = XMVector3LengthSq(temp);			// 2�_�Ԃ̋����i2�悵�����j
	float lenSq = 0.0f;
	XMStoreFloat(&lenSq, temp);

	// ���a��2�悵������苗�����Z���H
	if (len > lenSq)
	{
		ans = TRUE;	// �������Ă���
	}

	return ans;
}
















//�Ђ��`�ƈ�_�̓����蔻��
//�����͂Ђ��`�̒��S�_�ƕ��ƍ����Ɠ_�̍��W
BOOL CollisionDiaDot(XMFLOAT3 mpos, float mw, float mh,XMFLOAT3 ypos)
{
	BOOL ans = FALSE;	// �O����Z�b�g���Ă���
		// ���W�����S�_�Ȃ̂Ōv�Z���₷�������ɂ��Ă���
	mw /= 2;
	mh /= 2;
	


	



	XMFLOAT3 A;
	XMFLOAT3 B;
	XMFLOAT3 C;
	XMFLOAT3 D;
	
	XMFLOAT3 AB;
	XMFLOAT3 BC;
	XMFLOAT3 CD;
	XMFLOAT3 DA;



	
	A.x = (mpos.x);
	A.y = (mpos.y - mh);
	
	B.x = (mpos.x-mw);
	B.y = (mpos.y);
	
	C.x = (mpos.x);
	C.y = (mpos.y + mh);
	
	D.x = (mpos.x+mw);
	D.y = (mpos.y);
	

	AB.x = B.x - A.x;
	AB.y = B.y - A.y;
	BC.x = C.x - B.x;
	BC.y = C.y - B.y;
	CD.x = D.x - C.x;
	CD.y = D.y - C.y;
	DA.x = A.x - D.x;
	DA.y = A.y - D.y;

	XMFLOAT3 AO;
	XMFLOAT3 BO;
	XMFLOAT3 CO;
	XMFLOAT3 DO;

	AO.x = ypos.x - A.x;
	AO.y = ypos.y - A.y;
	BO.x = ypos.x - B.x;
	BO.y = ypos.y - B.y;
	CO.x = ypos.x - C.x;
	CO.y = ypos.y - C.y;
	DO.x = ypos.x - D.x;
	DO.y = ypos.y - D.y;

	//z�����������߂�
	float Az, Bz, Cz, Dz;
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;




	//�S�ăv���X���S�ă}�C�i�X�Ȃ璆�ɂ���
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}

	return ans;
}


//�l�p�`(�S�Ă̒��_�̍��W��180�x�ȓ�)�Ɠ_�̓����蔻��
//�����͎l�p�`�̎l�̒��_�Ɠ_�̍��W
BOOL CollisionFreeBox(XMFLOAT3 A,XMFLOAT3 B,XMFLOAT3 C,XMFLOAT3 D, XMFLOAT3 ypos)
{
	BOOL ans = FALSE;	// �O����Z�b�g���Ă���
	


	



	
	XMFLOAT3 AB;
	XMFLOAT3 BC;
	XMFLOAT3 CD;
	XMFLOAT3 DA;



	
	

	AB.x = B.x - A.x;
	AB.y = B.y - A.y;
	BC.x = C.x - B.x;
	BC.y = C.y - B.y;
	CD.x = D.x - C.x;
	CD.y = D.y - C.y;
	DA.x = A.x - D.x;
	DA.y = A.y - D.y;

	XMFLOAT3 AO;
	XMFLOAT3 BO;
	XMFLOAT3 CO;
	XMFLOAT3 DO;

	AO.x = ypos.x - A.x;
	AO.y = ypos.y - A.y;
	BO.x = ypos.x - B.x;
	BO.y = ypos.y - B.y;
	CO.x = ypos.x - C.x;
	CO.y = ypos.y - C.y;
	DO.x = ypos.x - D.x;
	DO.y = ypos.y - D.y;

	//z�����������߂�
	float Az, Bz, Cz, Dz;
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;




	//�S�ăv���X���S�ă}�C�i�X�Ȃ璆�ɂ���
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}

	return ans;
}
//�l�p�`(�S�Ă̒��_�̍��W��180�x�ȓ�)���m�̓����蔻��
//�����͎l�p�`1��2�̎l�̒��_���W
BOOL CollisionFreeBox(XMFLOAT3 A1,XMFLOAT3 B1,XMFLOAT3 C1,XMFLOAT3 D1,XMFLOAT3 A2,XMFLOAT3 B2,XMFLOAT3 C2,XMFLOAT3 D2)
{
	BOOL ans = FALSE;	// �O����Z�b�g���Ă���
	
	//���_�̂ǂꂩ���������Ă���Γ�����

	XMFLOAT3 AB;
	XMFLOAT3 BC;
	XMFLOAT3 CD;
	XMFLOAT3 DA;
	float Az, Bz, Cz, Dz;

	XMFLOAT3 AO;
	XMFLOAT3 BO;
	XMFLOAT3 CO;
	XMFLOAT3 DO;

	AB.x = B1.x - A1.x;
	AB.y = B1.y - A1.y;
	BC.x = C1.x - B1.x;
	BC.y = C1.y - B1.y;
	CD.x = D1.x - C1.x;
	CD.y = D1.y - C1.y;
	DA.x = A1.x - D1.x;
	DA.y = A1.y - D1.y;


	//���_A

	AO.x = A2.x - A1.x;
	AO.y = A2.y - A1.y;
	BO.x = A2.x - B1.x;
	BO.y = A2.y - B1.y;
	CO.x = A2.x - C1.x;
	CO.y = A2.y - C1.y;
	DO.x = A2.x - D1.x;
	DO.y = A2.y - D1.y;

	//z�����������߂�
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;
	//�S�ăv���X���S�ă}�C�i�X�Ȃ璆�ɂ���
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}


	//���_B
	AB.x = B1.x - A1.x;
	AB.y = B1.y - A1.y;
	BC.x = C1.x - B1.x;
	BC.y = C1.y - B1.y;
	CD.x = D1.x - C1.x;
	CD.y = D1.y - C1.y;
	DA.x = A1.x - D1.x;
	DA.y = A1.y - D1.y;

	AO.x = B2.x - A1.x;
	AO.y = B2.y - A1.y;
	BO.x = B2.x - B1.x;
	BO.y = B2.y - B1.y;
	CO.x = B2.x - C1.x;
	CO.y = B2.y - C1.y;
	DO.x = B2.x - D1.x;
	DO.y = B2.y - D1.y;

	//z�����������߂�
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;

	//�S�ăv���X���S�ă}�C�i�X�Ȃ璆�ɂ���
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}
	//���_C
	AB.x = B1.x - A1.x;
	AB.y = B1.y - A1.y;
	BC.x = C1.x - B1.x;
	BC.y = C1.y - B1.y;
	CD.x = D1.x - C1.x;
	CD.y = D1.y - C1.y;
	DA.x = A1.x - D1.x;
	DA.y = A1.y - D1.y;


	AO.x = C2.x - A1.x;
	AO.y = C2.y - A1.y;
	BO.x = C2.x - B1.x;
	BO.y = C2.y - B1.y;
	CO.x = C2.x - C1.x;
	CO.y = C2.y - C1.y;
	DO.x = C2.x - D1.x;
	DO.y = C2.y - D1.y;

	//z�����������߂�
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;

	//�S�ăv���X���S�ă}�C�i�X�Ȃ璆�ɂ���
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}
	//���_D
	AB.x = B1.x - A1.x;
	AB.y = B1.y - A1.y;
	BC.x = C1.x - B1.x;
	BC.y = C1.y - B1.y;
	CD.x = D1.x - C1.x;
	CD.y = D1.y - C1.y;
	DA.x = A1.x - D1.x;
	DA.y = A1.y - D1.y;


	AO.x = D2.x - A1.x;
	AO.y = D2.y - A1.y;
	BO.x = D2.x - B1.x;
	BO.y = D2.y - B1.y;
	CO.x = D2.x - C1.x;
	CO.y = D2.y - C1.y;
	DO.x = D2.x - D1.x;
	DO.y = D2.y - D1.y;

	//z�����������߂�
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;

	//�S�ăv���X���S�ă}�C�i�X�Ȃ璆�ɂ���
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}

	return ans;
}

//=============================================================================
// ����(dot)
//=============================================================================
float dotProduct(XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	float ans = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
#else
	// �_�C���N�g�w�ł́A�A�A
	XMVECTOR temp = XMVector3Dot(*v1, *v2);
	float ans = 0.0f;
	XMStoreFloat(&ans, temp);
#endif

	return(ans);
}


//=============================================================================
// �O��(cross)
//=============================================================================
void crossProduct(XMVECTOR* ret, XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	ret->x = v1->y * v2->z - v1->z * v2->y;
	ret->y = v1->z * v2->x - v1->x * v2->z;
	ret->z = v1->x * v2->y - v1->y * v2->x;
#else
	// �_�C���N�g�w�ł́A�A�A
	* ret = XMVector3Cross(*v1, *v2);
#endif

}


//=============================================================================
// ���C�L���X�g
// p0, p1, p2�@�|���S���̂R���_
// pos0 �n�_
// pos1 �I�_
// hit�@��_�̕ԋp�p
// normal �@���x�N�g���̕ԋp�p
// �������Ă���ꍇ�ATRUE��Ԃ�
//=============================================================================
BOOL RayCast(XMFLOAT3 xp0, XMFLOAT3 xp1, XMFLOAT3 xp2, XMFLOAT3 xpos0, XMFLOAT3 xpos1, XMFLOAT3* hit, XMFLOAT3* normal)
{
	XMVECTOR	p0 = XMLoadFloat3(&xp0);
	XMVECTOR	p1 = XMLoadFloat3(&xp1);
	XMVECTOR	p2 = XMLoadFloat3(&xp2);
	XMVECTOR	pos0 = XMLoadFloat3(&xpos0);
	XMVECTOR	pos1 = XMLoadFloat3(&xpos1);

	XMVECTOR	nor;	// �|���S���̖@��
	XMVECTOR	vec1;
	XMVECTOR	vec2;
	float		d1, d2;

	{	// �|���S���̊O�ς��Ƃ��Ė@�������߂�(���̏����͌Œ蕨�Ȃ�\��Init()�ōs���Ă����Ɨǂ�)
		vec1 = p1 - p0;
		vec2 = p2 - p0;
		crossProduct(&nor, &vec2, &vec1);
		nor = XMVector3Normalize(nor);		// �v�Z���₷���悤�ɖ@�����m�[�}���C�Y���Ă���(���̃x�N�g���̒������P�ɂ��Ă���)
		XMStoreFloat3(normal, nor);			// ���߂��@�������Ă���
	}

	// �|���S�����ʂƐ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�i�s�p�Ȃ�{�A�݊p�Ȃ�[�A���p�Ȃ�O�j
	vec1 = pos0 - p0;
	vec2 = pos1 - p0;
	{	// ���߂��|���S���̖@���ƂQ�̃x�N�g���i�����̗��[�ƃ|���S����̔C�ӂ̓_�j�̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		d1 = dotProduct(&vec1, &nor);
		d2 = dotProduct(&vec2, &nor);
		if (((d1 * d2) > 0.0f) || (d1 == 0 && d2 == 0))
		{
			// �������Ă���\���͖���
			return(FALSE);
		}
	}


	{	// �|���S���Ɛ����̌�_�����߂�
		d1 = (float)fabs(d1);	// ��Βl�����߂Ă���
		d2 = (float)fabs(d2);	// ��Βl�����߂Ă���
		float a = d1 / (d1 + d2);							// ������

		XMVECTOR	vec3 = (1 - a) * vec1 + a * vec2;		// p0�����_�ւ̃x�N�g��
		XMVECTOR	p3 = p0 + vec3;							// ��_
		XMStoreFloat3(hit, p3);								// ���߂���_�����Ă���

		{	// ���߂���_���|���S���̒��ɂ��邩���ׂ�

			// �|���S���̊e�ӂ̃x�N�g��
			XMVECTOR	v1 = p1 - p0;
			XMVECTOR	v2 = p2 - p1;
			XMVECTOR	v3 = p0 - p2;

			// �e���_�ƌ�_�Ƃ̃x�N�g��
			XMVECTOR	v4 = p3 - p1;
			XMVECTOR	v5 = p3 - p2;
			XMVECTOR	v6 = p3 - p0;

			// �O�ςŊe�ӂ̖@�������߂āA�|���S���̖@���Ƃ̓��ς��Ƃ��ĕ������`�F�b�N����
			XMVECTOR	n1, n2, n3;

			crossProduct(&n1, &v4, &v1);
			if (dotProduct(&n1, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�

			crossProduct(&n2, &v5, &v2);
			if (dotProduct(&n2, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�

			crossProduct(&n3, &v6, &v3);
			if (dotProduct(&n3, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�
		}
	}

	return(TRUE);	// �������Ă���I(hit�ɂ͓������Ă����_�������Ă���Bnormal�ɂ͖@���������Ă���)
}

BOOL CollisionRaySphire(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 center ,float r)
{




	BOOL ans = FALSE;
	XMVECTOR p1 = XMLoadFloat3(&pos1);
	XMVECTOR p2 = XMLoadFloat3(&pos2);
	XMVECTOR cp = XMLoadFloat3(&center);

	XMVECTOR p1c = cp - p1;
	XMVECTOR p2c = cp - p2;
	float p1l;
	float p2l;
	XMStoreFloat(&p1l, XMVector3Length(p1c));
	XMStoreFloat(&p2l, XMVector3Length(p2c));

	if (p1l < r || p2l < r)
	{
		return TRUE;
	}

	XMVECTOR lv = XMVector3LinePointDistance(p1,p2,cp);
	float length;
	XMStoreFloat(&length, lv);
	if (length<r)
	{
		return TRUE;
	}

	return FALSE;

}




