#include "SplineCamera.h"

Vector3 Spline::Compute(const size_t startIdx, const float t) const
{
	if (startIdx >= ControlPoints.size())
	{
		return ControlPoints.back();
	}

	if (startIdx == 0)
	{
		return ControlPoints[startIdx];
	}

	if (startIdx + 2 >= ControlPoints.size())
	{
		return ControlPoints[startIdx];
	}

	const Vector3 p0 = ControlPoints[startIdx - 1];
	const Vector3 p1 = ControlPoints[startIdx];
	const Vector3 p2 = ControlPoints[startIdx + 1];
	const Vector3 p3 = ControlPoints[startIdx + 2];

	const Vector3 position = 0.5f * (
		(2.f * p1) + (-1.f * p0 + p2) * t +
		(2.f * p0 - 5.f * p1 + 4.f * p2 - p3) * t * t +
		(-1.f * p0 + 3.f * p1 - 3.f * p2 + p3) * t * t * t
		);

	return position;
}

SplineCamera::SplineCamera(Actor* const pOwner)
	: CameraComponent(pOwner)
	, mPath()
	, mIndex(1)
	, mT(0.f)
	, mSpeed(0.5f)
	, mbPaused(true)
{

}

void SplineCamera::Update(const float deltaTime)
{
	CameraComponent::Update(deltaTime);

	if (!mbPaused)
	{
		mT += mSpeed * deltaTime;

		if (mT >= 1.f)
		{
			if (mIndex < mPath.GetNumPoints() - 3)
			{
				++mIndex;
				mT = mT - 1.f;
			}
			else
			{
				mbPaused = true;
			}
		}
	}

	const Vector3 cameraPos = mPath.Compute(mIndex, mT);
	const Vector3 target = mPath.Compute(mIndex, mT + 0.01f);
	const Vector3 up = Vector3::UnitZ;

	const Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);

	SetViewMatrix(view);
}

void SplineCamera::Restart()
{
	mIndex = 1;
	mT = 0.f;
	mbPaused = false;
}