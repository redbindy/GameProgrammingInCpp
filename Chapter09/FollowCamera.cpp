#include "FollowCamera.h"

#include "Actor.h"

FollowCamera::FollowCamera(Actor* const pOwner)
	: CameraComponent(pOwner)
	, mHorzDist(350.f)
	, mVertDist(150.f)
	, mTargetDist(100.f)
	, mActualPos(Vector3::Zero)
	, mVelocity(Vector3::Zero)
	, mSpringConstant(64.f)
{

}

void FollowCamera::Update(const float deltaTime)
{
	CameraComponent::Update(deltaTime);

	const float dampening = 2.f * Math::Sqrt(mSpringConstant);

	const Vector3 idealPos = computeCameraPos();
	const Vector3 diff = mActualPos - idealPos;

	const Vector3 accel = -mSpringConstant * diff - dampening * mVelocity;

	mVelocity += accel * deltaTime;
	mActualPos += mVelocity * deltaTime;

	const Vector3 target = mpOwner->GetPosition() + mpOwner->GetForward() * mTargetDist;

	const Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);
	SetViewMatrix(view);
}

void FollowCamera::SnapToIdeal()
{
	mActualPos = computeCameraPos();

	mVelocity = Vector3::Zero;

	const Vector3 target = mpOwner->GetPosition() + mpOwner->GetForward() * mTargetDist;

	const Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);

	SetViewMatrix(view);
}

Vector3 FollowCamera::computeCameraPos() const
{
	Vector3 cameraPos = mpOwner->GetPosition();
	cameraPos -= mpOwner->GetForward() * mHorzDist;
	cameraPos += Vector3::UnitZ * mVertDist;

	return cameraPos;
}
