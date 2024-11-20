#pragma once
#include "component.h"
#include "Coreminimal.h"

#define CLOUD_MPOINT (32)

class CloudComponent :public Component
{
public:
	CloudComponent(GameObject* gameObject);
	~CloudComponent();
	// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	void SetCloudHightTop(int xl, int xr, int zd, int zu, int n, float tl, float tr, float bl, float br);
	void SetCloudHightBottom(int xl, int xr, int zd, int zu, int n, float tl, float tr, float bl, float br);

private:
	float hightTop[CLOUD_MPOINT + 1][CLOUD_MPOINT + 1];
	float hightBottom[CLOUD_MPOINT + 1][CLOUD_MPOINT + 1];

	vector<XMFLOAT3> partPosArray;
	 
	int* partindexArray;
};

float GetRandomHightPM(int n);
