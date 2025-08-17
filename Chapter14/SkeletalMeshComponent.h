#pragma once

#include <vector>

#include "Math.h"
#include "MeshComponent.h"
#include "MatrixPalette.h"

class Actor;
class Skeleton;
class Animation;

class SkeletalMeshComponent final : public MeshComponent
{
public:
	SkeletalMeshComponent(Actor* const pActor);
	virtual ~SkeletalMeshComponent() = default;

	void Draw(Shader* const pShader) override;

	void Update(const float deltaTime) override;

	void SetSkeleton(const Skeleton* const pSK)
	{
		mpSkeleton = pSK;
	}

	float PlayAnimation(const Animation* pAnim, const float playRate = 1.f);

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;

	TypeID GetType() const override
	{
		return TSkeletalMeshComponent;
	}

protected:
	const Skeleton* mpSkeleton;
	const Animation* mpAnimation;

	float mAnimPlayRate;
	float mAnimTime;

	MatrixPalette mPalette;

protected:
	void computeMatrixPalette();
};