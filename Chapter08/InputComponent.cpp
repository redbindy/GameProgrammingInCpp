#include "InputComponent.h"

#include "Actor.h"
#include "InputSystem.h"

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

void InputComponent::ProcessInput(const InputState& const state)
{
	float forwardSpeed = 0.f;
	if (state.keyboard.GetKeyValue(SDL_Scancode(mForwardKey)))
	{
		forwardSpeed += mMaxForwardSpeed;
	}

	if (state.keyboard.GetKeyValue(SDL_Scancode(mBackKey)))
	{
		forwardSpeed -= mMaxForwardSpeed;
	}
	SetForwardSpeed(forwardSpeed);

	float angularSpeed = 0.f;
	if (state.keyboard.GetKeyValue(SDL_Scancode(mClockwiseKey)))
	{
		angularSpeed += mMaxAngularSpeed;
	}

	if (state.keyboard.GetKeyValue(SDL_Scancode(mCounterClockwiseKey)))
	{
		angularSpeed -= mMaxAngularSpeed;
	}
	SetAngularSpeed(angularSpeed);
}
