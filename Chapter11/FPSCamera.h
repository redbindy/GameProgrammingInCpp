#pragma once

#include "CameraComponent.h"
#include "Math.h"

class Actor;

class FPSCamera final : public CameraComponent
{
public:
	FPSCamera(Actor* const pOwner);
	virtual ~FPSCamera() = default;

	void Update(const float deltaTime) override;

	float GetPitch() const
	{
		return mPitch;
	}

	float GetPitchSpeed() const
	{
		return mPitchSpeed;
	}

	void SetPitchSpeed(const float speed)
	{
		mPitchSpeed = speed;
	}

	float GetMaxPitch() const
	{
		return mMaxPitch;
	}

	void SetMaxPitch(const float pitch)
	{
		mMaxPitch = pitch;
	}

private:
	float mPitchSpeed;
	float mMaxPitch;
	float mPitch;
};