#pragma once

#include "Component.h"
#include "Math.h"

class Actor;

class CircleComponent final : public Component
{
public:
	CircleComponent(Actor* const pOwner);
	virtual ~CircleComponent() = default;

	float GetRadius() const;
	inline void SetRadius(const float radius);

	const Vector3& GetCenter() const;

private:
	float mRadius;
};

bool Intersect(const CircleComponent& a, const CircleComponent& b);

inline void CircleComponent::SetRadius(const float radius)
{
	mRadius = radius;
}