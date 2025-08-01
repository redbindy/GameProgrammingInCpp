#include "InputComponent.h"

#include "Actor.h"

InputComponent::InputComponent(Actor* const pOwner)
	: MoveComponent(pOwner)
	, mMaxForwardSpeed(0.f)
	, mMaxAngularSpeed(0.f)
	, mForwardKey(0)
	, mBackKey(0)
	, mClockwiseKey(0)
	, mCounterClockwiseKey(0)
{

}

void InputComponent::ProcessInput(const bool* const pbKeyStates)
{
	float forwardSpeed = 0.f;
	if (pbKeyStates[mForwardKey])
	{
		// forwardSpeed += mMaxForwardSpeed;
		AddForce(mpOwner->GetForward() * mMaxForwardSpeed);
	}

	if (pbKeyStates[mBackKey])
	{
		// forwardSpeed -= mMaxForwardSpeed;
		AddForce(mpOwner->GetForward() * -1 * mMaxForwardSpeed);
	}
	SetForwardSpeed(forwardSpeed);

	float angularSpeed = 0.f;
	if (pbKeyStates[mClockwiseKey])
	{
		angularSpeed += mMaxAngularSpeed;
	}

	if (pbKeyStates[mCounterClockwiseKey])
	{
		angularSpeed -= mMaxAngularSpeed;
	}
	SetAngularSpeed(angularSpeed);
}
