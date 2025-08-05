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
		float rot = mpOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;

		mpOwner->SetRotation(rot);
	}

	if (!Math::IsNearZero(mForwardSpeed))
	{
		Vector2 pos = mpOwner->GetPosition();
		pos += mpOwner->GetForward() * mForwardSpeed * deltaTime;

		mpOwner->SetPosition(pos);
	}
}
