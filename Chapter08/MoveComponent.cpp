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

		constexpr float WORLD_LEFT = SCREEN_WIDTH * -0.5f;
		constexpr float WORLD_RIGHT = SCREEN_WIDTH * 0.5f;
		constexpr float WORLD_BOTTOM = SCREEN_HEIGHT * -0.5f;
		constexpr float WORLD_TOP = SCREEN_HEIGHT * 0.5f;

		if (pos.X < WORLD_LEFT)
		{
			pos.X = WORLD_RIGHT - 2.f;
		}
		else if (pos.X > WORLD_RIGHT)
		{
			pos.X = WORLD_LEFT + 2.f;
		}

		if (pos.Y < WORLD_BOTTOM)
		{
			pos.Y = WORLD_TOP - 2.f;
		}
		else if (pos.Y > WORLD_TOP)
		{
			pos.Y = WORLD_BOTTOM + 2.f;
		}

		mpOwner->SetPosition(pos);
	}
}
