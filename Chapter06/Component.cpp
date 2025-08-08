#include "Component.h"

#include "Actor.h"

Component::Component(Actor* const pOwner, const int updateOrder)
	: mpOwner(pOwner)
	, mUpdateOrder(updateOrder)
{
	pOwner->AddComponent(this);
}

Component::Component(Actor* const pOwner)
	: Component(pOwner, 100)
{
}

Component::~Component()
{
	mpOwner->RemoveComponent(this);
}

void Component::Update(const float deltaTime)
{

}

void Component::ProcessInput(const bool* const pbKeyStates)
{

}

void Component::OnUpdateWorldTransform()
{
}
