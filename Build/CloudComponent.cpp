#include "CloudComponent.h"
#include "level.h"
#include "SquareParticle.h"

#define CLOUD_HIGHT			(35)
#define CLOUD_PART_DIST		(200.0f)
#define	CLOUD_PART_SIZE		(200.0f)		// 頂点サイズ
#define CLOUD_ALPHA			(1.0f)
#define MPOINT_SPLIT		(5)


CloudComponent::CloudComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}

CloudComponent::~CloudComponent()
{
}

void CloudComponent::Init(void)
{

	int x1 = 0;
	int x2 = CLOUD_MPOINT;
	int z1 = 0;
	int z2 = CLOUD_MPOINT;

	pGameObject->GetTransFormComponent()->SetPosition(XMFLOAT3(0.0f, 0.0f, 50.0f));

	SetCloudHightTop(x1, x2, z1, z2, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	for (int x = 0; x < CLOUD_MPOINT+1; x++)
	{
		for (int z = 0; z < CLOUD_MPOINT + 1; z++)
		{
			hightTop[x][z] -= 5.0f;
			int yleng = (int)(hightTop[x][z]);

			for (int y = 0; y < yleng; y++)
			{

				XMFLOAT3 ppos;
				XMFLOAT3 pos = pGameObject->GetTransFormComponent()->GetPosition();
				ppos.x = x * CLOUD_PART_DIST + pos.x + (float)(rand() % 10 - 5) * 0.1f * CLOUD_PART_DIST;
				ppos.z = z * CLOUD_PART_DIST + pos.z + (float)(rand() % 10 - 5) * 0.1f * CLOUD_PART_DIST;
				ppos.y = pos.y + (hightTop[x][z] - hightTop[x][z] / 2) + ((hightTop[x][z] * CLOUD_PART_DIST) / yleng) * y - hightTop[x][z] / 2 + (float)(rand() % 10 - 5) * 0.1f * CLOUD_PART_DIST - yleng * CLOUD_HIGHT;
			
				partPosArray.push_back(ppos);
			
			}
		}

	}


	//for (int i = 0; i < partPosArray.size(); i++)
	//{
	//	pGameObject->GetLevel()->GetSquareParticleAdd()->AddParticle(partPosArray[i], CLOUD_PART_SIZE, CLOUD_ALPHA);
	//}

	pGameObject->GetLevel()->GetSquareParticleAdd()->AddParticle(pGameObject->GetTransFormComponent()->GetPosition(), CLOUD_PART_SIZE, CLOUD_ALPHA);

}

void CloudComponent::Uninit(void)
{
}

void CloudComponent::Update(void)
{
}

void CloudComponent::SetCloudHightTop(int xl, int xr, int zd, int zu, int n, float tl, float tr, float bl, float br)
{
	if (n == MPOINT_SPLIT)
	{
		return;
	}
	//平均
	float top = (tl + tr) / 2;
	float bottom = (bl + br) / 2;
	float left = (tl + bl) / 2;
	float right = (tr + br) / 2;
	float midpoint = (tl + tr + bl + br) / 4 + GetRandomHightPM(n);

	//float top = (tl + tr) / 2 + GetRandomHightPM(n+1);
	//float bottom = (bl + br) / 2 + GetRandomHightPM(n+1);
	//float left = (tl+ bl) / 2 + GetRandomHightPM(n+1);
	//float right = (tr + br) / 2 + GetRandomHightPM(n+1);
	//float midpoint = (tl + tr + bl + br) / 4+GetRandomHightPM(n);

	hightTop[(xl + xr) / 2][zu] = top;
	hightTop[(xl + xr) / 2][zd] = bottom;
	hightTop[xl][(zu + zd) / 2] = left;
	hightTop[xr][(zu + zd) / 2] = right;
	hightTop[(xl + xr) / 2][(zu + zd) / 2] = midpoint;


	SetCloudHightTop(xl, (xl + xr) / 2, (zu + zd) / 2, zu, n + 1, tl, top, left, midpoint);
	SetCloudHightTop((xl + xr) / 2, xr, (zu + zd) / 2, zu, n + 1, top, tr, midpoint, right);
	SetCloudHightTop(xl, (xl + xr) / 2, zd, (zu + zd) / 2, n + 1, left, midpoint, bl, bottom);
	SetCloudHightTop((xl + xr) / 2, xr, zd, (zu + zd) / 2, n + 1, midpoint, right, bottom, br);

}

void CloudComponent::SetCloudHightBottom(int xl, int xr, int zd, int zu, int n, float tl, float tr, float bl, float br)
{
}

float GetRandomHightPM(int n)
{
	float h = 0.0f;
	float f = powf(2, n);
	h = (rand() % (int)(CLOUD_HIGHT / powf(2, n))) * 0.01f;

	return h;
}
