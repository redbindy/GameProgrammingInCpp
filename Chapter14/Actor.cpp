#include "Actor.h"

#include <algorithm>

#include "Game.h"
#include "LevelLoader.h"

const char* Actor::TypeNames[NUM_ACTOR_TYPES] = {
	"Actor",
	"BallActor",
	"FollowActor",
	"PlaneActor",
	"TargetActor",
};

Actor::Actor(Game* const pGame)
	: mState(EState::ACTIVE)
	, mWorldTransform(Matrix4::Identity)
	, mPosition(Vector3::Zero)
	, mRotation(Quaternion::Identity)
	, mScale(1.f)
	, mpGame(pGame)
	, mbRecomputeWorldTransform(true)
{
	pGame->AddActor(this);
}

Actor::~Actor()
{
	mpGame->RemoveActor(this);

	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::Update(const float deltaTime)
{
	if (mState != EState::ACTIVE)
	{
		return;
	}

	ComputeWorldTransform();

	UpdateComponent(deltaTime);
	UpdateActor(deltaTime);

	ComputeWorldTransform();
}

void Actor::UpdateComponent(const float deltaTime)
{
	for (Component* const p : mComponents)
	{
		p->Update(deltaTime);
	}
}

void Actor::UpdateActor(const float deltaTime)
{

}

void Actor::ProcessInput(const bool* const pbKeyState)
{
	if (mState != EState::ACTIVE)
	{
		return;
	}

	for (Component* const p : mComponents)
	{
		p->ProcessInput(pbKeyState);
	}

	ActorInput(pbKeyState);
}

void Actor::ActorInput(const bool* const pbKeyState)
{
}

void Actor::ComputeWorldTransform()
{
	if (!mbRecomputeWorldTransform)
	{
		return;
	}

	mbRecomputeWorldTransform = false;

	mWorldTransform = Matrix4::CreateScale(mScale);
	mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
	mWorldTransform *= Matrix4::CreateTranslation(mPosition);

	for (Component* const p : mComponents)
	{
		p->OnUpdateWorldTransform();
	}
}

void Actor::AddComponent(Component* const pComponent)
{
	const int order = pComponent->GetUpdateOrder();

	std::vector<Component*>::iterator iter;
	for (iter = mComponents.begin(); iter != mComponents.end(); ++iter)
	{
		if (order < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	mComponents.insert(iter, pComponent);
}

void Actor::RemoveComponent(Component* const pComponent)
{
	std::vector<Component*>::iterator iter = std::find(mComponents.begin(), mComponents.end(), pComponent);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}

void Actor::RotateToNewForward(const Vector3& forward)
{
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);

	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::PI));
	}
	else
	{
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();

		SetRotation(Quaternion(axis, angle));
	}
}

void Actor::LoadProperties(const rapidjson::Value& inObj)
{
	std::string state;
	if (JsonHelper::GetString(inObj, "state", state))
	{
		if (state == "active")
		{
			SetState(EState::ACTIVE);
		}
		else if (state == "paused")
		{
			SetState(EState::PAUSED);
		}
		else if (state == "dead")
		{
			SetState(EState::DEAD);
		}
	}

	JsonHelper::GetVector3(inObj, "position", mPosition);
	JsonHelper::GetQuaternion(inObj, "rotation", mRotation);
	JsonHelper::GetFloat(inObj, "scale", mScale);

	ComputeWorldTransform();
}

void Actor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	std::string state = "active";
	if (mState == EState::PAUSED)
	{
		state = "paused";
	}
	else if (mState == EState::DEAD)
	{
		state = "dead";
	}

	JsonHelper::AddString(alloc, inObj, "state", state);
	JsonHelper::AddVector3(alloc, inObj, "position", mPosition);
	JsonHelper::AddQuaternion(alloc, inObj, "rotation", mRotation);
	JsonHelper::AddFloat(alloc, inObj, "scale", mScale);
}
