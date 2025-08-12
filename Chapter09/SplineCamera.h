#pragma once

#include "CameraComponent.h"
#include <vector>

class Spline
{
public:
	std::vector<Vector3> ControlPoints;


public:
	Vector3 Compute(const size_t startIdx, const float t) const;

	size_t GetNumPoints() const
	{
		return ControlPoints.size();
	}
};

class SplineCamera final : public CameraComponent
{
public:
	SplineCamera(Actor* const pOwner);
	virtual ~SplineCamera() = default;

	void Update(const float deltaTime) override;
	void Restart();

	void SetSpeed(const float speed)
	{
		mSpeed = speed;
	}

	void SetSpline(const Spline& spline)
	{
		mPath = spline;
	}

	void SetPaused(const bool bPause)
	{
		mbPaused = bPause;
	}

private:
	Spline mPath;
	size_t mIndex;

	float mT;
	float mSpeed;

	bool mbPaused;
};