#include "FollowCamera.h"

#include "Actor.h"
#include "LevelLoader.h"

FollowCamera::FollowCamera(Actor* const pOwner)
	: CameraComponent(pOwner)
	, mHorzDist(350.f)
	, mVertDist(250.f)
	, mTargetDist(100.f)
	, mActualPos(Vector3::Zero)
	, mVelocity(Vector3::Zero)
	, mSpringConstant(128.f)
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

void FollowCamera::LoadProperties(const rapidjson::Value& inObj)
{
	CameraComponent::LoadProperties(inObj);

	JsonHelper::GetVector3(inObj, "actualPos", mActualPos);
	JsonHelper::GetVector3(inObj, "velocity", mVelocity);
	JsonHelper::GetFloat(inObj, "horzDist", mHorzDist);
	JsonHelper::GetFloat(inObj, "vertDist", mVertDist);
	JsonHelper::GetFloat(inObj, "targetDist", mTargetDist);
	JsonHelper::GetFloat(inObj, "springConstant", mSpringConstant);
}

void FollowCamera::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	CameraComponent::SaveProperties(alloc, inObj);

	JsonHelper::AddVector3(alloc, inObj, "actualPos", mActualPos);
	JsonHelper::AddVector3(alloc, inObj, "velocity", mVelocity);
	JsonHelper::AddFloat(alloc, inObj, "horzDist", mHorzDist);
	JsonHelper::AddFloat(alloc, inObj, "vertDist", mVertDist);
	JsonHelper::AddFloat(alloc, inObj, "targetDist", mTargetDist);
	JsonHelper::AddFloat(alloc, inObj, "springConstant", mSpringConstant);
}

Vector3 FollowCamera::computeCameraPos() const
{
	Vector3 cameraPos = mpOwner->GetPosition();
	cameraPos -= mpOwner->GetForward() * mHorzDist;
	cameraPos += Vector3::UnitZ * mVertDist;

	return cameraPos;
}
