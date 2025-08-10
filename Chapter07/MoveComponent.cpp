#include "MoveComponent.h"

#include "Actor.h"

MoveComponent::MoveComponent(Actor* const pOwner)
	: Component(pOwner)
	, mAngularSpeed(0.f)
	, mForwardSpeed(0.f)
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

	if (!Math::IsNearZero(mForwardSpeed))
	{
		Vector3 pos = mpOwner->GetPosition();
		pos += mpOwner->GetForward() * mForwardSpeed * deltaTime;

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
