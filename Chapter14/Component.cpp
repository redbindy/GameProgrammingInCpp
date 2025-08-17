#include "Component.h"

#include "Actor.h"
#include "LevelLoader.h"

const char* Component::TypeNames[NUM_COMPONENT_TYPES] = {
	"Component",
	"AudioComponent",
	"BallMove",
	"BoxComponent",
	"CameraComponent",
	"FollowCamera",
	"MeshComponent",
	"MoveComponent",
	"SkeletalMeshComponent",
	"SpriteComponent",
	"MirrorCamera",
	"PointLightComponent",
	"TargetComponent"
};

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

void Component::ProcessInput(const bool* pbKeyState)
{
}

void Component::OnUpdateWorldTransform()
{

}

void Component::LoadProperties(const rapidjson::Value& inObj)
{
	JsonHelper::GetInt(inObj, "updateOrder", mUpdateOrder);
}

void Component::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	JsonHelper::AddInt(alloc, inObj, "updateOrder", mUpdateOrder);
}