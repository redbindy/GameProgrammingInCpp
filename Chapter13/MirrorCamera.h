#pragma once

#include "CameraComponent.h"

class MirrorCamera : public CameraComponent
{
public:
	MirrorCamera(Actor* const pOwner);
	virtual ~MirrorCamera() = default;

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

private:
	float mHorzDist;
	float mVertDist;
	float mTargetDist;

private:
	Vector3 computeCameraPos() const;
};