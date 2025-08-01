#pragma once

#include "MoveComponent.h"

class Actor;

class InputComponent final : public MoveComponent
{
public:
	InputComponent(Actor* const pOwner);
	virtual ~InputComponent() = default;

	void ProcessInput(const bool* const pbKeyStates) override;

	inline float GetMaxForward() const;
	inline void SetMaxForwardSpeed(const float speed);

	inline float GetMaxAngular() const;
	inline void SetMaxAngularSpeed(const float angular);

	inline int GetForwardKey() const;
	inline void SetForwardKey(const int key);

	inline int GetBackKey() const;
	inline void SetBackKey(const int key);

	inline int GetClockwiseKey() const;
	inline void SetClockwiseKey(const int key);

	inline int GetCounterClockwiseKey() const;
	inline void SetCounterClockwiseKey(const int key);

private:
	float mMaxForwardSpeed;
	float mMaxAngularSpeed;

	int mForwardKey;
	int mBackKey;

	int mClockwiseKey;
	int mCounterClockwiseKey;
};

inline float InputComponent::GetMaxForward() const
{
	return mMaxForwardSpeed;
}

inline void InputComponent::SetMaxForwardSpeed(const float speed)
{
	mMaxForwardSpeed = speed;
}

inline float InputComponent::GetMaxAngular() const
{
	return mMaxAngularSpeed;
}

inline void InputComponent::SetMaxAngularSpeed(const float angular)
{
	mMaxAngularSpeed = angular;
}

inline int InputComponent::GetForwardKey() const
{
	return mForwardKey;
}

inline void InputComponent::SetForwardKey(const int key)
{
	mForwardKey = key;
}

inline int InputComponent::GetBackKey() const
{
	return mBackKey;
}

inline void InputComponent::SetBackKey(const int key)
{
	mBackKey = key;
}

inline int InputComponent::GetClockwiseKey() const
{
	return mClockwiseKey;
}

inline void InputComponent::SetClockwiseKey(const int key)
{
	mClockwiseKey = key;
}

inline int InputComponent::GetCounterClockwiseKey() const
{
	return mCounterClockwiseKey;
}

inline void InputComponent::SetCounterClockwiseKey(const int key)
{
	mCounterClockwiseKey = key;
}
