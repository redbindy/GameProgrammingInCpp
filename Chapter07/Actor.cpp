#include "Actor.h"

#include <algorithm>

#include "Game.h"
#include "Component.h"

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
