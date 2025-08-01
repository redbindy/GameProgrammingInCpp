#include "MoveComponent.h"

#include "Actor.h"
#include "Game.h"

MoveComponent::MoveComponent(Actor* const pOwner, const int updateOrder)
	: Component(pOwner, updateOrder)
	, mAngularSpeed(0.f)
	, mForwardSpeed(0.f)
	, mMass(1.f)
	, mSumOfForces(Vector2::Zero)
	, mVelocity(Vector2::Zero)
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

	// if (!Math::IsNearZero(mForwardSpeed))
	{
		Vector2 pos = mpOwner->GetPosition();
		// pos += mpOwner->GetForward() * mForwardSpeed * deltaTime;

		const Vector2 acceleration = mSumOfForces * (1 / mMass);

		mVelocity += acceleration * deltaTime;

		pos += mVelocity * deltaTime;

		if (pos.X < 0.f)
		{
			pos.X = SCREEN_WIDTH - 2.f;
		}
		else if (pos.X > SCREEN_WIDTH)
		{
			pos.X = 2.f;
		}

		if (pos.Y < 0.f)
		{
			pos.Y = SCREEN_HEIGHT - 2.f;
		}
		else if (pos.Y > SCREEN_HEIGHT)
		{
			pos.Y = 2.f;
		}

		mpOwner->SetPosition(pos);
	}

	mSumOfForces = Vector2::Zero;
}

void MoveComponent::AddForce(const Vector2& force)
{
	mSumOfForces += force;
}

