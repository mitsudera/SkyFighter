#include "CloudComponent.h"

#define CLOUD_HIGHT			(3500)


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


	SetCloudHightTop(x1, x2, z1, z2, 0, 0.0f, 0.0f, 0.0f, 0.0f);

}

void CloudComponent::Uninit(void)
{
}

void CloudComponent::Update(void)
{
}

void CloudComponent::SetCloudHightTop(int xl, int xr, int zd, int zu, int n, float tl, float tr, float bl, float br)
{
	if (n == CLOUD_MPOINT)
	{
		return;
	}
	//•½‹Ï
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
	h = (rand() % (int)(CLOUD_HIGHT / powf(2, n))) * 0.01f;

	return h;
}
