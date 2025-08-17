#include "BoxComponent.h"

#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "LevelLoader.h"

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

void BoxComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);

	JsonHelper::GetVector3(inObj, "objectMin", mObjectBox.Min);
	JsonHelper::GetVector3(inObj, "objectMax", mObjectBox.Max);
	JsonHelper::GetVector3(inObj, "worldMin", mWorldBox.Min);
	JsonHelper::GetVector3(inObj, "worldMax", mWorldBox.Max);
	JsonHelper::GetBool(inObj, "shouldRotate", mbShouldRotate);
}

void BoxComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::SaveProperties(alloc, inObj);

	JsonHelper::AddVector3(alloc, inObj, "objectMin", mObjectBox.Min);
	JsonHelper::AddVector3(alloc, inObj, "ObjectMax", mObjectBox.Max);
	JsonHelper::AddVector3(alloc, inObj, "worldMin", mWorldBox.Min);
	JsonHelper::AddVector3(alloc, inObj, "worldMax", mWorldBox.Max);
	JsonHelper::AddBool(alloc, inObj, "shouldRotate", mbShouldRotate);
}
