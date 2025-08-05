#pragma once

#include "Component.h"
#include "Math.h"

class Actor;

class MoveComponent : public Component
{
public:
	MoveComponent(Actor* const pOwner, const int updateOrder);
	MoveComponent(Actor* const pOwner);
	virtual ~MoveComponent() = default;

	virtual void Update(const float deltaTime) override;

	inline float GetAngularSpeed() const;
	inline void SetAngularSpeed(const float speed);

	inline float GetForwardSpeed() const;
	inline void SetForwardSpeed(const float speed);

private:
	float mAngularSpeed;
	float mForwardSpeed;
};

inline float MoveComponent::GetAngularSpeed() const
{
	return mAngularSpeed;
}

inline void MoveComponent::SetAngularSpeed(const float speed)
{
	mAngularSpeed = speed;
}

inline float MoveComponent::GetForwardSpeed() const
{
	return mForwardSpeed;
}

inline void MoveComponent::SetForwardSpeed(const float speed)
{
	mForwardSpeed = speed;
}
