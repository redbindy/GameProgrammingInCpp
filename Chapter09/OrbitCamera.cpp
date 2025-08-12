#include "OrbitCamera.h"

#include "Actor.h"

OrbitCamera::OrbitCamera(Actor* const pOwner)
	: CameraComponent(pOwner)
	, mOffset(-400.f, 0.f, 0.f)
	, mUp(Vector3::UnitZ)
	, mPitchSpeed(0.f)
	, mYawSpeed(0.f)
{
}

void OrbitCamera::Update(const float deltaTime)
{
	CameraComponent::Update(deltaTime);

	const Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);

	mOffset = Vector3::Transform(mOffset, yaw);
	mUp = Vector3::Transform(mUp, yaw);

	Vector3 forward = -1.f * mOffset;
	forward.Normalize();

	Vector3 right = Vector3::Cross(mUp, forward);
	right.Normalize();

	const Quaternion pitch(right, mPitchSpeed * deltaTime);

	mOffset = Vector3::Transform(mOffset, pitch);
	mUp = Vector3::Transform(mUp, pitch);

	const Vector3 target = mpOwner->GetPosition();
	const Vector3 cameraPos = target + mOffset;

	const Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, mUp);

	SetViewMatrix(view);
}
