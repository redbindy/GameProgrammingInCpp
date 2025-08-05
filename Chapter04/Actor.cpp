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

	UpdateComponents(deltaTime);
	UpdateActor(deltaTime);
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
