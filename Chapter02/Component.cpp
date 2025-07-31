#include "Component.h"

Component::Component(class Actor* const pOwner, const int updateOrder)
	: mpOwner(pOwner)
	, mUpdateOrder(updateOrder)
{
	pOwner->AddComponent(this);
}

Component::Component(class Actor* const pOwner)
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