#include "MoveComponent.h"

#include "Actor.h"
#include "Game.h"

MoveComponent::MoveComponent(Actor* const pOwner, const int updateOrder)
	: Component(pOwner, updateOrder)
	, mAngularSpeed(0.f)
	, mForwardSpeed(0.f)
{
}

MoveComponent::MoveComponent(Actor* const pOwner)
	: MoveComponent(pOwner, 10)
{
}

void MoveComponent::Update(const float deltaTime)
{
	if (!Math::IsNearZero(mAngularSpeed))
	{
		const Quaternion rot = mpOwner->GetRotation();
		const float angle = mAngularSpeed * deltaTime;

		const Quaternion inc(Vector3::UnitZ, angle);

		const Quaternion nextRot = Quaternion::Concatenate(rot, inc);

		mpOwner->SetRotation(nextRot);
	}

	if (!Math::IsNearZero(mForwardSpeed))
	{
		Vector3 pos = mpOwner->GetPosition();
		pos += mpOwner->GetForward() * mForwardSpeed * deltaTime;

		mpOwner->SetPosition(pos);
	}
}
