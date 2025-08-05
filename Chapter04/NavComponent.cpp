#include "NavComponent.h"

#include "Actor.h"
#include "Tile.h"

NavComponent::NavComponent(Actor* const pOwner, const int updateOrder)
	: MoveComponent(pOwner, updateOrder)
	, mNextNode(nullptr)
{
}

NavComponent::NavComponent(Actor* const pOwner)
	: NavComponent(pOwner, 10)
{
}

void NavComponent::Update(const float deltaTime)
{
	if (mNextNode != nullptr)
	{
		const Vector2 diff = mpOwner->GetPosition() - mNextNode->GetPosition();
		if (Math::IsNearZero(diff.Length(), 2.f))
		{
			mNextNode = mNextNode->GetParent();
			TurnTo(mNextNode->GetPosition());
		}
	}

	MoveComponent::Update(deltaTime);
}

void NavComponent::StartPath(const Tile* const pStart)
{
	mNextNode = pStart->GetParent();
	TurnTo(mNextNode->GetPosition());
}

void NavComponent::TurnTo(const Vector2& pos)
{
	Vector2 dir = pos - mpOwner->GetPosition();

	const float angle = Math::Atan2(-dir.Y, dir.X);

	mpOwner->SetRotation(angle);
}
