#pragma once
#include "SkinMeshDataList.h"
class SkeletonAnimData
{

public:
	SkeletonAnimData();
	~SkeletonAnimData();
	void CreateSkeleton(int frameN);
	SKELETON* GetFrameSkeleton(int frame);
	void LoadSkeletonAnimData(string path);
	string GetFilePath(void);
	int GetFrameNum(void);

private:
	SKELETON* frameSkeleton;
	int frameNum;
	string filePath;
	
};

