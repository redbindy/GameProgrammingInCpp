#pragma once

#include "Component.h"
#include "Math.h"

class Actor;

class CircleComponent final : public Component
{
public:
	CircleComponent(Actor* const pOwner);
	virtual ~CircleComponent() = default;

	inline float GetRadius() const;
	inline void SetRadius(const float radius);

	const Vector2& GetCenter() const;

private:
	float mRadius;
};

inline void CircleComponent::SetRadius(const float radius)
{
	mRadius = radius;
}

bool Intersect(const CircleComponent& a, const CircleComponent& b);