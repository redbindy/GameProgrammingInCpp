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

	void AddForce(const Vector2& force);

	inline float GetMass() const;
	inline void SetMass(const float mass);

	inline const Vector2& GetSumOfForces() const;

private:
	float mAngularSpeed;
	float mForwardSpeed;

	float mMass;
	Vector2 mSumOfForces;
	Vector2 mVelocity;
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

inline float MoveComponent::GetMass() const
{
	return mMass;
}

inline void MoveComponent::SetMass(const float mass)
{
	mMass = mass;
}

inline const Vector2& MoveComponent::GetSumOfForces() const
{
	return mSumOfForces;
}
