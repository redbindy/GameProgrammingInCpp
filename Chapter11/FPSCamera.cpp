#include "FPSCamera.h"

#include "Actor.h"

FPSCamera::FPSCamera(Actor* const pOwner)
	: CameraComponent(pOwner)
	, mPitchSpeed(0.f)
	, mMaxPitch(Math::PI / 3.f)
	, mPitch(0.f)
{
}

void FPSCamera::Update(const float deltaTime)
{
	CameraComponent::Update(deltaTime);

	mPitch += mPitchSpeed * deltaTime;

	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);

	const Quaternion q(mpOwner->GetRight(), mPitch);

	const Vector3 cameraPos = mpOwner->GetPosition();
	const Vector3 viewForward = Vector3::Transform(mpOwner->GetForward(), q);
	const Vector3 target = cameraPos + viewForward * 100.f;
	const Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	const Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);

	SetViewMatrix(view);
}
