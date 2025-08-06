#include "Actor.h"

#include "Game.h"
#include "Component.h"

Actor::Actor(Game* pGame)
	: mState(EState::ACTIVE)
	, mPosition(Vector2::Zero)
	, mScale(1.f)
	, mRotation(0.f)
	, mComponentPtrs()
	, mpGame(pGame)
	, mWorldTransform()
	, mbReComputeWorldTransform(true)
{
	pGame->AddActor(this);
}

Actor::~Actor()
{
	mpGame->RemoveActor(this);

	while (!mComponentPtrs.empty())
	{
		delete mComponentPtrs.back();
	}
}

void Actor::Update(const float deltaTime)
{
	if (mState != EState::ACTIVE)
	{
		return;
	}

	ComputeWorldTransform();

	UpdateComponents(deltaTime);
	UpdateActor(deltaTime);

	ComputeWorldTransform();
}

void Actor::UpdateComponents(const float deltaTime)
{
	for (Component* const p : mComponentPtrs)
	{
		p->Update(deltaTime);
	}
}

void Actor::UpdateActor(const float deltaTime)
{

}

void Actor::ProcessInput(const bool* pbKeyStates)
{
	if (mState != EState::ACTIVE)
	{
		return;
	}

	for (Component* const p : mComponentPtrs)
	{
		p->ProcessInput(pbKeyStates);
	}

	ActorInput(pbKeyStates);
}

void Actor::ActorInput(const bool* pbKeyStates)
{

}

void Actor::AddComponent(Component* const pComponent)
{
	const int order = pComponent->GetUpdateOrder();
	std::vector<Component*>::iterator iter = mComponentPtrs.begin();

	while (iter != mComponentPtrs.end())
	{
		const int currOrder = (*iter)->GetUpdateOrder();
		if (currOrder > order)
		{
			break;
		}

		++iter;
	}

	mComponentPtrs.insert(iter, pComponent);
}

void Actor::RemoveComponent(Component* const pComponent)
{
	std::vector<Component*>::iterator iter = std::find(mComponentPtrs.begin(), mComponentPtrs.end(), pComponent);
	if (iter != mComponentPtrs.end())
	{
		mComponentPtrs.erase(iter);
	}
}

void Actor::ComputeWorldTransform()
{
	if (!mbReComputeWorldTransform)
	{
		return;
	}

	mbReComputeWorldTransform = false;

	mWorldTransform = Matrix4::CreateScale(mScale);
	mWorldTransform *= Matrix4::CreateRotationZ(mRotation);
	mWorldTransform *= Matrix4::CreateTranslation(Vector3(mPosition.X, mPosition.Y, 0.f));

	for (Component* const p : mComponentPtrs)
	{
		p->OnUpdateWorldTransform();
	}
}
