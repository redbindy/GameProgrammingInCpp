#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL3/SDL_log.h>

#include "BoneTransform.h"

class Skeleton;

class Animation final
{
public:
	bool Load(const std::string& fileName);

	size_t GetNumBones() const
	{
		return mNumBones;
	}

	size_t GetNumFrames() const
	{
		return mNumFrames;
	}

	float GetDuration() const
	{
		return mDuration;
	}

	float GetFrameDuration() const
	{
		return mFrameDuration;
	}

	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* pInSkeleton, const float inTime) const;

private:
	size_t mNumBones;
	size_t mNumFrames;

	float mDuration;
	float mFrameDuration;

	std::vector<std::vector<BoneTransform>> mTracks;
};