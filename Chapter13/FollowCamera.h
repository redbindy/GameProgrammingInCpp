#pragma once

#include "CameraComponent.h"

class FollowCamera final : public CameraComponent
{
public:
	FollowCamera(Actor* const pOwner);
	virtual ~FollowCamera() = default;

	void Update(const float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(const float dist)
	{
		mHorzDist = dist;
	}

	void SetVertDist(const float dist)
	{
		mVertDist = dist;
	}

	void SetTargetDist(const float dist)
	{
		mTargetDist = dist;
	}

	void SetSpringConstant(const float spring)
	{
		mSpringConstant = spring;
	}

private:
	float mHorzDist;
	float mVertDist;
	float mTargetDist;

	Vector3 mActualPos;
	Vector3 mVelocity;
	float mSpringConstant;

private:
	Vector3 computeCameraPos() const;
};