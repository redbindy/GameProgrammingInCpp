#pragma once

#include <string>
#include <vector>
#include <rapidjson/document.h>
#include <SDL3/SDL_log.h>
#include <fstream>
#include <sstream>

#include "BoneTransform.h"
#include "MatrixPalette.h"

class Skeleton final
{
public:
	struct Bone
	{
		BoneTransform localBindPose;
		std::string name;
		int parent;
	};

	bool Load(const std::string& fileName);

	size_t GetNumBones() const
	{
		return mBones.size();
	}

	const Bone& GetBones(const size_t idx) const
	{
		return mBones[idx];
	}

	const std::vector<Bone>& GetBones() const
	{
		return mBones;
	}

	const std::vector<Matrix4>& GetGlobalInvBindPoses() const
	{
		return mGlobalInvBindPoses;
	}

protected:
	void computeGlobalInvBindPoses();

private:
	std::vector<Bone> mBones;

	std::vector<Matrix4> mGlobalInvBindPoses;
};