#pragma once

#include "CameraComponent.h"

#include "Math.h"

class OrbitCamera final : public CameraComponent
{
public:
	OrbitCamera(Actor* const pOwner);
	virtual ~OrbitCamera() = default;

	void Update(const float deltaTime) override;

	float GetPitchSpeed() const
	{
		return mPitchSpeed;
	}

	void SetPicthSpeed(const float speed)
	{
		mPitchSpeed = speed;
	}

	float GetYawSpeed() const
	{
		return mYawSpeed;
	}

	void SetYawSpeed(const float speed)
	{
		mYawSpeed = speed;
	}

private:
	Vector3 mOffset;
	Vector3 mUp;

	float mPitchSpeed;
	float mYawSpeed;
};