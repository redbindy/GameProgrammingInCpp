#include "Skeleton.h"

bool Skeleton::Load(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Skeleton %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Skeleton %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// Check the metadata
	if (ver != 1)
	{
		SDL_Log("Skeleton %s unknown format", fileName.c_str());
		return false;
	}

	const rapidjson::Value& bonecount = doc["bonecount"];
	if (!bonecount.IsUint())
	{
		SDL_Log("Skeleton %s doesn't have a bone count.", fileName.c_str());
		return false;
	}

	size_t count = bonecount.GetUint();

	if (count > MAX_SKELETON_BONES)
	{
		SDL_Log("Skeleton %s exceeds maximum bone count.", fileName.c_str());
		return false;
	}

	mBones.reserve(count);

	const rapidjson::Value& bones = doc["bones"];
	if (!bones.IsArray())
	{
		SDL_Log("Skeleton %s doesn't have a bone array?", fileName.c_str());
		return false;
	}

	if (bones.Size() != count)
	{
		SDL_Log("Skeleton %s has a mismatch between the bone count and number of bones", fileName.c_str());
		return false;
	}

	Bone temp;

	for (rapidjson::SizeType i = 0; i < count; i++)
	{
		if (!bones[i].IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		const rapidjson::Value& name = bones[i]["name"];
		temp.name = name.GetString();

		const rapidjson::Value& parent = bones[i]["parent"];
		temp.parent = parent.GetInt();

		const rapidjson::Value& bindpose = bones[i]["bindpose"];
		if (!bindpose.IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		const rapidjson::Value& rot = bindpose["rot"];
		const rapidjson::Value& trans = bindpose["trans"];

		if (!rot.IsArray() || !trans.IsArray())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		temp.localBindPose.rotation.X = rot[0].GetDouble();
		temp.localBindPose.rotation.Y = rot[1].GetDouble();
		temp.localBindPose.rotation.Z = rot[2].GetDouble();
		temp.localBindPose.rotation.W = rot[3].GetDouble();

		temp.localBindPose.translation.X = trans[0].GetDouble();
		temp.localBindPose.translation.Y = trans[1].GetDouble();
		temp.localBindPose.translation.Z = trans[2].GetDouble();

		mBones.emplace_back(temp);
	}

	computeGlobalInvBindPoses();

	return true;
}

void Skeleton::computeGlobalInvBindPoses()
{
	mGlobalInvBindPoses.resize(GetNumBones());

	mGlobalInvBindPoses[0] = mBones[0].localBindPose.ToMatrix();
	for (size_t i = 1; i < mGlobalInvBindPoses.size(); ++i)
	{
		const Matrix4 localMat = mBones[i].localBindPose.ToMatrix();

		mGlobalInvBindPoses[i] = localMat * mGlobalInvBindPoses[mBones[i].parent];
	}

	for (size_t i = 0; i < mGlobalInvBindPoses.size(); ++i)
	{
		mGlobalInvBindPoses[i].Invert();
	}
}
