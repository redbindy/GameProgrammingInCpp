#include "BoxComponent.h"

#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"

BoxComponent::BoxComponent(Actor* const pOwner, const int updateOrder)
	: Component(pOwner, updateOrder)
	, mObjectBox(Vector3::Zero, Vector3::Zero)
	, mWorldBox(Vector3::Zero, Vector3::Zero)
	, mbShouldRotate(true)
{
	Game* const pGame = pOwner->GetGame();
	PhysWorld* const pPhysWorld = pGame->GetPhysWorld();

	pPhysWorld->AddBox(this);
}

BoxComponent::BoxComponent(Actor* const pOwner)
	: BoxComponent(pOwner, 100)
{
}

BoxComponent::~BoxComponent()
{
	Game* const pGame = mpOwner->GetGame();
	PhysWorld* const pPhysWorld = pGame->GetPhysWorld();

	pPhysWorld->RemoveBox(this);
}

void BoxComponent::OnUpdateWorldTransform()
{
	mWorldBox = mObjectBox;

	mWorldBox.Min *= mpOwner->GetScale();
	mWorldBox.Max *= mpOwner->GetScale();

	if (mbShouldRotate)
	{
		mWorldBox.Rotate(mpOwner->GetRotation());
	}

	mWorldBox.Min += mpOwner->GetPosition();
	mWorldBox.Max += mpOwner->GetPosition();
}
