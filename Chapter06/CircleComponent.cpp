#include "CircleComponent.h"

#include "Actor.h"
#include "Mesh.h"

CircleComponent::CircleComponent(Actor* const pOwner)
	: Component(pOwner)
	, mRadius(0.f)
{

}

float CircleComponent::GetRadius() const
{
	return mRadius * mpOwner->GetScale();
}

const Vector3& CircleComponent::GetCenter() const
{
	return mpOwner->GetPosition();
}

bool Intersect(const CircleComponent& a, const CircleComponent& b)
{
	const Vector3 diff = a.GetCenter() - b.GetCenter();
	const float distSq = diff.LengthSq();

	float radiiSq = a.GetRadius() + b.GetRadius();
	radiiSq *= radiiSq;

	return distSq <= radiiSq;
}
