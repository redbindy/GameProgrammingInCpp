#include "MoveComponent.h"

#include "Actor.h"
#include "LevelLoader.h"

MoveComponent::MoveComponent(Actor* const pOwner)
	: Component(pOwner)
	, mAngularSpeed(0.f)
	, mForwardSpeed(0.f)
	, mStrafeSpeed(0.f)
{
}

void MoveComponent::Update(const float deltaTime)
{
	if (!Math::IsNearZero(mAngularSpeed))
	{
		Quaternion rot = mpOwner->GetRotation();
		const float angle = mAngularSpeed * deltaTime;

		const Quaternion inc(Vector3::UnitZ, angle);

		rot = Quaternion::Concatenate(rot, inc);

		mpOwner->SetRotation(rot);
	}

	if (!Math::IsNearZero(mForwardSpeed) || !Math::IsNearZero(mStrafeSpeed))
	{
		Vector3 pos = mpOwner->GetPosition();
		pos += mpOwner->GetForward() * mForwardSpeed * deltaTime;

		pos += mpOwner->GetRight() * mStrafeSpeed * deltaTime;

		mpOwner->SetPosition(pos);
	}
}

float MoveComponent::GetAngularSpeed() const
{
	return mAngularSpeed;
}

void MoveComponent::SetAngularSpeed(const float speed)
{
	mAngularSpeed = speed;
}

float MoveComponent::GetForwardSpeed() const
{
	return mForwardSpeed;
}

void MoveComponent::SetForwardSpeed(const float speed)
{
	mForwardSpeed = speed;
}

float MoveComponent::GetStrafeSpeed() const
{
	return mStrafeSpeed;
}

void MoveComponent::SetStrafeSpeed(const float speed)
{
	mStrafeSpeed = speed;
}

void MoveComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);

	JsonHelper::GetFloat(inObj, "angularSpeed", mAngularSpeed);
	JsonHelper::GetFloat(inObj, "forwardSpeed", mForwardSpeed);
	JsonHelper::GetFloat(inObj, "strafeSpeed", mStrafeSpeed);
}

void MoveComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::SaveProperties(alloc, inObj);

	JsonHelper::AddFloat(alloc, inObj, "angularSpeed", mAngularSpeed);
	JsonHelper::AddFloat(alloc, inObj, "forwardSpeed", mForwardSpeed);
	JsonHelper::AddFloat(alloc, inObj, "strafeSpeed", mStrafeSpeed);
}


